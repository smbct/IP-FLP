/**
 * \file rechercheLocale.c
 * \brief implémentation de procédures de recherche locale
 */

#include "rechercheLocale.h"

//------------------------------------------------------------------------------
void rechercheLocale(Solution* meilleure, int bestImp) {

    int improve = 1;

    while(improve) {
        improve = chercherVoisin(meilleure, bestImp);
    }

}

//------------------------------------------------------------------------------
int chercherVoisin(Solution* sol, int bestImp) {

    int res = 0;

    // voisinage : modification d'une connexion
    int clientMin = -1;
    int serviceMin = -1;
    double bestZ = sol->z;

    for(int i = 0; i < sol->pb->n; i++) {

        for(int j = 0; j < sol->pb->m; j++) {

            // vérification que le service proposé est différent et la solution reste admissible
            if(j != sol->connexionClient[i] && sol->capaRestantes[j] >= sol->pb->demandes[i]) {

                // calcul du nouveau cout avec ce service
                double z = sol->z;
                z -= sol->pb->liaisons[sol->connexionClient[i]][i];
                z += sol->pb->liaisons[j][i];

                if(sol->nbClientsServices[j] == 0) { // le service doit être ouvert
                    z += sol->pb->couts[j];
                }
                if(sol->nbClientsServices[sol->connexionClient[i]] == 1) { // l'autre service peut être fermé
                    z -= sol->pb->couts[sol->connexionClient[i]];
                }

                if(bestImp) {

                    if(clientMin == -1 || z < bestZ) {
                        bestZ = z;
                        clientMin = i;
                        serviceMin = j;
                    }

                } else if(z < sol->z) {
                    // mise à jour de la solution
                    majSolution(sol, z, i, j);
                    res = 1;
                }
            }
        }
    }

    // mise à jour de la solution
    if(bestImp && bestZ < sol->z) {
        res = 1;
        majSolution(sol, bestZ, clientMin, serviceMin);
    }

    return res;
}

//------------------------------------------------------------------------------
void majSolution(Solution* sol, double z, int n, int nouvm) {

    sol->z = z;

    // mise à jour du nombre de client connectés à chaque service et des services ouverts
    sol->nbClientsServices[sol->connexionClient[n]] --;
    if(sol->nbClientsServices[sol->connexionClient[n]] == 0) {
        sol->services[sol->connexionClient[n]] = 0;
        sol->nbServicesOuverts --;
    }

    sol->nbClientsServices[nouvm] ++;
    if(sol->nbClientsServices[nouvm] == 1) {
        sol->services[nouvm] = 1;
        sol->nbServicesOuverts ++;
    }

    sol->capaRestantes[sol->connexionClient[n]] += sol->pb->demandes[n];
    sol->capaRestantes[nouvm] -= sol->pb->demandes[n];

    sol->connexionClient[n] = nouvm;

}
