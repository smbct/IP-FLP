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
void branchBound(Solution* sol);



#endif
