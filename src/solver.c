/**
 * \file solver.c
 * \brief fonctions de résolution du SSCFLP basées sur un branch & bound
 */

#include <stdlib.h>
#include <stdio.h>

#include "solver.h"
#include "bornes.h"

//------------------------------------------------------------------------------
void branchBound(Solution* sol) {

    Solution duale, best;
    creerSolution(sol->pb, &duale);
    creerSolution(sol->pb, &best);

    construction(&best);
    printf("Valeur initiale : %lf\n", best.z);

    printf("\n\nLancement du B&B \n\n");

    branchBoundRec(sol, &duale, &best);

    copierSolution(&best, sol);

    detruireSolution(&duale);
    detruireSolution(&best);

}

//------------------------------------------------------------------------------
void branchBoundRec(Solution* sol, Solution* duale, Solution* best) {

    printf("\tDEBUG\n");
    printf("services affectés : \n");
    for(int i = 0; i < sol->pb->m; i++) {
        printf("%d, ", sol->services[i]);
    }

    printf("\nclients affectées : \n");
    for(int i = 0; i < sol->pb->n; i++) {
        printf("%d, ", sol->connexionClient[i]);
    }
    printf("\n");

    // relaxation continue
    int resRelax = relaxationContinue(sol, duale);

    printf("relaxation : %lf\n", duale->z);
    printf("res relax continue : %d\n", resRelax);

    printf("nb services fixés : %d\n", sol->nbVarServicesFixees);
    printf("nb clients fixés : %d\n", sol->nbVarClientFixees);

    printf("meilleure solution connue : %lf\n", best->z);

    printf("\n");
    printf("\tFIN DEBUG\n\n");

    printf("\n\n\n");

    // si la relaxation ne permet pas de couper, le branchement est obligatoire
    if(resRelax == 0 && duale->z < best->z) {

        // branchement d'abord sur les services
        if(sol->nbVarServicesFixees < sol->pb->m) { // fixation de l'ouverture d'un service

            sol->nbVarServicesFixees ++;

            // fixation à 0
            sol->services[sol->nbVarServicesFixees-1] = 0;
            branchBoundRec(sol, duale, best);

            // fixation à 1
            sol->services[sol->nbVarServicesFixees-1] = 1;
            branchBoundRec(sol, duale, best);

            // ensuite l'affectation de la variable est annulée
            sol->nbVarServicesFixees --;
            sol->services[sol->nbVarServicesFixees] = -1;

        } else if(sol->nbVarClientFixees < sol->pb->n) { // fixation d'un client

            // affectation du prochain client
            // TODO : ordre d'affectation des clients
            int client = sol->nbVarClientFixees;

            // branchement sur cette variable
            sol->nbVarClientFixees ++;

            printf("affectation client %d\n", client);

            // on essaie de connecter le client à chacun des services, quand c'est possible

            for(int i = 0; i < sol->pb->n; i++) {
                // on peut vérifier facilement si la solution reste admissible en terme de respet des capacités
                // if(sol->capaRestantes[i] <= sol->pb->demandes[j]) {
                    printf("test\n");
                    sol->capaRestantes[i] -= sol->pb->demandes[client];
                    sol->connexionClient[client] = i;
                    branchBoundRec(sol, duale, best);
                    sol->capaRestantes[i] += sol->pb->demandes[client];
                // }
            }

            // ensuite la variable redevient libre
            sol->nbVarClientFixees --;
            sol->connexionClient[client] = -1;

        }

    } else if(resRelax == 1) {
        printf("La solution obtenue est entière, pas besoin de brancher\n");
        // solution entière, pas besoin brancher et en plus une solution entière de plus
        if(duale->z < best->z) { // meilleure solution mise à jour
            copierSolution(duale, best);
        }
        printf("meilleure solution : %lf\n", duale->z);
    } else {

        printf("branche coupée :\n");

        if(resRelax == -1) {
            printf("Le sous problème actuel n'admet aucune solution\n");
        } else {
            printf("La relaxation continue n'est pas mieux que la meilleure solution connue\n");
        }
        printf("\n\n\n");
    }

}

//------------------------------------------------------------------------------
void branchBoundIter(Solution* sol) {

    Solution duale, best;
    creerSolution(sol->pb, &duale);
    creerSolution(sol->pb, &best);

    construction(&best);
    printf("Valeur initiale : %lf\n", best.z);

    printf("\n\nLancement du B&B \n\n");



    detruireSolution(&duale);
    detruireSolution(&best);

}
