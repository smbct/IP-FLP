/**
 * \file tri.h
 * \brief définition de fonctions de tri
 */

/**
 * \brief tri le tableau ind par ordre décroissant en fonction de certaines valeurs
 * \param nbVal le nombre de valeur des tableaux
 * \param ind les indices des éléments à trier
 * \param val les valeurs des éléments à trier
 * \param val2 les valeurs des éléments à trier en cas d'égalité sur les valeurs val
 */
void trierCroissant(int nbVal, int* ind, double* val, double* val2);

/**
 * \brief procédure récursive de tri fusion
 * \param deb l'indice de début du tableau à trier
 * \param fin l'indice de fin du tableau à trier
 * \param ind les indices des valeurs à trier
 * \param val les valeurs à trier
 * \param val2 les valeurs des éléments à trier en cas d'égalité sur les valeurs val
 */
void triRec(int deb, int fin, int* ind, double* val, double* val2);

/**
 * \brief fusion en un tableau trié de deux sous-tableaux triés
 * \param deb indice de départ du premier tableau
 * \param milieu indice de fin du premier tableau
 * \param fin indice de fin du deuxième tableau
 * \param ind tableaux des indices des valeurs à trier
 * \param val valeurs à trier
 * \param val2 les valeurs des éléments à trier en cas d'égalité sur les valeurs val
 */
void fusion(int deb, int milieu, int fin, int* ind, double* val, double* val2);
