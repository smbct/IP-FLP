/**
 * \file solution.c
 * \brief implémentation des méthodes de la structure de solution
 */

#include "solution.h"
#include "stdio.h"
#include "stdlib.h"

//------------------------------------------------------------------------------
void creerSolution(Probleme* pb, Solution* sol) {

    sol->pb = pb;

    sol->connexions = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < sol->pb->m; i++) {
        sol->connexions[i] = malloc((long unsigned int)pb->n*sizeof(double));
        for(int j = 0; j < pb->n; j++) {
            sol->connexions[i][j] = 0.;
        }
    }

    sol->services = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < pb->m; i++) {
        sol->services[i] = 0;
    }

    sol->capaRestantes = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < pb->m;i++) {
        sol->capaRestantes[i] = pb->capacites[i];
    }

    sol->z = 0.;

    sol->nbServicesOuverts = 0;
    sol->nbClientsConnectes = 0;
    sol->clientsConnectes = malloc((long unsigned int)pb->n*sizeof(int));
    for(int i = 0; i < pb->n; i++) {
        sol->clientsConnectes[i] = 0;
    }

    sol->varConnexionsAffectees = malloc((long unsigned int)pb->m*sizeof(int*));
    for(int i = 0; i < pb->m; i++) {
        sol->varConnexionsAffectees[i] = malloc((long unsigned int)pb->n*sizeof(int));
        for(int j = 0; j < pb->n; j++) {
            sol->varConnexionsAffectees[i][j] = 0;
        }
    }

    sol->varServicesAffectees = malloc((long unsigned int)pb->m*sizeof(int));
    for(int i = 0; i < pb->m; i++) {
        sol->varServicesAffectees[i] = 0;
    }

    sol->nbVarConnFixees = 0;
    sol->nbVarServicesFixees = 0;


}

//------------------------------------------------------------------------------
void afficherSolution(Solution* sol) {

    printf("connexions :\n");
    for(int i = 0; i < sol->pb->m; i++) {
        for(int j = 0; j < sol->pb->n; j++) {
            printf("%lf, ", sol->connexions[i][j]);
        }
        printf("\n");
    }
    printf("\nservices ouverts :\n");
    for(int i = 0; i < sol->pb->m; i++) {
        printf("%d, ", sol->services[i]);
    }
    printf("\n");
    printf("z = %lf\n", sol->z);

}


//------------------------------------------------------------------------------
int solutionAdmissible(Solution* sol) {

    int res = 1;
    // vérification des capacités et demandes
    for(int i = 0; i < sol->pb->m; i++) {
        double somme = 0;
        for(int j = 0; j < sol->pb->n; j++) {
            if(sol->connexions[i][j]) {
                somme += sol->pb->demandes[j];
            }
        }
        if(somme > sol->pb->capacites[i]) {
            res = 0;
        }
    }
    // vérif que tous les clients sont connectés une et une seule fois
    for(int j = 0; j < sol->pb->n; j++) {
        int nbFois = 0;
        for(int i = 0; i < sol->pb->m;i++) {
            if(sol->connexions[i][j]) {
                nbFois ++;
                if(sol->services[i] == 0) {
                    res = 0;
                }
            }
        }
        if(nbFois != 1) {
            res = 0;
        }
    }

    return res;

}

//------------------------------------------------------------------------------
void copierSolution(Solution* sol, Solution* copie) {

    copie->pb = sol->pb;
    // valeurs des variables
    for(int i = 0; i < sol->pb->m; i++) {
        for(int j = 0; j < sol->pb->n; j++) {
            copie->connexions[i][j] = sol->connexions[i][j];
        }
        copie->services[i] = sol->services[i];
        copie->varServicesAffectees[i] = sol->varServicesAffectees[i];
        copie->capaRestantes[i] = sol->capaRestantes[i];
    }

    for(int i = 0; i < sol->pb->n; i++) {
        copie->varConnexionsAffectees[i] = sol->varConnexionsAffectees[i];
        copie->clientsConnectes[i] = sol->clientsConnectes[i];
    }

    copie->nbVarConnFixees = sol->nbVarConnFixees;
    copie->nbVarServicesFixees = sol->nbVarServicesFixees;
    copie->nbServicesOuverts = sol->nbServicesOuverts;
    copie->nbClientsConnectes = sol->nbClientsConnectes;
    copie->z = sol->z;

}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {

    free(sol->capaRestantes);

    for(int i = 0; i < sol->pb->m; i++) {
        free(sol->connexions[i]);
    }
    free(sol->connexions);
    free(sol->services);
    free(sol->clientsConnectes);

    for(int i = 0; i < sol->pb->m; i++) {
        free(sol->varConnexionsAffectees[i]);
    }
    free(sol->varConnexionsAffectees);
    free(sol->varServicesAffectees);
}
