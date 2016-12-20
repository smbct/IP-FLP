/**
 * \file tabu.h
 * \brief définitions de fonctions pour une métaheuristique de type tabu
 */

#ifndef TABU_H
#define TABU_H

#include "solution.h"

/**
 * \brief effectue une recherche tabu
 * \param meilleure la meilleure solution trouvée
 */
void rechercheTabu(Solution* meilleure);

#endif // TABU_H
