/**
 * \file aco.h
 * \brief définitions de fonctions pour une métaheuristique de type colonies de fourmies
 */

#ifndef ACO_H
#define ACO_H

#include "solution.h"

/**
 * \brief construction d'une solution basée sur la métaheuristique des colonies de fourmies
 * \param[out] best la meilleure solutio trouvée par l'algorithme
 * \param[in] localsearch code de la recherche locale (0 -> sans, 1->descente, 2->PPD, 3->tabou)
 * \param[in] tabuListLenght si localsearch == 3 taille de la liste tabou
 * \param[in] tmaxtabu si localsearch == 3 budget de temps pour la recherche tabou
 * \param[in] tmax budget de temps de calcul
 * \param[in] alpha puissance des pheromones
 * \param[in] beta puissance de l'information heuristique
 * \param[in] rho coeficient d'evaporation
 * \param[in] pheromone_init valeur initiale des pheromones
 * \param[in] n_ants nombre de fourmis de la population
 * \param[in] pheremononeUpdateScheme methode de gestion des pheromones : 0->ACO, 1->EAS, 2->rank base AS
 * \param[in] nb_elit si EAS nombre de solution elite
 * \param[in] nu si rank base facteur de reduction en fonction du rang
 * \param[in] seed gaine du generateur aleatoire
 */
void construireACO(Solution* best, int localsearch, int tabuListLenght, long tmaxtabu, long tmax, double alpha, double beta, double rho, double pheromone_init, int n_ants, int pheremononeUpdateScheme, int nb_elit, double nu, unsigned int seed);

/**
 * \brief construction d'une solution en suivant les traces
 * \param sol la solution à construire
 * \param[in] probability matrice combine des pheromones et de l'information heuristique
 */
void constructionFourmi(Solution* sol, double** probability);

/**
 * \brief mise à jour des traces présentes sur chaque choix pour les variables
 * \param[in,out] best la meilleure solution connue, mise a jour
 * \param[in] n_ants le nombre de fourmi de la colonie
 * \param[in] solFourmi les solutions créées
 * \param[in,out] pheromone les traces actuelles
 * \param[in] rho coeficient d'evaporation
 * \param[in] pheremononeUpdateScheme methode de gestion des pheromones : 0->ACO, 1->EAS, 2->rank base AS
 * \param[in] nb_elit si EAS nombre de solution elite
 * \param[in] nu si rank base facteur de reduction en fonction du rang
 */
void majPheromones(Solution* best, int n_ants, Solution* solFourmi, double** pheromone, double rho, int pheremononeUpdateScheme, int nb_elit, double nu);

/**
 * \brief calcule les probabilité d'utiliser chaque association service/client
 * \param[in] pheromone matrice des pheromones
 * \param[in] heuristic matrice de l'information heuristique
 * \param[out] probability information combine des pheromones et de l'heuristique
 * \param[in] n nombre de client dans l'instance
 * \param[in] m nombre de service dans l'instance
 * \param[in] alpha puissance des pheromones
 * \param[in] beta puissance de l'information heuristique
 */
void calculProba(double ** pheromone, double ** heuristic, double ** probability, int n, int m, double alpha, double beta);

/*
 * \brief retourne un nombre aléatoire entre min et max
 * \param[in] min borne inférieur
 * \param[in] max borne supérieur
 */
double aleatoire(double min, double max);

#endif // ACO_H
