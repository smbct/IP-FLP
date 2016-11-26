/**
 * \file tri.c
 * \brief implémentation de fonctions de tri
 */

#include <stdio.h>
#include <stdlib.h>

#include "tri.h"

//------------------------------------------------------------------------------
void trierCroissant(int nbVal, int* ind, double* val, double* val2) {
    triRec(0, nbVal-1, ind, val, val2);
}

//------------------------------------------------------------------------------
void triRec(int deb, int fin, int* ind, double* val, double* val2) {

    if(deb < fin) {

        int milieu = (deb+fin)/2;

        if(deb < milieu) {
            triRec(deb, milieu, ind, val, val2);
        }
        if(milieu+1 < fin) {
            triRec(milieu+1, fin, ind, val, val2);
        }

        fusion(deb, milieu, fin, ind, val, val2);
    }

}

//------------------------------------------------------------------------------
void fusion(int deb, int milieu, int fin, int* ind, double* val, double* val2) {

    int* triees = malloc((long unsigned int)(fin-deb+1)*sizeof(int));

    int indGch = deb;
    int indDte = milieu+1;

    for(int i = 0; i < fin-deb+1; i++) { // parcours de toutes les valeurs à trier

        // cas où la c'est l'élément du tableau de droite qui est sélectionné
        if(indDte <= fin && (indGch > milieu || (val[ind[indDte]] < val[ind[indGch]] || (val[ind[indDte]] == val[ind[indGch]] && val2[ind[indDte]] > val2[ind[indGch]]))))
        {
            triees[i] = ind[indDte];
            indDte ++;
        } else { // cas où c'est l'élément de gauche qui est sélectionné
            triees[i] = ind[indGch];
            indGch ++;
        }
    }

    // copie des valeurs triees dans le tableau original
    for(int i = deb; i <= fin; i++) {
        ind[i] = triees[i-deb];
    }

    // libération mémoire du tableau trié
    free(triees);

}
