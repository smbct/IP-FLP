/**
 * \file solution.c
 * \brief implémentation des méthodes de la structure de solution
 */

#include "solution.h"
#include "stdio.h"
#include "stdlib.h"

//------------------------------------------------------------------------------
void creerSolution(Probleme* pb, Solution* sol) {

    sol->pb = pb;
    sol->nbVarFixees = 0;

    int nbVar = pb->m*(pb->n+1);
    sol->valeurs = malloc((long unsigned int)nbVar*sizeof(double));
    for(int i = 0; i < nbVar; i++) {
        sol->valeurs[i] = 0.;
    }

}

//------------------------------------------------------------------------------
void afficherSolution(Solution* sol) {

    for(int i = 0; i < sol->pb->m*(sol->pb->n+1); i++) {
        printf("%lf, ", sol->valeurs[i]);
    }
    printf("\n");

}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {

    free(sol->valeurs);

}
