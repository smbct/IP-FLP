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

    sol->connexions = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < sol->pb->m; i++) {
        sol->connexions[i] = malloc((long unsigned int)pb->n*sizeof(double));
        for(int j = 0; j < pb->n; j++) {
            sol->connexions[i][j] = 0.;
        }
    }

    sol->services = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < pb->m; i++) {
        sol->services[i] = 0;
    }

    sol->capaRestantes = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < pb->m;i++) {
        sol->capaRestantes[i] = pb->capacites[i];
    }

    sol->z = 0.;

}

//------------------------------------------------------------------------------
void afficherSolution(Solution* sol) {

    printf("connexions :\n");
    for(int i = 0; i < sol->pb->m; i++) {
        for(int j = 0; j < sol->pb->n; j++) {
            printf("%lf, ", sol->connexions[i][j]);
        }
        printf("\n");
    }
    printf("\nservices ouverts :\n");
    for(int i = 0; i < sol->pb->m; i++) {
        printf("%d, ", sol->services[i]);
    }
    printf("\n");
    printf("z = %lf\n", sol->z);

}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {

    free(sol->capaRestantes);

    for(int i = 0; i < sol->pb->m; i++) {
        free(sol->connexions[i]);
    }
    free(sol->connexions);
    free(sol->services);
}
