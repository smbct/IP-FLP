/**
 * \file tabu.c
 * \brief implémentation de la métaheuristique tabu
 */

#include "tabu.h"

#include "bornes.h"

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
void rechercheTabu(Solution* meilleure) {

    // construction gloutonne
    construction(meilleure);

    printf("solution gloutonne : \n");
    afficherSolution(meilleure);
    printf("\n\n");

    Solution sol;
    creerSolution(meilleure->pb, &sol);
    copierSolution(meilleure, &sol);

    // construction de la liste tabu
    int** tabuListe = malloc((long unsigned int)sol.pb->n*sizeof(int*));
    for(int i = 0; i < sol.pb->n; i++) {
        tabuListe[i] = malloc((long unsigned int)sol.pb->m*sizeof(int));
        for(int j = 0; j < sol.pb->m; j++) {
            tabuListe[i][j] = 0;
        }
    }

    int longueurTabu = 7;

    int nbIt = 0;
    int nbItMax = 1000;

    while(nbIt < nbItMax) {

        selectionnerVoisin(&sol, meilleure, tabuListe, nbIt, longueurTabu);

        // si la solution actuelle est meilleure que la meilleure trouvée, mise à jour
        if(sol.z < meilleure->z) {
            copierSolution(&sol, meilleure);
            nbItMax += 1000;
        }

        nbIt ++;
    }

    afficherSolution(meilleure);

    detruireSolution(&sol);

    for(int i = 0; i < sol.pb->n; i++) {
        free(tabuListe[i]);
    }
    free(tabuListe);
}

//------------------------------------------------------------------------------
void selectionnerVoisin(Solution* sol, Solution* meilleure, int** tabuListe, int it, int longueurTabu) {

    // voisinage : modification d'une connexion
    int clientMin = -1;
    int serviceMin = -1;
    double bestZ = sol->z;

    for(int i = 0; i < sol->pb->n; i++) {

        for(int j = 0; j < sol->pb->m; j++) {

            // vérification que la solution reste admissible
            if(sol->capaRestantes[j] >= sol->pb->demandes[i]) {

                double z = sol->z;
                z -= sol->pb->liaisons[sol->connexionClient[i]][i];
                z += sol->pb->liaisons[j][i];

                if(sol->nbClientsServices[j] == 0) {
                    z += sol->pb->couts[j];
                }
                if(sol->nbClientsServices[sol->connexionClient[i]] == 1) {
                    z -= sol->pb->couts[sol->connexionClient[i]];
                }

                if( (clientMin == -1 || z < bestZ) && (tabuListe[i][j] <= it || z < meilleure->z) ) {
                    bestZ = z;
                    clientMin = i;
                    serviceMin = j;
                }

            }

        }

    }

    // mise à jour de la solution
    sol->z -= sol->pb->liaisons[sol->connexionClient[clientMin]][clientMin];
    sol->z += sol->pb->liaisons[serviceMin][clientMin];

    // mise à jour du nombre de client connectés à chaque service et des services ouverts
    sol->nbClientsServices[sol->connexionClient[clientMin]] --;
    if(sol->nbClientsServices[sol->connexionClient[clientMin]] == 0) {
        sol->z -= sol->pb->couts[sol->connexionClient[clientMin]];
        sol->services[sol->connexionClient[clientMin]] = 0;
        sol->nbServicesOuverts --;
    }

    sol->nbClientsServices[serviceMin] ++;
    if(sol->nbClientsServices[serviceMin] == 1) {
        sol->z += sol->pb->couts[serviceMin];
        sol->services[serviceMin] = 1;
        sol->nbServicesOuverts ++;
    }

    sol->capaRestantes[sol->connexionClient[clientMin]] += sol->pb->demandes[clientMin];
    sol->capaRestantes[serviceMin] -= sol->pb->demandes[clientMin];

    sol->connexionClient[clientMin] = serviceMin;

    // le mouvement devient tabu
    tabuListe[clientMin][serviceMin] = it + longueurTabu;


}
