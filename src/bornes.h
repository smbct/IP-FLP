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
 * \param sol la solution contenant le résultat de la relaxation
 */
void relaxationContinue(Solution* sol);

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
