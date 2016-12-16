/**
 * \file listeAffectation.h
 * \brief définition d'une structure de donnée de type file pour conserver les affectations des variables du problème
 */

#ifndef LISTE_AFFECTATION_H
#define LISTE_AFFECTATION_H

/**
 * \class MaillonService
 * \brief structure de donnée représentant une affectation de service
 */
typedef struct MaillonService {
    int service; // indice du service
    int valeur; // valeur du service (0 ou 1)

    struct MaillonService* prec; // précédent dans la liste
} MaillonService;


/**
 * \class MaillonClient
 * \brief structure de donnée représentant une connexion de client
 **/
typedef struct MaillonClient {
    int client; // indice du client à connecter
    int valeur; // service auquel est connecté le client

    struct MaillonClient* prec; // précédent dans la liste
} MaillonClient;

typedef struct {
    int nbService; // nombre de service affecté
    int nbClient; // nombbre de client affecté

    struct MaillonService* dernierService; // dernier service du chaînage
    struct MaillonClient* dernierClient; // dernier client du chaînage
} ListeAffectation;

/**
 * \brief création d'une liste d'affectations
 * \param liste la liste à créer
 */
void creerListeAffectation(ListeAffectation* liste);

/**
 * \brief retourne vrai ssi la liste ne contient aucun service et aucun client
 * \param liste la liste à tester
 */
int listeVide(ListeAffectation* liste);

/**
 * \brief ajout d'une affectation de service à la liste
 * \param liste la liste à modifier
 * \param service l'indice du service à affecter
 */
void ajouterService(ListeAffectation* liste, int service);

/**
 * \brief ajout d'une affectation de client à la liste
 * \param liste la liste à modifier
 * \param client l'indice du client à affecter
 */
void ajouterClient(ListeAffectation* liste, int client);

/**
 * \brief retire la dernière affectation de client effectuée
 * \param liste la liste à modifier
 */
void popAffectationClient(ListeAffectation* liste);

/**
 * \brief retire la dernière affectation de service effectuée
 * \param liste la liste à modifier
 */
void popAffectationService(ListeAffectation* liste);

/**
 * \brief désallocation mémoire de la liste
 */
void detruireListe(ListeAffectation* liste);


#endif // LISTE_AFFECTATION_H
