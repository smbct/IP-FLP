/**
 * \file tabu.c
 * \brief implémentation de la métaheuristique tabu
 */

#include "tabu.h"

#include "bornes.h"

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
void rechercheTabu(Solution* meilleure) {

    // construction gloutonne
    construction(meilleure);
    afficherSolution(meilleure);

    Solution sol;
    creerSolution(meilleure->pb, &sol);

    // construction de la liste tabu
    int** tabuListe = malloc((long unsigned int)sol.pb->n*sizeof(int*));
    for(int i = 0; i < sol.pb->n; i++) {
        tabuListe[i] = malloc((long unsigned int)sol.pb->m*sizeof(int));
        for(int j = 0; j < sol.pb->m; j++) {
            tabuListe[i][j] = 0;
        }
    }

    int longueurTabu = 7;

    int nbIt = 0;
    int nbItMax = 100;

    while(nbIt < nbItMax) {



        nbIt ++;
    }


    detruireSolution(&sol);

    for(int i = 0; i < sol.pb->n; i++) {
        free(tabuListe[i]);
    }
    free(tabuListe);
}
