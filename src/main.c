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
int main(int argc, char* argv[]) {

    Probleme pb;

    // chargerProbleme(&pb, "Instances/Areizaga/p1.dat");
    // chargerProbleme(&pb, "Instances/Yang/60-300-1.dat");
    chargerProbleme(&pb, "Instances/Holmberg/p22.dat");
    // chargerProbleme(&pb, "Instances/jouet.dat");

    // afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);

    construction(&sol);
    // relaxationContinue(&sol);
    // printf("relaxation continue : %lf\n", sol.z);

    // afficherSolution(&sol);
    printf("z = %lf\n", sol.z);

    detruireSolution(&sol);

    detruireProblem(&pb);

    return 0;
}
