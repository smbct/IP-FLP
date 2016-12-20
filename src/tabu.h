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

/**
 * \brief sélection de la prochaine solution à partir de laquelle chercher
 * \param sol la solution actuelle
 * \param meilleure la meilleure solution connue
 * \param tabuListe la liste tabu
 * \param it l'itération actuelle
 * \param longueurTabu la longueur tabou
 */
void selectionnerVoisin(Solution* sol, Solution* meilleure, int** tabuListe, int it, int longueurTabu);

#endif // TABU_H
