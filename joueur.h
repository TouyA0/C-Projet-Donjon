#ifndef JOUEUR_H
#define JOUEUR_H

#include "inventaire.h"

// Structure privée d’un joueur
struct sJoueur {
    int x, y ; // position du joueur dans le donjon
    int pv, pvMax ; // points de vie actuels / maximum
    int atk, def ; // attaque et défense
    tInventaire inv ; // inventaire
};

// Type public (pointeur vers structure privée)
typedef struct sJoueur * tJoueur ;

tJoueur JoueurCreer(int x, int y);

void JoueurLiberer(tJoueur *pj);

void JoueurPosition(const tJoueur j, int *px, int *py);

int JoueurPV(const tJoueur j);

int JoueurPVMax(const tJoueur j);

int JoueurAttaque(const tJoueur j);

int JoueurDefense(const tJoueur j);

tInventaire JoueurInventaire(const tJoueur j);

void JoueurMajPos(tJoueur j, int x, int y);

void JoueurMajPV(tJoueur j, int pv);

#endif