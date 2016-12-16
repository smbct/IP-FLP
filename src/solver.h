/**
 * \file solver.h
 * \brief fonctions de résolution du SSCFLP basées sur un branch & bound
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "solution.h"

/**
 * \brief algorithme de branch & bound pour la résolution de SSCFLP
 * \param sol la solution contenant le problème à résoudre
 */
void branchBoundRec(Solution* sol, Solution* duale, Solution* best);

/**
 * \brief effectue un branch & bound de manière itérative
 * \param sol une solution vide du problème
 */
void branchBoundIter(Solution* sol);


/**
 * \brief backtrack dans l'arbre de résolution du problème
 */
void backtrack(ListeAffectation* liste);

void branchBound(Solution* sol);



#endif // SOLVER_H
