#include "ennemi.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tEnnemi EnnemiCreer(const char *nom, int pvMax, int atk, int def) {
    if (nom == NULL || pvMax <= 0 || atk <= 0 || def < 0) {
        return NULL;
    }

    tEnnemi nv_ennemi = malloc(sizeof(struct sEnnemi));
    if (nv_ennemi == NULL) {
        return NULL;
    }

    strcpy(nv_ennemi->nom, nom);
    nv_ennemi->pv = nv_ennemi->pvMax = pvMax;
    nv_ennemi->atk = atk;
    nv_ennemi->def = def;

    return nv_ennemi;
}

void EnnemiLiberer(tEnnemi *pe)  {
    if (pe != NULL && *pe != NULL) {
        free(*pe);
        *pe = NULL;
    }
}

char *EnnemiNom(const tEnnemi e) {
    if (e == NULL) {
        return NULL;
    }

    return e->nom;
}

int EnnemiPV(const tEnnemi e) {
    if (e == NULL) {
        return 0;
    }

    return e->pv;
}

int EnnemiPVMax(const tEnnemi e) {
    if (e == NULL) {
        return 0;
    }

    return e->pvMax;
}

int EnnemiAttaque(const tEnnemi e) {
    if (e == NULL) {
        return 0;
    }

    return e->atk;
}

int EnnemiDefense(const tEnnemi e) {
    if (e == NULL) {
        return 0;
    }

    return e->def;
}

int EnnemiEstVivant(const tEnnemi e) {
    if (e == NULL || e->pv <= 0) {
        return 0;
    }

    return 1;
}

int EnnemiSubirAttaque(tEnnemi e, const tJoueur j) {
    if (e == NULL || j == NULL) {
        return 0;
    }

    int degats = JoueurAttaque(j) - EnnemiDefense(e);
    if (degats < 1) {
        degats = 1; // minimum 1 de degat
    }

    // max de degats = pv de l'ennemi
    if (degats > e->pv) {
        degats = e->pv;
    }

    e->pv -= degats;
    return degats;
}

int EnnemiAttaquerJoueur(const tEnnemi e, tJoueur j) {
    if (e == NULL || j == NULL) {
        return 0;
    }

    int degats = EnnemiAttaque(e) - JoueurDefense(j);
    if (degats < 1) {
        degats = 1;  // minimum 1 dégât
    }

    // max de degats = pv du joueur
    if (degats > JoueurPV(j)) {
        degats = JoueurPV(j);
    }

    JoueurMajPV(j, JoueurPV(j) - degats);
    return degats;
}

void EnnemiMajPV(tEnnemi e, int pv) {
    if (e == NULL) {
        return;
    }

    // max de pv = pvMax
    if (pv > e->pvMax) {
        pv = e->pvMax;
    }

    // min de pv = 0
    if (pv < 0) {
        pv = 0;
    }

    e->pv = pv;
}