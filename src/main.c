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

    printf("Hello world\n");

    Probleme pb;

    // chargerProbleme(&pb, "Instances/Beasley/cap64.dat");
    chargerProbleme(&pb, "Instances/jouet.dat");

    afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);

    construction(&sol);

    detruireSolution(&sol);

    detruireProblem(&pb);

    return 0;
}
