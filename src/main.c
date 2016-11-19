/**
 * \file main.c
 * \brief point d'entrée du programme
 */

#include <stdio.h>
#include <stdlib.h>

#include "probleme.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    printf("Hello world\n");

    Probleme pb;

    chargerProbleme(&pb, "Instances/Beasley/cap64.dat");

    afficherProbleme(&pb);

    detruireProblem(&pb);

    return 0;
}
