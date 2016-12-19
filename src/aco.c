/**
 * \file aco.c
 * \brief implmentation de la métaheuristique des colonies de fourmies
 */

#include "aco.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//------------------------------------------------------------------------------
void construireACO(Solution* best) {

    best->z = -1.;

    // initialisation du générateur aléatoire
    srand((unsigned int)time(NULL));

    int nbFourmi = 50;
    Solution* solFourmi = malloc((long unsigned int)nbFourmi*sizeof(Solution));
    for(int i = 0; i < nbFourmi; i++) {
        creerSolution(best->pb, &solFourmi[i]);

    }

    // phéromones des connexions clients*services
    double** pheroConn = malloc((long unsigned int)best->pb->n*sizeof(double*));
    for(int i = 0; i < best->pb->n; i++) {
        pheroConn[i] = malloc((long unsigned int)best->pb->m*sizeof(double));
    }

    // initialisation des phéromones, de manière équilibrée
    for(int i = 0; i < best->pb->n; i++) {
        for(int j = 0; j < best->pb->m; j++) {
            pheroConn[i][j] = 1000.;
        }
    }

    // lancer 100 itérations de fourmis
    for(int it = 0; it < 1000; it++) {

        printf("\n\nitération #%d\n", it);
        for(int i = 0; i < nbFourmi; i++) {

            resetSolution(&solFourmi[i]);
            constructionFourmi(&solFourmi[i], pheroConn);

        }

        majPheromones(best, nbFourmi, solFourmi, pheroConn);

    }

    printf("phéromones à la fin : \n");
    for(int i = 0; i < best->pb->n; i++) {
        printf("client %d : ", i);
        for(int j = 0; j < best->pb->m; j++) {
            printf("%lf, ", pheroConn[i][j]);
        }
        printf("\n");
    }


    printf("construction d'une solution par une fourmi : z = %lf\n", best->z);

    for(int i = 0; i < best->pb->n; i++) {
        free(pheroConn[i]);
    }
    free(pheroConn);

    for(int i = 0; i < nbFourmi; i++) {
        detruireSolution(&solFourmi[i]);
    }
    free(solFourmi);
}

//------------------------------------------------------------------------------
void constructionFourmi(Solution* sol, double** pheroConn) {

    int continuer = 1;

    while(continuer) {

        // sélection d'une connexion pour le premier client non affecté
        // roulette biaisée pour sélectionner le service auquel l'affecter

        double somme = 0;
        double min = -1.;

        for(int i = 0; i < sol->pb->m; i++) {
            if(sol->capaRestantes[i] >= sol->pb->demandes[sol->nbVarClientFixees]) {
                somme += pheroConn[sol->nbVarClientFixees][i];
                if(min < 0 || min > pheroConn[sol->nbVarClientFixees][i]) {
                    min = pheroConn[sol->nbVarClientFixees][i];
                }
            }
        }

        if(min > 0) {
            double tirage = aleatoire(min, somme);
            double somme2 = 0;
            int i = 0;
            int trouve = 0;

            // recherche du service correspondant au nombre tiré aléatoirement (roulette biaisée)
            while(!trouve && i < sol->pb->m) {
                if(sol->capaRestantes[i] >= sol->pb->demandes[sol->nbVarClientFixees]) {
                    if(somme2 + pheroConn[sol->nbVarClientFixees][i] >= tirage) {
                        trouve = 1;
                    } else {
                        somme2 += pheroConn[sol->nbVarClientFixees][i];
                    }
                }
                if(!trouve) {
                    i ++;
                }

            }

            // connexion au service trouvé dans la solution
            sol->connexionClient[sol->nbVarClientFixees] = i;
            sol->z += sol->pb->liaisons[i][sol->nbVarClientFixees];
            if(sol->services[i] != 1) {
                if(sol->services[i] == -1) {
                    sol->nbVarServicesFixees ++;
                }
                sol->services[i] = 1;
                sol->z += sol->pb->couts[i];
            }

            sol->nbVarClientFixees ++;
        } else { // aucune connexion possible, la fourmi est bloquée
            continuer = 0;
            sol->z = -1.;
        }

        if(sol->nbVarClientFixees == sol->pb->n) {
            continuer = 0; // une solution a été trouvée
        }

    }

}

//------------------------------------------------------------------------------
void majPheromones(Solution* best, int nbFourmi, Solution* solFourmi, double** phero) {

    double meilleure = -1.;
    double moinsBonne = -1.;

    double bestZ = best->z;

    for(int i = 0; i < nbFourmi; i++) {
        if( (meilleure < 0 || solFourmi[i].z < meilleure) && solFourmi[i].z > 0) {
            meilleure = solFourmi[i].z;
        }
        if( (moinsBonne < 0 || solFourmi[i].z > moinsBonne) && solFourmi[i].z > 0) {
            moinsBonne = solFourmi[i].z;
        }
    }

    for(int i = 0; i < nbFourmi; i++) {
        for(int j = 0; j < solFourmi[i].pb->n; j++) {

            if(solFourmi[i].z > 0) {

                double ratio = (solFourmi[i].z-moinsBonne)/(meilleure-moinsBonne);
                // printf("ratio : %lf\n", ratio);
                if(solFourmi[i].z < bestZ) { // accuentation quand la meilleure solution est dépassée
                    phero[j][solFourmi[i].connexionClient[j]] += phero[j][solFourmi[i].connexionClient[j]]*0.8*ratio;
                } else { // diminution autrement
                    phero[j][solFourmi[i].connexionClient[j]] -= phero[j][solFourmi[i].connexionClient[j]]*0.000004*ratio;
                }

                // évaporation
                phero[j][solFourmi[i].connexionClient[j]] -= 2.;
                if(phero[j][solFourmi[i].connexionClient[j]] < 0) {
                    phero[j][solFourmi[i].connexionClient[j]] = 0.;
                }

                // mise à jour de la meilleure solution connue
                if(best->z < 0 || solFourmi[i].z < best->z) {
                    copierSolution(&solFourmi[i], best);
                    printf("amélioration : z = %lf\n", best->z);
                }

            }

        }

    }

}

//------------------------------------------------------------------------------
double aleatoire(double min, double max) {

    double res = min;

    int alea = rand()%10000;
    double norm = (double)alea / 10000.;

    return res + (max-min)*norm;

}
