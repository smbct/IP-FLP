/**
 * \file main.c
 * \brief point d'entrée du programme
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "probleme.h"
#include "solution.h"
#include "bornes.h"
#include "solver.h"

#include "aco.h"
#include "tabu.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    clock_t begin, end;
    double temps;

    if(argc > 1) {
        Probleme pb;

        chargerProbleme(&pb, argv[1]);

        Solution sol;

        creerSolution(&pb, &sol);

        printf("Lancement du branch & bound\n");

        begin = clock();
        branchBoundIter(&sol);
        end = clock();

        printf("Solution optimale : \n");

        // construction(&sol);

        afficherSolution(&sol);
        // printf("%lf\n", sol.z);

        temps = (double)(end-begin) / CLOCKS_PER_SEC;
        printf("La résolution s'est terminée en %lf secondes\n", temps);

        detruireSolution(&sol);

        detruireProbleme(&pb);
    }


    return 0;
}
