/**
 * \file solver.h
 * \brief fonctions de résolution du SSCFLP basées sur un branch & bound
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "solution.h"

#include "listeAffectation.h"

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
 * \param liste la liste des variables en cours d'affectation
 * \param sol la solution en cours de résolution
 * \param best la meilleure solution connue à ce stade de la résolution
 * \return faux si la liste d'affectation est vide (la recherche arborescente est alors terminée)
 */
int backtrack(ListeAffectation* liste, Solution* sol, Solution* best);

/**
 * \brief algorithme de branch and bound récursif pour calculer la solution optimale du SSCFLP
 * \param sol la solution optimale du problème (initialement vide)
 */
void branchBound(Solution* sol);


#endif // SOLVER_H
