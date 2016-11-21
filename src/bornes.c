/**
 * \file brones.c
 * \brief implémentations de fonctions de calcul de bornes pour le SSCFLP
 */

#include "bornes.h"
#include "stdio.h"
#include "stdlib.h"


//------------------------------------------------------------------------------
void construction(Solution* sol) {

    double* cmin = malloc((long unsigned int)sol->pb->n*sizeof(double));

    // calculs des cmin_i, les couts d'affectation minimaux par clients
    for(int i = 0; i < sol->pb->n; i++) {
        for(int j = 0; j < sol->pb->m; j++) {
            if(j == 0 || sol->pb->liaisons[j][i] < cmin[i]) {
                cmin[i] = sol->pb->liaisons[j][i];
            }
        }
    }

    // tableau des nombres de clients non affectés par services
    int* nbLibre = malloc((long unsigned int)sol->pb->m*sizeof(int));
    for(int i = 0; i < sol->pb->m; i++) {
        nbLibre[i] = sol->pb->n; // pour l'instance aucune affectation n'a été faite
    }

    // tableau de booléen comprenant les services utiliséss
    int* utilise = malloc((long unsigned int)sol->pb->m*sizeof(int));
    for(int i = 0; i < sol->pb->m; i++) {
        utilise[i] = 0;
    }

    int nbClientAffecte = 0;

    int continuer = 1;
    while(continuer) {

        // sélection du service à ouvrir
        double utiliteMin = 0;
        int serviceMin = 0;
        for(int i = 0; i < sol->pb->m; i++) {

            if(!utilise[i]) {

                // calcul de l'utilité pour chaque service
                double utilite = 0;
                for(int j = 0; j < sol->pb->n; j++) {
                    utilite += sol->pb->liaisons[i][j] - cmin[j];
                }
                utilite += sol->pb->couts[i];
                utilite /= nbLibre[i];

                if(i == 0 || utilite < utiliteMin) {
                    utiliteMin = utilite;
                    serviceMin = i;
                }
            }

        }

        continuer = 0;
    }


    free(utilise);
    free(nbLibre);
    free(cmin);
}
