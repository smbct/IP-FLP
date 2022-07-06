/**
 * \file tabu.c
 * \brief implémentation de la métaheuristique tabu
 */

#include "tabu.h"

#include "bornes.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------
void rechercheTabu(Solution* meilleure, int longueurTabu, long tmax) {

    // construction gloutonne, une solution initiale est nécessaire
    construction(meilleure);

    // printf("la construction donne z = %lf\n", meilleure->z);
    long tstart = clock(); //pour le critère d'arret
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

    int nbIt = 0;

    while(clock() - tstart < tmax*CLOCKS_PER_SEC) {

        selectionnerVoisin(&sol, meilleure, tabuListe, nbIt, longueurTabu);
        // selectionnerVoisin2(&sol, meilleure, tabuListe, nbIt, longueurTabu);


        // si la solution actuelle est meilleure que la meilleure trouvée, mise à jour
        if(sol.z < meilleure->z) {
            copierSolution(&sol, meilleure);
        }

        nbIt ++;
    }

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


            // vérification que le service proposé est différent et la solution reste admissible
            if(j != sol->connexionClient[i] && sol->capaRestantes[j] >= sol->pb->demandes[i]) {

                double z = sol->z;
                z -= sol->pb->liaisons[sol->connexionClient[i]][i];
                z += sol->pb->liaisons[j][i];

                if(sol->nbClientsServices[j] == 0) { // le service doit être ouvert
                    z += sol->pb->couts[j];
                }
                if(sol->nbClientsServices[sol->connexionClient[i]] == 1) { // l'autre service peut être fermé
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

    // printf("meilleur actuel : %lf\n", meilleure->z);
    // printf("meilleure itération : z = %lf\n", bestZ);

    // mise à jour de la solution
    sol->z = bestZ;

    // mise à jour du nombre de client connectés à chaque service et des services ouverts
    sol->nbClientsServices[sol->connexionClient[clientMin]] --;
    if(sol->nbClientsServices[sol->connexionClient[clientMin]] == 0) {
        sol->services[sol->connexionClient[clientMin]] = 0;
        sol->nbServicesOuverts --;
    }

    sol->nbClientsServices[serviceMin] ++;
    if(sol->nbClientsServices[serviceMin] == 1) {
        sol->services[serviceMin] = 1;
        sol->nbServicesOuverts ++;
    }

    sol->capaRestantes[sol->connexionClient[clientMin]] += sol->pb->demandes[clientMin];
    sol->capaRestantes[serviceMin] -= sol->pb->demandes[clientMin];

    sol->connexionClient[clientMin] = serviceMin;

    // le mouvement devient tabu
    tabuListe[clientMin][serviceMin] = it + longueurTabu;

}

//------------------------------------------------------------------------------
void selectionnerVoisin2(Solution* sol, Solution* meilleure, int** tabuListe, int it, int longueurTabu) {

    // swap de des services entre deux clients

    // voisinage : modification d'une connexion
    int clientMin1 = -1;
    int clientMin2 = -1;
    double bestZ = sol->z;

    for(int i = 0; i < sol->pb->n; i++) {

        for(int j = i+1; j < sol->pb->n; j++) {

            // vérification que l'échange est utile (services différents)
            if(sol->connexionClient[j] != sol->connexionClient[i]) {

                // verif solution toujours admissible après échange
                if(sol->pb->demandes[j]-sol->pb->demandes[i] <= sol->capaRestantes[sol->connexionClient[i]] && sol->pb->demandes[i]-sol->pb->demandes[j] <= sol->capaRestantes[sol->connexionClient[j]]) {

                    double z = sol->z;
                    z -= sol->pb->liaisons[sol->connexionClient[i]][i];
                    z += sol->pb->liaisons[sol->connexionClient[j]][i];
                    z -= sol->pb->liaisons[sol->connexionClient[j]][j];
                    z += sol->pb->liaisons[sol->connexionClient[i]][j];


                    if( (clientMin1 == -1 || z < bestZ) && (tabuListe[i][j] <= it || z < meilleure->z) ) {
                        bestZ = z;
                        clientMin1 = i;
                        clientMin2 = j;
                    }


                }
            }
        }
    }

    if(clientMin1 != -1) {

        // mise à jour de la solution
        sol->z = bestZ;

        sol->capaRestantes[sol->connexionClient[clientMin1]] += sol->pb->demandes[clientMin2]-sol->pb->demandes[clientMin1];
        sol->capaRestantes[sol->connexionClient[clientMin2]] += sol->pb->demandes[clientMin1]-sol->pb->demandes[clientMin2];

        int tempService = sol->connexionClient[clientMin1];
        sol->connexionClient[clientMin1] = sol->connexionClient[clientMin2];
        sol->connexionClient[clientMin2] = tempService;

        // le mouvement devient tabu
        tabuListe[clientMin1][sol->connexionClient[clientMin1]] = it + longueurTabu;
        tabuListe[clientMin2][sol->connexionClient[clientMin2]] = it + longueurTabu;


    }


}
