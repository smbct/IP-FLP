/**
 * \file solverGlpk.c
 * \brief utilisation de glpk pour résoudre le problème de SSCFLP
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glpk.h>

#include "probleme.h"
#include "solution.h"

/**
 * \brief résolution de l'instance à l'aide de glpk
 * \param pb l'instance du problème à résoudre
 */
void resoudre(char* instance, Solution* sol);

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    // printf("Solver avec glpk\n");

    Solution sol;

    if(argc > 1) {

        printf("%s", argv[1]);
        clock_t begin, end;
        begin = clock();
        resoudre(argv[1], &sol);
        end = clock();
        double temps = (double)(end - begin) / CLOCKS_PER_SEC;
        temps *= 1000;

        printf(" & %lf\\\\ \n", temps);

    } else {
        printf("indiquez une instance en entrée.\n");
    }

    return 0;
}

//------------------------------------------------------------------------------
void resoudre(char* instance, Solution* sol) {

    Probleme pb;
    chargerProbleme(&pb, instance);

    creerSolution(&pb, sol);

    printf(" $ %d * %d", pb.m, pb.n);

    //désactivation du log de glpk
    glp_term_out(0);

    glp_prob* prob;
    prob = glp_create_prob();
    glp_set_prob_name(prob, "SSCFLP");
    glp_set_obj_dir(prob, GLP_MIN);

    // timer glpk
    glp_iocp param;
    glp_init_iocp(&param);
    param.tm_lim = 1000*60*2; // valeur en ms
    // param.tm_lim = 5000; // valeur en ms

    int nbVar = (pb.n+1)*pb.m;
    int nbCont = pb.m + pb.n;

    int nbCreux = pb.m*(pb.n+1) + pb.n*pb.m;
    int* ia = malloc((long unsigned int)(nbCreux+1)*sizeof(int));
    int* ja = malloc((long unsigned int)(nbCreux+1)*sizeof(int));
    double* ar = malloc((long unsigned int)(nbCreux+1)*sizeof(double));

    // déclaration des contraintes et des bornes sur celles-ci
    glp_add_rows(prob, nbCont);
    for(int i = 1; i <= pb.m; i++) {
        glp_set_row_bnds(prob, i, GLP_UP, 0.0, 0.0);
    }
    for(int i = 1; i <= pb.n; i++) {
        glp_set_row_bnds(prob, pb.m+i, GLP_FX, 1.0, 1.0);
    }

    // variables du problèmes, toutes binaires
    glp_add_cols(prob, nbVar);
    for(int i = 1; i <= nbVar; i++) {
        glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0);
        glp_set_col_kind(prob, i, GLP_BV);
    }

    // coefficients dans la fonction objectif
    for(int i = 0; i < pb.m; i++) {
        for(int j = 0; j < pb.n; j++) {
            glp_set_obj_coef(prob, i*pb.n+j+1, pb.liaisons[i][j]);
        }
        glp_set_obj_coef(prob, pb.m*pb.n+i+1, pb.couts[i]);
    }

    // initialisation de la matrice creuse des contraintes
    int indice = 1;

    // pour les m services, la capacité n'est pas dépassée
    for(int i = 0; i < pb.m; i++) {
        ia[indice] = i+1;
        ja[indice] = pb.n*pb.m+i+1; // variable y_i
        ar[indice] = -pb.capacites[i];
        indice ++;
        for(int j = 0; j < pb.n; j++) {
            ia[indice] = i+1;
            ja[indice] = i*pb.n+j+1; // variable x_{ij}
            ar[indice] = pb.demandes[j];
            indice ++;
        }
    }

    // chaque client est connecté à un unique service
    for(int i = 0; i < pb.n; i++) {
        for(int j = 0; j < pb.m; j++) {
            ia[indice] = pb.m+i+1;
            ja[indice] = j*pb.n+i+1; // variable x_{ji}
            ar[indice] = 1.0;
            indice ++;
        }
    }

    glp_load_matrix(prob, nbCreux, ia, ja, ar);

    glp_write_lp(prob,NULL,"modele.lp");

    glp_simplex(prob, NULL);
    int res = glp_intopt(prob, &param);
    sol->z = glp_mip_obj_val(prob);

    // récupération des valeurs des variables

    // lecture des connexions
    for(int i = 0; i < pb.m; i++) {
        for(int j = 0; j < pb.n; j++) {
            sol->connexions[i][j] = (int)(glp_mip_col_val(prob, i*pb.n+j+1)+0.5);
        }
    }

    // lecture des services ouverts
    for(int i = 0; i < pb.m; i++) {
        sol->services[i] = (int)(glp_mip_col_val(prob, pb.m*pb.n+1+i)+0.5);
    }

    printf(" $ %f", sol->z);

    if(res == GLP_ETMLIM) {
        printf("*");
    }

    afficherSolution(sol);
    detruireSolution(sol);

    glp_delete_prob(prob);

    free(ia);
    free(ja);
    free(ar);

    detruireProblem(&pb);

}
