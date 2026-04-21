#include "jeu.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_donjon>\n", argv[0]);
        return 1;
    }

    // créer jeu
    struct sJeu *jeu = JeuCreer(argv[1]);
    if (jeu == NULL) {
        fprintf(stderr, "Erreur : impossible de créer le jeu\n");
        return 1;
    }

    JeuJouer(jeu);

    JeuLiberer(jeu);

    return 0;
}