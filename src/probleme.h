/**
 * \file probleme.h
 * \brief définition d'une structure de donnée pour les instances FLP et de fonctions pour le chargement
 */

#ifndef PROBLEME_H
#define PROBLEME_H

typedef struct {

    int m; // nombre de service
    int n; // nombre de client

    double* couts; // coûts d'ouverture des services
    double* capacites; // capacités des services

    double* demandes; // demandes des clients

    double** liaisons; // coûts des liaisons

} Probleme;

/**
 * \brief chargement d'un problème au format OR LIBRARY
 * \param pb le probleme à charger
 * \param nom le nom du fichier à charger
 */
void chargerProbleme(Probleme* pb, char* nom);

/**
 * \brief affichage formaté d'un problème
 * \param pb le problème à afficher
 */
void afficherProbleme(Probleme* pb);

/**
 * \brief désallocation d'un problème
 * \param pb le probleme à désallouer
 */
void detruireProbleme(Probleme* pb);

#endif // PROBLEME_H
