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
    int nbVarFixees;
    double* capaRestantes;
    Probleme* pb;
    double z;

} Solution;

/**
 * \brief création d'une solution pour le problème pb
 * \param pb le problème correspondant à la solution
 */
void creerSolution(Probleme* pb, Solution* sol);

/**
 * \brief affochage de la solution
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
 * \brief désallocation mémoire d'une solution
 * \param sol la solution à désallouer
 */
void detruireSolution(Solution* sol);

#endif
