/**
 * \file listeAffectation.c
 * \brief implémentation des fonctions de la structure de donnée ListeAffectation
 */

#include <stdlib.h>

#include "listeAffectation.h"


//------------------------------------------------------------------------------
void creerListeAffectation(ListeAffectation* liste) {
    liste->nbService = 0;
    liste->nbClient = 0;
    liste->dernierClient = NULL;
    liste->dernierService = NULL;
}


//------------------------------------------------------------------------------
int listeVide(ListeAffectation* liste) {
    return (liste->nbService == 0 && liste->nbClient == 0);
}

//------------------------------------------------------------------------------
void ajouterService(ListeAffectation* liste, int service) {
    MaillonService* maillon = malloc(sizeof(MaillonService));
    maillon->service = service;
    maillon->valeur = 0;
    maillon->prec = liste->dernierService;
    liste->dernierService = maillon;
    liste->nbService ++;
}

//------------------------------------------------------------------------------
void ajouterClient(ListeAffectation* liste, int client) {
    MaillonClient* maillon = malloc(sizeof(MaillonClient));
    maillon->client = client;
    maillon->valeur = 0;
    maillon->prec = liste->dernierClient;
    liste->dernierClient = maillon;
    liste->nbClient ++;
}

//------------------------------------------------------------------------------
void popAffectationClient(ListeAffectation* liste) {
    MaillonClient* aSup = liste->dernierClient;
    liste->dernierClient = aSup->prec;
    free(aSup);
    liste->nbClient --;
}

//------------------------------------------------------------------------------
void popAffectationService(ListeAffectation* liste) {
    MaillonService* aSup = liste->dernierService;
    liste->dernierService = aSup->prec;
    free(aSup);
    liste->nbService --;
}

//------------------------------------------------------------------------------
void detruireListe(ListeAffectation* liste) {
    while(liste->nbClient > 0) {
        popAffectationClient(liste);
    }
    while(liste->nbService > 0) {
        popAffectationService(liste);
    }
}
