#ifndef JEU_H
#define JEU_H

#include "donjon.h"
#include "joueur.h"
#include "ui.h"

struct sJeu {
tDonjon d ; // donjon
tJoueur j ; // joueur
tUI ui ; // interface utilisateur
int fini ; // fin du jeu si la valeur est à 1
};

struct sJeu *JeuCreer(const char *fichierDonjon);

void JeuLiberer(struct sJeu *g);

void JeuJouer(struct sJeu *g);

#endif