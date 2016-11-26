/**
 * \file brones.c
 * \brief implémentations de fonctions de calcul de bornes pour le SSCFLP
 */

#include <stdio.h>
#include <stdlib.h>

#include <glpk.h>

#include "bornes.h"
#include "tri.h"


//------------------------------------------------------------------------------
void construction(Solution* sol) {

    sol->z = 0;

    double* cmin = malloc((long unsigned int)sol->pb->n*sizeof(double));

    // calculs des cmin_i, les couts d'affectation minimaux par clients
    for(int i = 0; i < sol->pb->n; i++) {
        for(int j = 0; j < sol->pb->m; j++) {
            if(j == 0 || sol->pb->liaisons[j][i] < cmin[i]) {
                cmin[i] = sol->pb->liaisons[j][i];
            }
        }
    }

    // delta_{ij} = c_{ij} - cmin_i
    double** deltas = malloc((long unsigned int)sol->pb->m*sizeof(double*));
    for(int i = 0; i < sol->pb->m; i++) {
        deltas[i] = malloc((long unsigned int)sol->pb->n*sizeof(double));
        for(int j = 0; j < sol->pb->n; j++) {
            deltas[i][j] = sol->pb->liaisons[i][j] - cmin[j];
        }
    }

    // DEBUG affichage des deltas
    /*printf("affichage des deltas :\n");
    for(int i = 0; i < sol->pb->n; i++) {
        printf("%d ; cmin = %lf : ", i, cmin[i]);
        for(int j = 0; j < sol->pb->m; j++) {
            printf("%lf, ", sol->pb->liaisons[j][i] - cmin[i]);
        }
        printf("\n\n");
    }*/

    // tableau des clients affectés
    int* affectes = malloc((long unsigned int)sol->pb->n*sizeof(int));
    for(int i = 0; i < sol->pb->n;i++) {
        affectes[i] = 0;
    }

    // tableau des clients par service triés en fonction des deltas
    int** clientsTries = malloc((long unsigned int)sol->pb->m*sizeof(int*));
    for(int i = 0; i < sol->pb->m; i++) {
        clientsTries[i] = malloc((long unsigned int)sol->pb->n*sizeof(int));
        for(int j = 0; j < sol->pb->n; j++) {
            clientsTries[i][j] = j;
        }
        // tri des clients pour chaque service en fonction des cmin
        trierCroissant(sol->pb->n, clientsTries[i], deltas[i], sol->pb->demandes);
    }

    // DEBUG affichage du tri
    /*printf("résultat du tri :\n");
    for(int i = 0; i < sol->pb->m; i++) {

        for(int j = 0; j < sol->pb->n; j++) {

            printf("( %lf ; %lf ), ", deltas[i][clientsTries[i][j]], sol->pb->demandes[clientsTries[i][j]]);

        }
        printf("\n");

    }*/

    int nbClientAffecte = 0;
    int nbServiceOuvert = 0;

    int continuer = 1;
    while(nbClientAffecte < sol->pb->n && nbServiceOuvert < sol->pb->m) { // tant que tous les clients ne sont pas affectés

        // sélection du service à ouvrir
        double utiliteMin = 0;
        int serviceMin = -1;

        // calcul de l'utilité min sur tous les services non affectés
        for(int i = 0; i < sol->pb->m; i++) {

            if(!sol->services[i]) {

                // calcul de l'utilité pour chaque service
                double utilite = sol->pb->couts[i];

                double capaRestante = sol->capaRestantes[i];
                int indClient = 0;
                int nbClientAjout = 0;

                // prise en compte des clients que l'on peut affecter
                while(indClient < sol->pb->n) {
                    if(!affectes[clientsTries[i][indClient]] && sol->pb->demandes[clientsTries[i][indClient]] <= capaRestante) {
                        nbClientAjout ++;
                        capaRestante -= sol->pb->demandes[clientsTries[i][indClient]];
                        utilite += deltas[i][clientsTries[i][indClient]];
                    }
                    indClient ++;
                }

                utilite /= (double)nbClientAjout;

                if(serviceMin == -1 || utilite <= utiliteMin) {
                    utiliteMin = utilite;
                    serviceMin = i;
                }
            }
        }

        // retrait du meilleur service trouvé selon l'utilité et connexions des clients
        nbServiceOuvert ++;
        sol->services[serviceMin] = 1; // le service ne sera plus ouvert
        sol->z += sol->pb->couts[serviceMin]; // le cout de l'ouverture du service est pris en compte

        // affectation des clients
        int indClient = 0;
        while(indClient < sol->pb->n) {

            // si les conditions sont vérifiées, le client est affecté
            if(!affectes[clientsTries[serviceMin][indClient]] && sol->pb->demandes[clientsTries[serviceMin][indClient]] <= sol->capaRestantes[serviceMin]) {
                sol->connexions[serviceMin][clientsTries[serviceMin][indClient]] = 1.;
                sol->z += sol->pb->liaisons[serviceMin][clientsTries[serviceMin][indClient]]; // prise en compte du cout de connexion
                affectes[clientsTries[serviceMin][indClient]] = 1;
                sol->capaRestantes[serviceMin] -= sol->pb->demandes[clientsTries[serviceMin][indClient]];
                nbClientAffecte ++;
            }
            indClient ++;
        }
    }

    /*printf("nbClientAffecte : %d\n", nbClientAffecte);
    printf("nbServiceOuvert : %d\n", nbServiceOuvert);*/

    if(nbClientAffecte < sol->pb->n) {
        printf("Problème impossible\n");
    }

    free(cmin);
    for(int i = 0; i < sol->pb->m;i++) {
        free(deltas[i]);
    }
    free(deltas);
    free(affectes);
    for(int i = 0; i < sol->pb->m; i++) {
        free(clientsTries[i]);
    }
    free(clientsTries);
}

//------------------------------------------------------------------------------
void relaxationContinue(Solution* sol) {

    Probleme* pb = sol->pb;

    //désactivation du log de glpk
    glp_term_out(0);

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
        glp_set_col_kind(prob, i, GLP_CV);
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

    glp_simplex(prob, NULL);

    sol->z = glp_get_obj_val(prob);
    // récupération de la solution ?

    glp_delete_prob(prob);

    free(ia);
    free(ja);
    free(ar);

}

//------------------------------------------------------------------------------
void relaxationCFLP(Solution* sol) {

    Probleme* pb = sol->pb;

    //désactivation du log de glpk
    glp_term_out(0);

    glp_prob* prob;
    prob = glp_create_prob();
    glp_set_prob_name(prob, "Relaxation CFLP");
    glp_set_obj_dir(prob, GLP_MIN);

    // timer glpk
    glp_iocp param;
    glp_init_iocp(&param);
    param.tm_lim = 1000*60; // valeur en ms

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
        glp_set_row_bnds(prob, pb->m+i, GLP_LO, 1.0, 1.0);
    }

    // variables du problèmes, toutes binaires
    glp_add_cols(prob, nbVar);
    for(int i = 1; i <= nbVar; i++) {
        glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0);
        if(i <= pb->n*pb->m) {
            glp_set_col_kind(prob, i, GLP_CV);
        } else {
            glp_set_col_kind(prob, i, GLP_BV);
        }
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

    // glp_write_lp(prob,NULL,"relax.lp");

    glp_simplex(prob, NULL);
    int res = glp_intopt(prob, &param);

    if(res == GLP_ETMLIM) {
        printf("*");
    }

    sol->z = glp_mip_obj_val(prob);
    // récupération de la solution ?

    glp_delete_prob(prob);

    free(ia);
    free(ja);
    free(ar);

}

//------------------------------------------------------------------------------
void relaxationUFLP(Solution* sol) {

    Probleme* pb = sol->pb;

    //désactivation du log de glpk
    glp_term_out(0);

    glp_prob* prob;
    prob = glp_create_prob();
    glp_set_prob_name(prob, "Relaxation UFLP");
    glp_set_obj_dir(prob, GLP_MIN);

    int nbVar = (pb->n+1)*pb->m;
    int nbCont = pb->m*pb->n + pb->n;

    int nbCreux = pb->m*pb->n*2 + pb->n*pb->m;
    int* ia = malloc((long unsigned int)(nbCreux+1)*sizeof(int));
    int* ja = malloc((long unsigned int)(nbCreux+1)*sizeof(int));
    double* ar = malloc((long unsigned int)(nbCreux+1)*sizeof(double));

    // déclaration des contraintes et des bornes sur celles-ci
    glp_add_rows(prob, nbCont);
    for(int i = 1; i <= pb->m*pb->n; i++) {
        glp_set_row_bnds(prob, i, GLP_UP, 0.0, 0.0);
    }
    for(int i = 1; i <= pb->n; i++) {
        glp_set_row_bnds(prob, pb->m*pb->n+i, GLP_LO, 1.0, 1.0);
    }

    // variables du problèmes, toutes binaires
    glp_add_cols(prob, nbVar);
    for(int i = 1; i <= nbVar; i++) {
        glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0);
        if(i <= pb->n*pb->m) {
            glp_set_col_kind(prob, i, GLP_CV);
        } else {
            glp_set_col_kind(prob, i, GLP_BV);
        }
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

    // x_{ij} = 1 => y_i = 1
    for(int i = 0; i < pb->m; i++) {
        for(int j = 0; j < pb->n; j++) {
            ia[indice] = i*pb->n+j+1;
            ja[indice] = i*pb->n+j+1; // variable x_{ij}
            ar[indice] = 1.0;
            indice ++;

            ia[indice] = i*pb->n+j+1;
            ja[indice] = pb->n*pb->m+i+1; // variable y_i
            ar[indice] = -1.0;
            indice ++;
        }
    }

    // chaque client est connecté à un unique service
    for(int i = 0; i < pb->n; i++) {
        for(int j = 0; j < pb->m; j++) {
            ia[indice] = pb->m*pb->n+i+1;
            ja[indice] = j*pb->n+i+1; // variable x_{ji}
            ar[indice] = 1.0;
            indice ++;
        }
    }

    glp_load_matrix(prob, nbCreux, ia, ja, ar);

    // glp_write_lp(prob,NULL,"relaxUFLP.lp");

    glp_simplex(prob, NULL);
    glp_intopt(prob, NULL);

    sol->z = glp_mip_obj_val(prob);
    // récupération de la solution ?

    glp_delete_prob(prob);

    free(ia);
    free(ja);
    free(ar);

}
