#include "joueur.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tJoueur JoueurCreer(int x, int y) {
    tJoueur nv_joueur = malloc(sizeof(struct sJoueur));
    if (nv_joueur == NULL) {
        return NULL;
    }

    nv_joueur->x = x;
    nv_joueur->y = y;
    nv_joueur->pv = 20;
    nv_joueur->pvMax = 20;
    nv_joueur->atk = 5;
    nv_joueur->def = 1;

    nv_joueur->inv = InventaireCreer();
    if (nv_joueur->inv == NULL) {
        free(nv_joueur);
        return NULL;
    }

    return nv_joueur;
}

void JoueurLiberer(tJoueur *pj) {
    if (pj != NULL && *pj != NULL) {
        InventaireLiberer(&(*pj)->inv);
        free(*pj);
        *pj = NULL;
    }
}

void JoueurPosition(const tJoueur j, int *px, int *py) {
    if (j == NULL || px == NULL || py == NULL) {
        return;
    }

    *px = j->x;
    *py = j->y;
}

int JoueurPV(const tJoueur j) {
    if (j == NULL) {
        return -1;
    }

    return j->pv;
}

int JoueurPVMax(const tJoueur j) {
    if (j == NULL) {
        return -1;
    }

    return j->pvMax;
}

int JoueurAttaque(const tJoueur j) {
    if (j == NULL) {
        return -1;
    }

    return j->atk;
}

int JoueurDefense(const tJoueur j) {
    if (j == NULL) {
        return -1;
    }

    return j->def;
}

tInventaire JoueurInventaire(const tJoueur j) {
    if (j == NULL) {
        return NULL;
    }

    return j->inv;
}

void JoueurMajPos(tJoueur j, int x, int y) {
    if (j == NULL) {
        return;
    }

    j->x = x;
    j->y = y;
}

void JoueurMajPV(tJoueur j, int pv) {
    if (j == NULL) {
        return;
    }

    j->pv = pv;
}