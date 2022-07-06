/**
 * \file solver.c
 * \brief fonctions de résolution du SSCFLP basées sur un branch & bound
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "solver.h"
#include "bornes.h"

#include "tabu.h"

//------------------------------------------------------------------------------
void branchBound(Solution* sol) {

    Solution duale, best;
    creerSolution(sol->pb, &duale);
    creerSolution(sol->pb, &best);

    construction(&best);
    printf("Valeur initiale : %lf\n", best.z);

    printf("\n\nLancement du B&B \n\n");

    branchBoundRec(sol, &duale, &best);

    copierSolution(&best, sol);

    detruireSolution(&duale);
    detruireSolution(&best);

}

//------------------------------------------------------------------------------
void branchBoundRec(Solution* sol, Solution* duale, Solution* best) {

    printf("\tDEBUG\n");
    printf("services affectés : \n");
    for(int i = 0; i < sol->pb->m; i++) {
        printf("%d, ", sol->services[i]);
    }

    printf("\nclients affectées : \n");
    for(int i = 0; i < sol->pb->n; i++) {
        printf("%d, ", sol->connexionClient[i]);
    }
    printf("\n");

    // relaxation continue
    int resRelax = relaxationContinue(sol, duale);

    printf("relaxation : %lf\n", duale->z);
    printf("res relax continue : %d\n", resRelax);

    printf("nb services fixés : %d\n", sol->nbVarServicesFixees);
    printf("nb clients fixés : %d\n", sol->nbVarClientFixees);

    printf("meilleure solution connue : %lf\n", best->z);

    printf("\n");
    printf("\tFIN DEBUG\n\n");

    printf("\n\n\n");

    // si la relaxation ne permet pas de couper, le branchement est obligatoire
    if(resRelax == 0 && duale->z < best->z) {

        // branchement d'abord sur les services
        if(sol->nbVarServicesFixees < sol->pb->m) { // fixation de l'ouverture d'un service

            sol->nbVarServicesFixees ++;

            // fixation à 0
            sol->services[sol->nbVarServicesFixees-1] = 0;
            branchBoundRec(sol, duale, best);

            // fixation à 1
            sol->services[sol->nbVarServicesFixees-1] = 1;
            branchBoundRec(sol, duale, best);

            // ensuite l'affectation de la variable est annulée
            sol->nbVarServicesFixees --;
            sol->services[sol->nbVarServicesFixees] = -1;

        } else if(sol->nbVarClientFixees < sol->pb->n) { // fixation d'un client

            // affectation du prochain client
            // TODO : ordre d'affectation des clients
            int client = sol->nbVarClientFixees;

            // branchement sur cette variable
            sol->nbVarClientFixees ++;

            printf("affectation client %d\n", client);

            // on essaie de connecter le client à chacun des services, quand c'est possible

            for(int i = 0; i < sol->pb->n; i++) {
                // on peut vérifier facilement si la solution reste admissible en terme de respet des capacités
                // if(sol->capaRestantes[i] <= sol->pb->demandes[j]) {
                    printf("test\n");
                    sol->capaRestantes[i] -= sol->pb->demandes[client];
                    sol->connexionClient[client] = i;
                    branchBoundRec(sol, duale, best);
                    sol->capaRestantes[i] += sol->pb->demandes[client];
                // }
            }

            // ensuite la variable redevient libre
            sol->nbVarClientFixees --;
            sol->connexionClient[client] = -1;

        }

    } else if(resRelax == 1) {
        printf("La solution obtenue est entière, pas besoin de brancher\n");
        // solution entière, pas besoin brancher et en plus une solution entière de plus
        if(duale->z < best->z) { // meilleure solution mise à jour
            copierSolution(duale, best);
        }
        printf("meilleure solution : %lf\n", duale->z);
    } else {

        printf("branche coupée :\n");

        if(resRelax == -1) {
            printf("Le sous problème actuel n'admet aucune solution\n");
        } else {
            printf("La relaxation continue n'est pas mieux que la meilleure solution connue\n");
        }
        printf("\n\n\n");
    }

}

//------------------------------------------------------------------------------
void branchBoundIter(Solution* sol) {

    clock_t begin, end;
    double temps;

    Solution duale, best;
    creerSolution(sol->pb, &duale);
    creerSolution(sol->pb, &best);

    // création et initialisation du problème relaché pour glpk
    glp_prob* glp_prob_relax;
    glp_prob_relax = glp_create_prob();
    initGLPKProblemeRelache(glp_prob_relax, sol);

    // printf("Construction gloutonne d'une première solution : \n");
    // construction(&best);
    // printf("Valeur initiale de la solution gloutonne : %lf\n", best.z);

    // printf("Lancement d'une recherche tabou : \n");
    // int longueurTabu = 10;
    // long tmax = 5;
    // rechercheTabu(&best, longueurTabu, tmax);
    // printf("Valeur initiale retournée par tabou : %lf\n", best.z);

    // printf("Lancement d'une heuristique de colonies de fourmies : \n");
    // --localsearch 2 --alpha 0.728 --beta 0.6774 --rho 0.1665 --pheromone_init 0.8331 --n_ants 118 --pheremononeUpdateScheme 0
    // construireACO(sol, localsearch, tabuListLenght, tmaxtabu, tmax, alpha, beta, rho, pheromone_init, n_ants, pheremononeUpdateScheme, nb_elit, nu, seed);
    // construireACO(&best, 2, 5, 5, 1, 0.728, 0.6774, 0.1665, 0.8331, 118, 0, 0, 0, 42);
    // printf("Valeur initiale retournée par l'heuristique des colonies de fourmies : %lf\n", best.z);

    afficherSolution(&best);

    // printf("valeur initiale oracle : 9881.5\n");
    // best.z = 9881.5;

    printf("valeur initiale oracle : 11630\n");
    best.z = 11630;

    printf("\n\nLancement du B&B \n\n");

    ListeAffectation liste;
    creerListeAffectation(&liste);

    // ajout du premier service
    ajouterService(&liste, 0);
    sol->nbVarServicesFixees ++;
    sol->services[0] = 0;

    begin = clock();
    temps = 0.;

    int nbIt = 0;

    while(!listeVide(&liste) && temps < 600.) {

        // nbIt ++;
        // if(nbIt > 100) {
        //   break;
        // }

        printf("------------------------DEBUG--------------------------\n");
        printf("nb service affecté : %d\n", liste.nbService);
        printf("nb client affecté : %d\n", liste.nbClient);
        printf("meilleure valeur trouvée : %lf\n", best.z);
        printf("solution actuelle partielle : z = %lf\n", sol->z);

        printf("Affectation des variables de service :\n");
        for(int i = 0; i < sol->pb->m; i++) {
            printf("%d, ", sol->services[i]);
        }
        printf("\nAffectation des variables de clients :\n");
        for(int i = 0; i < sol->pb->n; i++) {
            printf("%d, ", sol->connexionClient[i]);
        }
        printf("\n");

        // test si toutes les variables ont été fixées
        if(sol->nbVarServicesFixees == sol->pb->m && sol->nbVarClientFixees == sol->pb->n) {

            printf("problème fixé\n");

            // mise à jour de la meilleure solution trouvée
            if(sol->z < best.z) {
              copierSolution(sol, &best);
            }

            backtrack(&liste, sol, &best);

        } else { // sinon, on borne le noeud courant par relaxation continue (avec warm start)

          // relaxation continue
          // clock_t relax_begin = clock();

          // int resRelax = relaxationContinue2(sol, &duale);

          // printf("test 1 \n");
          majGLPKProblemeRelache(glp_prob_relax, sol);
          int resRelax = solveGLPKProblemeRelache(glp_prob_relax, &duale);
          // printf("test 2 \n");


          // clock_t relax_end = clock();
          // printf("temps relaxation continue: %f \n", (double)(relax_end-relax_begin) / CLOCKS_PER_SEC);

          printf("Résultat de la relaxation continue : %d, z = %lf\n", resRelax, duale.z);



          if(resRelax == 0) { // relaxation non entière

              if(duale.z < best.z) {

                  // ajout d'affectation si nécessaire
                  if(liste.nbService < sol->pb->m) {


                      sol->services[liste.nbService] = 0;
                      sol->nbVarServicesFixees ++; // le service est affecté à 0, pas besoin de modifier z
                      ajouterService(&liste, liste.nbService);


                  } else if(liste.nbClient < sol->pb->n) {

                      // le service n'est pas forcément ouvert, il faut vérifier

                      // recherche d'un service ouvert et qui peut être connecté au cient
                      int i = 0;
                      while( (sol->services[i] != 1 || sol->capaRestantes[i] < sol->pb->demandes[liste.nbClient]) && i < sol->pb->m) {
                          i ++;
                      }

                      if(i < sol->pb->m) {
                          sol->connexionClient[liste.nbClient] = i;
                          sol->nbVarClientFixees ++;
                          ajouterClient(&liste, liste.nbClient);
                          liste.dernierClient->valeur = i;
                          sol->z += sol->pb->liaisons[i][liste.dernierClient->client];
                          // mise à jour des capacités restantes
                          sol->capaRestantes[liste.dernierClient->valeur] -= sol->pb->demandes[liste.dernierClient->client];
                      } else {
                          // le client ne peut être affecté à aucun service, il faut backtracker
                          backtrack(&liste, sol, &best);
                      }

                  } else { // problème fixé, calcul de la solution

                      // vérification de la solution
                      // ne doit pas arriver par la relaxation

                  }


              } else {
                  printf("La relaxation permet de couper dans la recherche\n");
                  backtrack(&liste, sol, &best);
              }

          } else if(resRelax == 1) { // relaxation entière, vérif et backtrack

              printf("Relaxation continue donne une solution entière : z = %lf\n", duale.z);

              if(duale.z < best.z) { // une meilleure solution est trouvée, mise à jour
                  copierSolution(&duale, &best);
              }
              // après ça, backtracking
              backtrack(&liste, sol, &best);

          } else { // le problème du noeud courant est impossible

              printf("La relaxation est un problème impossible\n");
              backtrack(&liste, sol, &best);

          }

        }



        printf("----------------------FIN DEBUG------------------------\n\n\n\n");

        end = clock();
        temps = (double)(end-begin) / CLOCKS_PER_SEC;

    }

    end = clock();
    temps = (double)(end-begin) / CLOCKS_PER_SEC;
    printf("La résolution s'est terminée en %lf secondes\n", temps);

    copierSolution(&best, sol);

    detruireSolution(&duale);
    detruireSolution(&best);

    glp_delete_prob(glp_prob_relax); // libération de la mémoire du pb relaché glpk

}

//------------------------------------------------------------------------------
int backtrack(ListeAffectation* liste, Solution* sol, Solution* best) {

    printf("...backtracking\n");

    int continuer = 1;
    int vide = 0;

    int retirer = 1;

    while(continuer) {

        retirer = 1;

        // l'affectation suivante est tentée, sinon le nouveau dernier est popé
        if(liste->nbClient > 0) { // tentative d'affectation du dernier client
            if(liste->dernierClient->valeur < sol->pb->m-1) {
                sol->capaRestantes[liste->dernierClient->valeur] += sol->pb->demandes[liste->dernierClient->client];
                sol->z -= sol->pb->liaisons[liste->dernierClient->valeur][liste->dernierClient->client];
                liste->dernierClient->valeur ++;
                sol->connexionClient[liste->dernierClient->client] ++;
                sol->capaRestantes[liste->dernierClient->valeur] -= sol->pb->demandes[liste->dernierClient->client];
                sol->z += sol->pb->liaisons[liste->dernierClient->valeur][liste->dernierClient->client];
                // on peut tester directement que les capacités sont bien respectées
                if(sol->capaRestantes[liste->dernierClient->valeur] >= 0 && sol->z < best->z) {
                    continuer = 0; // si les capacités sont bonnes, on peut effectuer la relaxation continue
                    retirer = 0;
                } else if(liste->dernierClient->valeur < sol->pb->m) { // si le nouveau sous-problème n'est pas réalisable mais que tous les services n'ont pas été testés, on ne veut pas retirer la dernière affectation
                    retirer = 0;
                }
            }
        } else if(liste->nbService > 0) {
            if(liste->dernierService->valeur < 1) {
                liste->dernierService->valeur = 1;
                sol->services[liste->dernierService->service] = 1;
                sol->z += sol->pb->couts[liste->dernierService->service];

                if(sol->z < best->z) {
                    continuer = 0;
                    retirer = 0;
                }

            }
        }

        if(retirer) { // si aucune affectation n'a pu être faite, le backtrack continue, la dernière affectation est popée

            // le dernier de la liste est popé
            if(liste->nbClient > 0) { // un client est popé
                // avant de l'enlever, re mise-à-jour de la capacité restante du service
                sol->capaRestantes[liste->dernierClient->valeur] += sol->pb->demandes[liste->dernierClient->client];
                sol->z -= sol->pb->liaisons[liste->dernierClient->valeur][liste->dernierClient->client];
                popAffectationClient(liste);
                sol->connexionClient[liste->nbClient] = -1;
                sol->nbVarClientFixees --;
            } else if(liste->nbService > 0) { // un service est popé
                sol->z -= sol->pb->couts[liste->dernierService->service];
                popAffectationService(liste);
                sol->services[liste->nbService] = -1;
                sol->nbVarServicesFixees --;
            } else {
                vide = 1;
                continuer = 0;
            }

        }

    }

    return !vide;
}
