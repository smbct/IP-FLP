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

//------------------------------------------------------------------------------
void calculerBorne(Probleme* pb, int option) {

    Solution sol;
    creerSolution(pb, &sol);

    if(option == 0) {
        relaxationContinue(&sol);
    } else if(option == 1) {
        relaxationUFLP(&sol);
    } else if(option == 2) {
        relaxationCFLP(&sol);
    } else {
        construction(&sol);
    }

    printf("%lf", sol.z);

    detruireSolution(&sol);
}

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    clock_t begin, end;
    double temps;

    if(argc > 1) {
        Probleme pb;

        chargerProbleme(&pb, argv[1]);

        Solution sol;

        creerSolution(&pb, &sol);

        relaxationUFLP(&sol);
        printf("z = %lf\n", sol.z);

        detruireSolution(&sol);

        detruireProbleme(&pb);
    }


    return 0;
}
