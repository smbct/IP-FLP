/**
 * \file main.c
 * \brief point d'entrée du programme
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "probleme.h"
#include "solution.h"
//#include "bornes.h"
//#include "solver.h"
#include "aco.h"
#include "tabu.h"
#include "rechercheLocale.h"

//./main --localsearch 2 --alpha 0.7 --beta 2 --rho 0.9 --pheromone_init 0.0005 --n_ants 700 --pheremononeUpdateScheme 2 --nu 0.6  --seed 0 --instance Instances/Holmberg/p11.dat --tmax 20 --bestval 8932
//./main --localsearch 2 --alpha 0.7 --beta 2 --rho 0.9 --pheromone_init 0.0005 --n_ants 700 --pheremononeUpdateScheme 2 --nu 0.6  --seed 0 --instance Instances/Holmberg/p22.dat --tmax 50 --bestval 7092

//./main --localsearch 2 --alpha 0.9516 --beta 0.9619 --rho 0.5448 --pheromone_init 0.3168 --n_ants 683 --pheremononeUpdateScheme 0  --seed 0 --instance Instances/Holmberg/p11.dat --tmax 20 --bestval 8932
//./main --localsearch 2 --alpha 0.9516 --beta 0.9619 --rho 0.5448 --pheromone_init 0.3168 --n_ants 683 --pheremononeUpdateScheme 0  --seed 0 --instance Instances/Holmberg/p22.dat --tmax 50 --bestval 7092


void resoudre(Solution* sol, int localsearch, int tabuListLenght, long tmaxtabu, clock_t tmax, double alpha, double beta, double rho, double pheromone_init, int n_ants, int pheremononeUpdateScheme, int nb_elit, double nu, double bestval, unsigned int seed);

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    //parametres d'appel
        char * instanceFile; //nom du fichier d'instance

        int localsearch; //code de la recherche locale (0 -> sans, 1->descente, 2->PPD, 3->tabou)
        int tabuListLenght; //si localsearch == 3 taille de la liste tabou
        long tmaxtabu; //temps de recherche maximale de la recherche tabou
        long tmax; //budget de temps de calcul
        double alpha; //puissance des pheromones
        double beta; //puissance de l'information heuristique
        double rho; //coeficient d'evaporation
        double pheromone_init; //valeur initiale des pheromones
        int n_ants; //nombre de fourmis de la population
        int pheremononeUpdateScheme; //methode de gestion des pheromones : 0->ACO, 1->EAS, 2->rank base AS
        int nb_elit; //si EAS nombre de solution elite
        double nu; //si rank base facteur de reduction en fonction du rang
        unsigned int seed; //graine du generateur aleatoire
        double bestval; //meilleure valeur connue pour l'instance



    //reading all parameter
    if (argc <= 1) {
        printf("Pas d'arguments\n");
        return 0;
    }
    for(int i=1; i< argc ; i++){
        if(strcmp(argv[i], "--localsearch") == 0){
            localsearch = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--tabuListLenght") == 0){
            tabuListLenght = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--tmaxtabu") == 0) {
            tmaxtabu = atol(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--tmax") == 0) {
            tmax = atol(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--alpha") == 0){
            alpha = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--beta") == 0){
            beta = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--rho") == 0){
            rho = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--pheromone_init") == 0){
            pheromone_init = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--n_ants") == 0){
            n_ants = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--pheremononeUpdateScheme") == 0){
            pheremononeUpdateScheme = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--nb_elit") == 0){
            nb_elit = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--nu") == 0){
            nu = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--instance") == 0) {
            instanceFile = argv[i+1];
            i++;
        } else if(strcmp(argv[i], "--seed") == 0){
            seed = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--bestval") == 0){
            bestval = atof(argv[i+1]);
            i++;
        }

    }

    long tstart = clock();

    //debut
    Probleme pb;

    chargerProbleme(&pb, instanceFile);

    Solution sol;

    creerSolution(&pb, &sol);

    resoudre(&sol, localsearch, tabuListLenght, tmaxtabu, tmax, alpha, beta, rho, pheromone_init, n_ants, pheremononeUpdateScheme, nb_elit, nu, bestval, seed);

    detruireSolution(&sol);

    detruireProbleme(&pb);

    printf("temps : %lf  \n", (double)((double)(clock() - tstart)/(double)(CLOCKS_PER_SEC)));

    //fin
    return 0;
}

//------------------------------------------------------------------------------
void resoudre(Solution* sol, int localsearch, int tabuListLenght, long tmaxtabu, clock_t tmax, double alpha, double beta, double rho, double pheromone_init, int n_ants, int pheremononeUpdateScheme, int nb_elit, double nu, double bestval, unsigned int seed) {

    //juste pour tester l'aco
    printf("Lancement de l'ACO\n");

    construireACO(sol, localsearch, tabuListLenght, tmaxtabu, tmax, alpha, beta, rho, pheromone_init, n_ants, pheremononeUpdateScheme, nb_elit, nu, seed);

    printf("relative deviation : %lf \n", 100.0*(double)(sol->z - bestval)/(double)bestval);

    afficherSolution(sol);


    //pour lancer la resolution exacte
/*  printf("Lancement du branch & bound\n");

    branchBoundIter(sol);

    printf("Solution optimale : \n");

    afficherSolution(sol);
*/
}
