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

void resoudre(Solution* sol);

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    if(argc > 1) {
        Probleme pb;

        chargerProbleme(&pb, argv[1]);

        Solution sol;

        creerSolution(&pb, &sol);

        resoudre(&sol);

        detruireSolution(&sol);

        detruireProbleme(&pb);
    }


    return 0;
}

//------------------------------------------------------------------------------
void resoudre(Solution* sol) {

    printf("Lancement du branch & bound\n");

    branchBoundIter(sol);

    printf("Solution optimale : \n");

    afficherSolution(sol);

}
