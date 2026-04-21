#ifndef DONJON_H
#define DONJON_H

#include "salle.h"

// structure privée donjon
struct sDonjon {
    int w, h ; // taille du donjon (w largeur, h hauteur )
    tSalle * cases ; // tableau dynamique de taille w * h des salles du donjon
};

// Type public ( pointeur vers structure privée)
typedef struct sDonjon * tDonjon ;

tDonjon DonjonCreer(int w, int h);

void DonjonLiberer(tDonjon *pd);

int DonjonH(const tDonjon d);

int DonjonW(const tDonjon d);

tSalle DonjonSalle(const tDonjon d, int x, int y);

int DonjonCharger(const char *fichier, tDonjon *dj, int *departX, int *departY);

#endif