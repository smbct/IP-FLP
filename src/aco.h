/**
 * \file aco.h
 * \brief définitions de fonctions pour une métaheuristique de type colonies de fourmies
 */

#ifndef ACO_H
#define ACO_H

#include "solution.h"

/**
 * \brief construction d'une solution basée sur la métaheuristique des colonies de fourmies
 * \param best la meilleure solutio trouvée par l'algorithme
 */
void construireACO(Solution* best);

/**
 * \brief construction d'une solution en suivant les traces
 * \param sol la solution à construire
 * \param pheroConn les phéromones présentes sur la solution
 */
void constructionFourmi(Solution* sol, double** pheroConn);

/**
 * \brief mise à jour des traces présentes sur chaque choix pour les variables
 * \param best la meilleure solution connue
 * \param nbFourmi le nombre de fourmi de la colonie
 * \param solFourmi les solutions créées
 * \param phero les traces actuelles
 */
void majPheromones(double best, int nbFourmi, Solution* solFourmi, double** phero);

/*
 * \brief retourne un nombre aléatoire entre min et max
 */
double aleatoire(double min, double max);

#endif // ACO_H
