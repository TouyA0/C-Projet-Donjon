#ifndef ENNEMI_H
#define ENNEMI_H

#include "joueur.h"

#define NOM_MAX 32

// Structure privée d’un ennemi
struct sEnnemi {
    char nom [ NOM_MAX ]; // NOM_MAX est à 32
    int pv , pvMax , atk , def ;
};

// Type public ( pointeur vers structure privée)
typedef struct sEnnemi * tEnnemi ;

tEnnemi EnnemiCreer(const char *nom, int pvMax, int atk, int def);

void EnnemiLiberer(tEnnemi *pe);

char *EnnemiNom(const tEnnemi e);

int EnnemiPV(const tEnnemi e);

int EnnemiPVMax(const tEnnemi e);

int EnnemiAttaque(const tEnnemi e);

int EnnemiDefense(const tEnnemi e);

int EnnemiEstVivant(const tEnnemi e);

int EnnemiSubirAttaque(tEnnemi e, const tJoueur j);

int EnnemiAttaquerJoueur(const tEnnemi e, tJoueur j);

void EnnemiMajPV(tEnnemi e, int pv);

#endif