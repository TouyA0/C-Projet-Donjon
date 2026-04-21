#include "ui.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    tDonjon donjon = NULL;
    tJoueur joueur = NULL;
    int departX, departY;
    
    // donjon
    if (!DonjonCharger("donjon64.txt", &donjon, &departX, &departY)) {
        fprintf(stderr, "Erreur : impossible de charger le donjon\n");
        return 1;
    }

    // joueur
    joueur = JoueurCreer(departX, departY);
    if (joueur == NULL) {
        fprintf(stderr, "Erreur : impossible de créer le joueur\n");
        DonjonLiberer(&donjon);
        return 1;
    }

    // interface ui
    tUI ui = UI_Creer();
    if (ui == NULL) {
        fprintf(stderr, "Erreur : impossible de créer l'interface\n");
        JoueurLiberer(&joueur);
        DonjonLiberer(&donjon);
        return 1;
    }
    UI_Afficher(ui, donjon, joueur);

    // libérer
    UI_Liberer(&ui);
    JoueurLiberer(&joueur);
    DonjonLiberer(&donjon);

    return 0;
}