/**
 * \file listeAffectation.c
 * \brief implémentation des fonctions de la structure de donnée ListeAffectation
 */

#include <stdlib.h>

#include "listeAffectation.h"


//------------------------------------------------------------------------------
void creerListe(ListeAffectation* liste) {
    liste->nbService = 0;
    liste->nbClient = 0;

    liste->dernierClient = NULL;
    liste->dernierService = NULL;
}

//------------------------------------------------------------------------------
void ajouterService(ListeAffectation* liste, int service) {

    MaillonService* maillon = malloc(sizeof(MaillonService));
    maillon->service = service;
    maillon->valeur = 0;
    maillon->prec = liste->dernierClient;

}

//------------------------------------------------------------------------------
void ajoutClient(ListeAffectation* liste, int client) {

    MaillonClient* maillon = malloc(sizeof())

}

//------------------------------------------------------------------------------
void popAffectationClient(ListeAffectation* liste) {

}

//------------------------------------------------------------------------------
void popAffectationService(ListeAffectation* liste) {

}

//------------------------------------------------------------------------------
void detruireListe(ListeAffectation* liste) {


}
