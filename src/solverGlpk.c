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

    // chargerProbleme(&pb, "Instances/Areizaga/p28.dat");
    // chargerProbleme(&pb, "Instances/Beasley/cap103.dat");
    chargerProbleme(&pb, "Instances/Holmberg/p14");
    // chargerProbleme(&pb, "Instances/Yang/60-300-1.dat");
    // chargerProbleme(&pb, "Instances/jouet.dat");

    // afficherProbleme(&pb);

    resoudre(&pb);

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

    int nbCreux = pb->m*(pb->n+1) + pb->n*pb->m;
    int* ia = malloc((long unsigned int)(nbCreux+1)*sizeof(int));
    int* ja = malloc((long unsigned int)(nbCreux+1)*sizeof(int));
    double* ar = malloc((long unsigned int)(nbCreux+1)*sizeof(double));

    // déclaration des contraintes et des bornes sur celles-ci
    glp_add_rows(prob, nbCont);
    for(int i = 1; i <= pb->m; i++) {
        glp_set_row_bnds(prob, i, GLP_UP, 0.0, 0.0);
    }
    for(int i = 1; i <= pb->n; i++) {
        glp_set_row_bnds(prob, pb->m+i, GLP_FX, 1.0, 1.0);
    }

    // variables du problèmes, toutes binaires
    glp_add_cols(prob, nbVar);
    for(int i = 1; i <= nbVar; i++) {
        glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0);
        glp_set_col_kind(prob, i, GLP_BV);
    }

    // coefficients dans la fonction objectif
    for(int i = 0; i < pb->m; i++) {
        for(int j = 0; j < pb->n; j++) {
            glp_set_obj_coef(prob, i*pb->n+j+1, pb->liaisons[i][j]);
        }
        glp_set_obj_coef(prob, pb->m*pb->n+i+1, pb->couts[i]);
    }

    // initialisation de la matrice creuse des contraintes
    int indice = 1;

    // pour les m services, la capacité n'est pas dépassée
    for(int i = 0; i < pb->m; i++) {
        ia[indice] = i+1;
        ja[indice] = pb->n*pb->m+i+1; // variable y_i
        ar[indice] = -pb->capacites[i];
        indice ++;
        for(int j = 0; j < pb->n; j++) {
            ia[indice] = i+1;
            ja[indice] = i*pb->n+j+1; // variable x_{ij}
            ar[indice] = pb->demandes[j];
            indice ++;
        }
    }

    // chaque client est connecté à un unique service
    for(int i = 0; i < pb->n; i++) {
        for(int j = 0; j < pb->m; j++) {
            ia[indice] = pb->m+i+1;
            ja[indice] = j*pb->n+i+1; // variable x_{ji}
            ar[indice] = 1.0;
            indice ++;
        }
    }

    glp_load_matrix(prob, nbCreux, ia, ja, ar);

    // glp_write_lp(prob,NULL,"modele.lp");

    glp_simplex(prob, NULL);
    glp_intopt(prob, NULL);

    printf("Valeur optimale : %f\n", glp_mip_obj_val(prob));

    free(ia);
    free(ja);
    free(ar);

}
