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

    // tableau des clients affectés
    int* affectes = malloc((long unsigned int)sol->pb->n*sizeof(int));
    for(int i = 0; i < sol->pb->n;i++) {
        affectes[i] = 0;
    }

    // tableau des clients par service triés en fonction des couts
    int** clientsTries = malloc((long unsigned int)sol->pb->m*sizeof(int*));
    for(int j = 0; j < sol->pb->n; j++) {
        clientTries = malloc((long unsigned int)sol->pb->n*sizeof(int));
    }

    int nbClientAffecte = 0;

    int continuer = 1;
    while(nbClientAffecte < sol->pb->n) { // tant que tous les clients ne sont pas affectés

        // sélection du service à ouvrir
        double utiliteMin = 0;
        int serviceMin = 0;
        for(int i = 0; i < sol->pb->m; i++) {
            if(!sol->services[i]) {
                // calcul de l'utilité pour chaque service
                int nbClientPot = 0;
                double utilite = sol->pb->couts[i];
                for(int j = 0; j < sol->pb->n; j++) {
                    if(!affectes[j] && sol->pb->demandes[j] < sol->capaRestantes[i]) {
                        utilite += sol->pb->liaisons[i][j] - cmin[j];
                        nbClientPot ++;
                    }
                }
                utilite /= (double)nbClientPot;
                if(i == 0 || utilite < utiliteMin) {
                    utiliteMin = utilite;
                    serviceMin = i;
                }
            }
        }

        // retrait du meilleur service trouvé selon l'utilité et connexions des clients
        sol->services[serviceMin] = 1; // le service ne sera plus ouvert
        sol->z += sol->pb->couts[serviceMin]; // le cout de l'ouverture du service est pris en compte

        // tri des clients par ordre croissants des deltas
        int* clients = malloc((long unsigned int)sol->pb->n*sizeof(int));
        for(int i = 0; i < sol->pb->n; i++) {
            clients[i] = i;
        }
        trierCroissant(sol->pb->n, clients, sol->pb->liaisons[serviceMin]);

        // affectation des clients
        int indClient = 0;
        while(indClient < sol->pb->n) {
            if(sol->pb->demandes[clients[indClient]] <= sol->capaRestantes[serviceMin]) {
                sol->connexions[serviceMin][clients[indClient]] = 1.;
                sol->z += sol->pb->liaisons[serviceMin][clients[indClient]]; // prise en compte du cout de connexion
                affectes[clients[indClient]] = 1;
                sol->capaRestantes[serviceMin] -= sol->pb->demandes[clients[indClient]];
                nbClientAffecte ++;
            }
            indClient ++;
        }
        free(clients);

    }

    free(cmin);
    free(affectes);

    for(int i = 0; i < sol->pb->n; i++) {
        free(sol->)
    }
}
