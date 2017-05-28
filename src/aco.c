/**
 * \file aco.c
 * \brief implmentation de la métaheuristique des colonies de fourmies
 */

#include "aco.h"
#include "tri.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//------------------------------------------------------------------------------
void construireACO(Solution* best, int localsearch, int tabuListLenght, long tmax, double alpha, double beta, double rho, double pheromone_init, int n_ants, int pheremononeUpdateScheme, int nb_elit, double nu, unsigned int seed) {
    //variable
        long tstart = clock(); //pour le critère d'arret
        int n = best->pb->n; //nombre de client
        int m = best->pb->m; //nombre de service
        int i,j, tmp; //compteur de boucle et variable de traitement
        double ** pheromone; //matrice des pheromones
        double ** heuristic; //matrice de l'information heuristique
        double ** probability; //information combine des pheromones et de l'heuristique

    //début
        //initialisation
            srand(seed); // initialisation du générateur aléatoire

            // initialisation des matrices
            pheromone = malloc((long unsigned int)n*sizeof(double*));
            heuristic = malloc((long unsigned int)n*sizeof(double*));
            probability = malloc((long unsigned int)n*sizeof(double*));
            for(i = 0; i < n; ++i) {
                pheromone[i] = malloc((long unsigned int)m*sizeof(double));
                heuristic[i] = malloc((long unsigned int)m*sizeof(double));
                probability[i] = malloc((long unsigned int)m*sizeof(double));
                for (j = 0; j<m; ++j) {
                    //valeur initiale des pheromones
                    pheromone[i][j] = pheromone_init;

                    //calcul de la valeur heuristique
                    heuristic[i][j] = 1.0 / (0.1 + best->pb->liaisons[j][i]);
                }
            }

            //calcul des probas
            calculProba(pheromone, heuristic, probability, n, m, alpha, beta);

            //population initiale
            Solution* solFourmi = malloc((long unsigned int)n_ants*sizeof(Solution));
            for(i = 0; i < n_ants; ++i) {
                creerSolution(best->pb, &solFourmi[i]);
                constructionFourmi(&solFourmi[i], probability);
            }

            //initialisation du best
            tmp = 0;
            for (i = 1; i<n_ants; ++i) {
                if (solFourmi[i].z < solFourmi[tmp].z) { tmp = i; }
            }

            copierSolution(&solFourmi[tmp], best);
            
        //recherche
        while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
            //mise a jour des pheromones
            majPheromones(best, n_ants, solFourmi, pheromone, rho, pheremononeUpdateScheme, nb_elit, nu);

            //mise a jour des probabilite
            calculProba( pheromone, heuristic, probability, n, m, alpha, beta);

            //nouvelle population
            for (i = 0; i<n_ants; ++i) {
                resetSolution(&solFourmi[i]);
                constructionFourmi(&solFourmi[i], probability);   
            }

            //mise a jour du best
            tmp = -1;
            for (i = 0; i<n_ants; ++i) {
                if (solFourmi[i].z < best->z) { tmp = i; }
            }
            if (tmp >= 0) {copierSolution(&solFourmi[tmp], best);}

        }
    //fin
}

//------------------------------------------------------------------------------
void constructionFourmi(Solution* sol, double** probability) { //TODO la recherche locale
    //variables
        int continuer = 1;
        double somme, somme2; //pour la roulette biaise
        double min; //valeur de la probabilité minimale
        double tirage; //valeur aléaoire uniformement distribué
        int i; //compteur de boucle
        int trouve; //si on a trouve notre facilite

    //debut
    while(continuer) {
        // sélection d'une connexion pour le premier client non affecté
        // roulette biaisée pour sélectionner le service auquel l'affecter
        somme = 0;
        min = -1.;

        for(i = 0; i < sol->pb->m; i++) {
            if(sol->capaRestantes[i] >= sol->pb->demandes[sol->nbVarClientFixees]) {
                somme += probability[sol->nbVarClientFixees][i];
                if(min < 0 || min > probability[sol->nbVarClientFixees][i]) {
                    min = probability[sol->nbVarClientFixees][i];
                }
            }
        }

        if(min > 0) {
            tirage = aleatoire(min, somme);
            somme2 = 0;
            i = 0;
            trouve = 0;

            // recherche du service correspondant au nombre tiré aléatoirement (roulette biaisée)
            while(!trouve && i < sol->pb->m) {
                if(sol->capaRestantes[i] >= sol->pb->demandes[sol->nbVarClientFixees]) {
                    if(somme2 + probability[sol->nbVarClientFixees][i] >= tirage) {
                        trouve = 1;
                    } else {
                        somme2 += probability[sol->nbVarClientFixees][i];
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
void majPheromones(Solution* best, int n_ants, Solution* solFourmi, double** pheromone, double rho, int pheremononeUpdateScheme, int nb_elit, double nu) {
    int i,j; //compteurs de boucle
    int n = best->pb->n;
    int m = best->pb->m;

    //evaporation (commun à tous)
    for(i = 0; i<n; ++i) {
        for(j = 0; j<m; ++j) {
            pheromone[i][j] *= rho;
        }
    }

    if (0 == pheremononeUpdateScheme) { //ACO
        //toutes les fourmies ajoutent de manière identiques
        for(i = 0; i<n_ants; ++i) { //pour toutes les fourmis 
            for(j = 0; j<n; ++j) { //pour tout les client
                pheromone[j][solFourmi[i].connexionClient[j]] += 1.0 / solFourmi[i].z;
            }
        }
    } else if(1 == pheremononeUpdateScheme) { //EAS
        //tri des solutions
        int * indices = malloc((long unsigned int)n_ants*sizeof(int));
        double * val = malloc((long unsigned int)n_ants*sizeof(double));
        for(i = 0; i<n_ants; ++i) {
            indices[i] = i;
            val[i] = solFourmi[i].z;
        }
        trierCroissant(n_ants, indices, val, val);
        
        //seul les meilleurs ajoutent
        for (i = 0; i<nb_elit; ++i) {
            for(j = 0; j<n; ++j) {
                pheromone[j][solFourmi[indices[i]].connexionClient[j]] += 1.0 / solFourmi[indices[i]].z;
            }
        }
    } else/* if(2 == pheremononeUpdateScheme) */{ //rank base
        //tri des solutions
        int * indices = malloc((long unsigned int)n_ants*sizeof(int));
        double * val = malloc((long unsigned int)n_ants*sizeof(double));
        for(i = 0; i<n_ants; ++i) {
            indices[i] = i;
            val[i] = solFourmi[i].z;
        }
        trierCroissant(n_ants, indices, val, val);

        //ajout avec decroissance
        double acc = 1.0;
        for (i = 0; i<nb_elit; ++i) {
            for(j = 0; j<n; ++j) {
                pheromone[j][solFourmi[indices[i]].connexionClient[j]] += acc / solFourmi[indices[i]].z;
            }
            acc *= nu;
        }
    }

}

void calculProba(double ** pheromone, double ** heuristic, double ** probability, int n, int m, double alpha, double beta) {
    for (int i = 0; i<n; ++i) {
        for (int j = 0; j<m; ++j) {
            probability[i][j] = pow(pheromone[i][j], alpha) + pow(heuristic[i][j], beta); //non normalisé car cela dependra de l'avancement de la construction
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
