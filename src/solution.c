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

    sol->services = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < pb->m; i++) {
        sol->services[i] = -1; // variable non affectée
    }

    sol->capaRestantes = malloc((long unsigned int)pb->m*sizeof(double));
    for(int i = 0; i < pb->m;i++) {
        sol->capaRestantes[i] = pb->capacites[i];
    }

    sol->z = 0.;

    sol->nbServicesOuverts = 0;
    sol->nbClientsConnectes = 0;

    sol->connexionClient = malloc((long unsigned int)pb->n*sizeof(int));
    for(int i = 0; i < pb->n; i++) {
        sol->connexionClient[i] = -1; // variable non affectée
    }

    sol->nbVarClientFixees = 0;
    sol->nbVarServicesFixees = 0;

    sol->nbClientsServices = malloc((long unsigned int)pb->m*sizeof(int));
    for(int i = 0; i < pb->m; i++) {
        sol->nbClientsServices[i] = 0;
    }

}

//------------------------------------------------------------------------------
void resetSolution(Solution* sol) {

    // remise à 0 des capacités restantes et des services
    for(int i = 0; i < sol->pb->m; i++) {
        sol->services[i] = -1;
        sol->capaRestantes[i] = sol->pb->capacites[i];
    }
    sol->nbVarServicesFixees = 0;

    // remise à 0 des connexions
    for(int i = 0; i < sol->pb->n; i++) {
        sol->connexionClient[i] = -1;
    }
    sol->nbVarClientFixees = 0;

    sol->nbServicesOuverts = 0;
    sol->nbClientsConnectes = 0;

    for(int i = 0; i < sol->pb->m; i++) {
        sol->nbClientsServices[i] = 0;
    }

    sol->z = 0.;

}

//------------------------------------------------------------------------------
void afficherSolution(Solution* sol) {

    printf("connections:\n");
    for(int i = 0; i < sol->pb->n; i++) {
        printf("%d, ", sol->connexionClient[i]);
    }
    printf("\n");

    printf("\nfacilities:\n");
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
            if(sol->connexionClient[j] == i) { // si le client j est connecté à i, la demande est prise en compte
                somme += sol->pb->demandes[j];
            }
        }
        if(somme > sol->pb->capacites[i]) {
            res = 0;
        }
    }
    // vérif que tous les clients sont connectés une et une seule fois
    for(int i = 0; i < sol->pb->n; i++) {
        if(sol->connexionClient[i] < 0 || sol->connexionClient[i] >= sol->pb->m) {
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
        copie->services[i] = sol->services[i];
        copie->capaRestantes[i] = sol->capaRestantes[i];
        copie->nbClientsServices[i] = sol->nbClientsServices[i];
    }

    for(int i = 0; i < sol->pb->n; i++) {
        copie->connexionClient[i] = sol->connexionClient[i];
    }

    copie->nbVarServicesFixees = sol->nbVarServicesFixees;
    copie->nbServicesOuverts = sol->nbServicesOuverts;
    copie->nbClientsConnectes = sol->nbClientsConnectes;
    copie->z = sol->z;

}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {

    free(sol->capaRestantes);

    free(sol->services);
    free(sol->connexionClient);

    free(sol->nbClientsServices);
}
