/**
 * \file solver.c
 * \brief fonctions de résolution du SSCFLP basées sur un branch & bound
 */

#include "solver.h"
#include "bornes.h"

//------------------------------------------------------------------------------
void branchBound(Solution* sol) {

    Solution duale, best;
    creerSolution(sol->pb, &duale);
    creerSolution(sol->pb, &best);

    construction(&best);
    printf("Valeur initiale : %lf\n", best.z);

    branchBoundRec(sol, &duale, &best);

    printf("Valeur optimale : z = %lf\n", best.z);

    detruireSolution(&duale);
    detruireSolution(&best);

}

//------------------------------------------------------------------------------
void branchBoundRec(Solution* sol, Solution* duale, Solution* best) {

    printf("bb rec\n");

    printf("\tDEBUG\n");
    printf("services affectés : \n");
    for(int i = 0; i < sol->pb->m; i++) {
        printf("%d, ", sol->varServicesAffectees[i]);

        if(sol->varServicesAffectees[i]) {
            printf(" (%d), ", sol->services[i]);
        }
    }

    printf("\n\nconnexions affectées : \n");
    for(int i = 0; i < sol->pb->m; i++) {
        for(int j = 0; j < sol->pb->n; j++) {
            printf("%d, ", sol->varConnexionsAffectees[i][j]);
        }
        printf("\n");
    }
    printf("\tFIN DEBUG\n\n");

    // relaxation continue
    int resRelax = relaxationContinue(sol, duale);

    printf("relaxation : %lf\n", duale->z);
    printf("res relax continue : %d\n", resRelax);

    printf("nbConn fixées : %d\n", sol->nbVarConnFixees);

    // si la relaxation ne permet pas de couper, le branchement est obligatoire
    if(resRelax == 0 && duale->z < best->z) {

        // branchement d'abord sur les services

        if(sol->nbVarServicesFixees < sol->pb->n) { // fixation de l'ouverture d'un service

            sol->varServicesAffectees[sol->nbVarServicesFixees] = 1;
            sol->nbVarServicesFixees ++;

            // fixation à 0
            sol->services[sol->nbVarServicesFixees-1] = 0;
            branchBoundRec(sol, duale, best);

            // fixation à 1
            sol->services[sol->nbVarServicesFixees-1] = 1;
            branchBoundRec(sol, duale, best);

        } else if(sol->nbVarConnFixees < sol->pb->n*sol->pb->m) { // fixation d'une connexion

            // recherche d'une variable de connexion non encore affectée
            int i = 0, j = 0, trouve = 0;

            while(!trouve && i < sol->pb->m) {
                j = 0;
                while(!trouve && j < sol->pb->n) {
                    if(!sol->varConnexionsAffectees[i][j]) {
                        trouve = 1;
                    } else {
                        j ++;
                    }
                }
                if(!trouve) {
                    i ++;
                }
            }

            // sauvegarde des variables qui étaient déjà affectées
            int* dejaAffecte = malloc((long unsigned int)sol->pb->m*sizeof(int));
            for(int l = 0; l < sol->pb->m; l ++) {
                dejaAffecte[l] = 0;
            }

            // branchement sur cette variable
            sol->nbVarConnFixees ++;
            sol->varConnexionsAffectees[i][j] = 1;

            // fixée à 0
            sol->connexions[i][j] = 0;
            branchBoundRec(sol, duale, best);

            // fixée à 1
            sol->connexions[i][j] = 1;
            // le client n'est connecté à personne d'autre
            for(int k = 0; k < sol->pb->m; k++) {
                if(k != i) {
                    if(!sol->varConnexionsAffectees[k][j]) {
                        sol->varConnexionsAffectees[k][j] = 1;
                        sol->nbVarConnFixees ++;
                    } else {
                        dejaAffecte[k] = 1;
                    }
                    sol->connexions[k][j] = 0;
                }
            }
            branchBoundRec(sol, duale, best);

            // ensuite les autres variables sont désaffectées

            for(int k = 0; k < sol->pb->m; k++) {
                if(k != i) {
                    if(!dejaAffecte[k]) {
                        sol->varConnexionsAffectees[k][j] = 0;
                        sol->nbVarConnFixees --;
                    }
                }
            }

            free(dejaAffecte);
        }

        // branchement ensuite sur les cennexions

    } else if(resRelax == 1) {
        // solution entière, pas besoin brancher et en plus une solution entière de plus
        if(duale->z < best->z) { // meilleure solution mise à jour
            copierSolution(duale, best);
        }
        printf("meilleure solution : %lf\n", duale->z);
    }

}
