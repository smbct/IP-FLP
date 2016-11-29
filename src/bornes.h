/**
 * \file brones.h
 * \brief définitions de fonctions de calcul de bornes pour le SSCFLP
 */

#ifndef BORNES_H
#define BORNES_H

#include "solution.h"


/**
 * \brief construction d'une solution heuristique
 * \param sol la solution à construire
 */
void construction(Solution* sol);

/**
 * \brief relaxation continue du problème
 * \param sol la solution contenant le sous problème à résoudre (variables non affectées)
 * \param otpim la solution contenant z, et les valeurs de variables si entières
 * \return -1 si le problème est impossible, 0 si la solution est non entière et 1 si la solution est entière
 */
int relaxationContinue(Solution* sol, Solution* optim);

/**
 * \brief relaxation du problème en résolvant le CFLP
 * \param sol la solution contenant le résultat de la relaxation
 */
void relaxationCFLP(Solution* sol);

/**
 * \brief relaxation du problème en résolvant le UFLP
 * \param sol la solution contenant le résultat de la relaxation
 */
void relaxationUFLP(Solution* sol);

#endif
