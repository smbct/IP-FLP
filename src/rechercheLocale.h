/**
 * \file rechercheLocale.h
 * \brief def de procédures de recherche locale
 */

#ifndef RECHERCHE_LOCALE_H
#define RECHERCHE_LOCALE_H

#include "solution.h"

/**
 * \brief effectue une recherche locale
 * \param meilleure la meilleure solution trouvée
 */
void rechercheLocale(Solution* meilleure, int bestImp);

/**
 * \brief recherche d'un meilleur voisin pour basculer vers lui
 * \param meilleure la solution à améliorer
 * \param bestImp 1 ssi le meilleure voisin est sélectionner, sinon le premier améliorant est choisi
 * \return 0 ssi la solution n'a pas été améliorée
 */
int chercherVoisin(Solution* sol, int bestImp);

/**
 * \brief mise à jour de la solution
 * \param sol la solution à mettre à jour
 * \param z la nouvelle valeur de la fonction objectif
 * \param n le client actuel
 * \param nouvm le nouveau service
 */
void majSolution(Solution* sol, double z,int n, int nouvm);

#endif
