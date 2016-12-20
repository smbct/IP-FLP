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

    /*clock_t begin, end;
    double temps;*/

    if(argc > 1) {
        Probleme pb;

        chargerProbleme(&pb, argv[1]);

        Solution sol;

        creerSolution(&pb, &sol);

        // construireACO(&sol);
        rechercheTabu(&sol);

        // printf("Lancement du branch & bound\n");
        // branchBoundIter(&sol);

        // printf("Solution optimale : \n");

        // construction(&sol);

        // afficherSolution(&sol);
        // printf("%lf\n", sol.z);

        detruireSolution(&sol);

        detruireProbleme(&pb);
    }


    return 0;
}
