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

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    /*clock_t begin, end;
    double temps;*/

    if(argc > 1) {
        Probleme pb;

        chargerProbleme(&pb, argv[1]);

        Solution sol;

        creerSolution(&pb, &sol);
        construction(&sol);

        afficherSolution(&sol);

        // printf("Lancement du branch & bound\n");
        // branchBound(&sol);

        detruireSolution(&sol);

        detruireProbleme(&pb);
    }


    return 0;
}
