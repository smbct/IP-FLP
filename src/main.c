/**
 * \file main.c
 * \brief point d'entrée du programme
 */

#include <stdio.h>
#include <stdlib.h>

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

    if(argc > 1) {
        Probleme pb;

        chargerProbleme(&pb, argv[1]);

        /*printf("%s & ", argv[1]);
        for(int i = 0; i < 4; i++) {
            calculerBorne(&pb, i);
            if(i < 3) {
                printf(" & ");
            } else {
                printf("\\\\ \n");
            }
        }*/

        Solution sol;
        creerSolution(&pb, &sol);
        relaxationContinue(&sol);
        // construction(&sol);
        printf("%lf\n", sol.z);
        detruireProbleme(&pb);
    }






    return 0;
}
