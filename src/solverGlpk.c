/**
 * \file solverGlpk.c
 * \brief utilisation de glpk pour résoudre le problème de SSCFLP
 */

#include <stdio.h>
#include <stdlib.h>

#include <glpk.h>

#include "probleme.h"

/**
 * \brief résolution de l'instance à l'aide de glpk
 * \param pb l'instance du problème à résoudre
 */
void resoudre(Probleme* pb);

//------------------------------------------------------------------------------
int main(int arcg, char* argv[]) {

    printf("Solver avec glpk\n");

    Probleme pb;

    chargerProbleme(&pb, "Instances/Beasley/cap64.dat");

    // afficherProbleme(&pb);

    detruireProblem(&pb);

    return 0;
}

//------------------------------------------------------------------------------
void resoudre(Probleme* pb) {

    glp_prob* prob;
    prob = glp_create_prob();
    glp_set_prob_name(prob, "SSCFLP");
    glp_set_obj_dir(prob, GLP_MIN);

    int nbVar = (pb->n+1)*pb->m;
    int nbCont = pb->m + pb->n;

    // déclaration des contraintes et des bornes sur celles-ci
    glp_add_rows(prob, nbCont);
    for(int i = 1; i <= nbVar; i++) {

    }
    for(int i = nbVar+1; i <= nbCont; i++) {

    }

}
