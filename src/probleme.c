/**
 * \file probleme.c
 * \brief implémentation des fonction de manipulation d'instances
 */

#include "probleme.h"
#include "stdio.h"
#include "stdlib.h"

//------------------------------------------------------------------------------
void chargerProbleme(Probleme* pb, char* nom) {

    FILE* fichier = fopen(nom, "r");

    if(fichier) {

        // lecture du nombre de service et du nombre de clients
        int ret = fscanf(fichier, "%d", &pb->m);
        ret = fscanf(fichier, "%d", &pb->n);

        // allocation de la mémoire
        pb->couts = malloc((long unsigned int)pb->m*sizeof(double));
        pb->capacites = malloc((long unsigned int)pb->m*sizeof(double));

        pb->demandes = malloc((long unsigned int)pb->n*sizeof(double));

        pb->liaisons = malloc((long unsigned int)pb->m*sizeof(double));
        for(int i = 0; i < pb->m; i++) {
            pb->liaisons[i] = malloc((long unsigned int)pb->n*sizeof(double));
        }

        // lecture des capacités et des coûts d'ouverture des services
        for(int i = 0; i < pb->m; i++) {
            ret = fscanf(fichier, "%lf", &pb->capacites[i]);
            ret = fscanf(fichier, "%lf", &pb->couts[i]);
        }

        // lecture des demandes des clients
        for(int i = 0; i < pb->n; i++) {
            ret = fscanf(fichier, "%lf", &pb->demandes[i]);
        }

        // lecture du cout des liaisons
        for(int i = 0; i < pb->m; i++) {
            for(int j = 0; j < pb->n; j++) {
                ret = fscanf(fichier, "%lf", &pb->liaisons[i][j]);
            }
        }

        fclose(fichier);
    } else {
        printf("Erreur, impossible de charger l'instance.");
    }

}

//------------------------------------------------------------------------------
void afficherProbleme(Probleme* pb) {

    printf("Nombre de services : %d\n", pb->m);
    printf("Nombre de clients : %d\n", pb->n);

    printf("capacités et couts :\n");
    for(int i = 0; i < pb->m; i++) {
        printf("%lf \t %lf\n", pb->capacites[i], pb->couts[i]);
    }

    printf("\n\ndemandes des clients : \n");
    for(int i = 0; i < pb->n; i++) {
        printf("%lf\n", pb->demandes[i]);
    }

    printf("\n\ncouts des liaisons\n");
    for(int i = 0; i < pb->m; i++) {
        for(int j = 0; j < pb->n; j++) {
            printf("%lf \n", pb->liaisons[i][j]);
        }
        printf("\n");
    }

}

//------------------------------------------------------------------------------
void detruireProblem(Probleme* pb) {

    free(pb->couts);
    free(pb->capacites);
    free(pb->demandes);
    for(int i = 0; i < pb->m; i++) {
        free(pb->liaisons[i]);
    }
    free(pb->liaisons);

}
