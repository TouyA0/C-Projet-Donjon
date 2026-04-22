#ifndef SAVE_H
#define SAVE_H

#include "donjon.h"
#include "joueur.h"

int SaveEcrire(const char *fichier, tDonjon d, tJoueur j);

int SaveLire(const char *fichier, tDonjon d, tJoueur j);

#endif