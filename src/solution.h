/**
 * \file solution.h
 * \brief définition d'une structure de donnée représentant une solution du SSCFLP
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include "probleme.h"

typedef struct {

    double** connexions; // les services connectés aux clients (dans cet ordre)
    int* services; // les services qui sont ouverts

    int nbVarServicesFixees; // nombre de variable de service fixées
    int nbVarConnFixees; // nombre de variables de clients affectées

    int nbServicesOuverts; // nombre de service ouvert
    int nbClientsConnectes; // nombre de client connecté

    int* clientsConnectes; // tableau de booléens, 1 ssi le client i est connecté
    int** varConnexionsAffectees;  // tableau de booléen indiquant les variables de connexions fixées durant la résolution
    int* varServicesAffectees; // tableau de booléen indiquant les variables de services fixées durant la résolution

    double* capaRestantes; // capacités restantes des services
    Probleme* pb; // problème à résoudre
    double z; // valeur de la fonction objective

} Solution;

/**
 * \brief création d'une solution pour le problème pb
 * \param pb le problème correspondant à la solution
 */
void creerSolution(Probleme* pb, Solution* sol);

/**
 * \brief affichage de la solution
 * \brief sol la solution à afficher
 */
void afficherSolution(Solution* sol);

/**
 * \brief vérification de l'admissibilité de la solution
 * \param sol la solution à vérifier
 * \return vrai ssi la solution est admissible
 */
int solutionAdmissible(Solution* sol);

/**
 * \brief copie d'une solution dans une autre
 * \param sol la solution à copier
 * \param copie la copie de sol
 */
void copierSolution(Solution* sol, Solution* copie);

/**
 * \brief désallocation mémoire d'une solution
 * \param sol la solution à désallouer
 */
void detruireSolution(Solution* sol);

#endif
