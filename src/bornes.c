/**
 * \file brones.c
 * \brief implémentations de fonctions de calcul de bornes pour le SSCFLP
 */

#include <stdio.h>
#include <stdlib.h>

#include "bornes.h"
#include "tri.h"


//------------------------------------------------------------------------------
void construction(Solution* sol) {

    sol->z = 0;

    double* cmin = malloc((long unsigned int)sol->pb->n*sizeof(double));

    // calculs des cmin_i, les couts d'affectation minimaux par clients
    for(int i = 0; i < sol->pb->n; i++) {
        for(int j = 0; j < sol->pb->m; j++) {
            if(j == 0 || sol->pb->liaisons[j][i] < cmin[i]) {
                cmin[i] = sol->pb->liaisons[j][i];
            }
        }
    }

    // delta_{ij} = c_{ij} - cmin_i
    double** deltas = malloc((long unsigned int)sol->pb->m*sizeof(double*));
    for(int i = 0; i < sol->pb->m; i++) {
        deltas[i] = malloc((long unsigned int)sol->pb->n*sizeof(double));
        for(int j = 0; j < sol->pb->n; j++) {
            deltas[i][j] = sol->pb->liaisons[i][j] - cmin[j];
        }
    }

    // tableau des clients affectés
    int* affectes = malloc((long unsigned int)sol->pb->n*sizeof(int));
    for(int i = 0; i < sol->pb->n;i++) {
        affectes[i] = 0;
    }

    // tableau des clients par service triés en fonction des couts
    int** clientsTries = malloc((long unsigned int)sol->pb->m*sizeof(int*));
    for(int i = 0; i < sol->pb->m; i++) {
        clientsTries[i] = malloc((long unsigned int)sol->pb->n*sizeof(int));
        for(int j = 0; j < sol->pb->n; j++) {
            clientsTries[i][j] = j;
        }
        // tri des clients
        trierCroissant(sol->pb->n, clientsTries[i], deltas[i], sol->pb->demandes);
    }

    // tri des clients pour chaque usine en fonction des cmin

    int nbClientAffecte = 0;
    int nbServiceOuvert = 0;

    int continuer = 1;
    while(nbClientAffecte < sol->pb->n && nbServiceOuvert < sol->pb->m) { // tant que tous les clients ne sont pas affectés

        // sélection du service à ouvrir
        double utiliteMin = 0;
        int serviceMin = 0;

        for(int i = 0; i < sol->pb->m; i++) {

            if(!sol->services[i]) {

                // calcul de l'utilité pour chaque service
                double utilite = sol->pb->couts[i];

                double capaRestante = sol->capaRestantes[i];
                int indClient = 0;
                int nbClientAjout = 0;

                // prise en compte des clients que le pourrait choisir
                while(indClient < sol->pb->n) {
                    if(!affectes[clientsTries[i][indClient]] && sol->pb->demandes[clientsTries[i][indClient]] <= capaRestante) {
                        nbClientAjout ++;
                        capaRestante -= sol->pb->demandes[clientsTries[i][indClient]];
                        utilite += deltas[i][clientsTries[i][indClient]];
                    }
                    indClient ++;
                }


                utilite /= (double)nbClientAjout;
                printf("utilite : %f\n", utilite);

                if(i == 0 || utilite < utiliteMin) {
                    utiliteMin = utilite;
                    serviceMin = i;
                }
            }
        }
        printf("meilleur phi : %f\n\n\n", utiliteMin);

        // retrait du meilleur service trouvé selon l'utilité et connexions des clients
        nbServiceOuvert ++;
        sol->services[serviceMin] = 1; // le service ne sera plus ouvert
        sol->z += sol->pb->couts[serviceMin]; // le cout de l'ouverture du service est pris en compte

        // affectation des clients
        int indClient = 0;
        while(indClient < sol->pb->n) {

            // si les conditions sont vérifiées, le client est affecté
            if(!affectes[clientsTries[serviceMin][indClient]] && sol->pb->demandes[clientsTries[serviceMin][indClient]] <= sol->capaRestantes[serviceMin]) {
                sol->connexions[serviceMin][clientsTries[serviceMin][indClient]] = 1.;
                sol->z += sol->pb->liaisons[serviceMin][clientsTries[serviceMin][indClient]]; // prise en compte du cout de connexion
                affectes[clientsTries[serviceMin][indClient]] = 1;
                sol->capaRestantes[serviceMin] -= sol->pb->demandes[clientsTries[serviceMin][indClient]];
                nbClientAffecte ++;
            }
            indClient ++;
        }
    }

    printf("nbClientAffecte : %d\n", nbClientAffecte);
    printf("nbServiceOuvert : %d\n", nbServiceOuvert);

    free(cmin);
    for(int i = 0; i < sol->pb->m;i++) {
        free(deltas[i]);
    }
    free(deltas);
    free(affectes);
    for(int i = 0; i < sol->pb->m; i++) {
        free(clientsTries[i]);
    }
    free(clientsTries);
}

//------------------------------------------------------------------------------
void relaxationContinue(Solution* sol) {


}
