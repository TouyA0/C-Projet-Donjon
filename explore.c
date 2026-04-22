#include "explore.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int explorerRec(tDonjon d, tJoueur j, int x, int y, unsigned char *visite, FILE *trace);

int ExploreTrouverTresor(tDonjon d, tJoueur j, const char *fichierTrace) {
    if (d == NULL || j == NULL || fichierTrace == NULL) {
        return -1;
    }

    int w = DonjonW(d);
    int h = DonjonH(d);
    
    // matrice salles visitées
    unsigned char *visite = malloc(w * h * sizeof(unsigned char));
    if (visite == NULL) {
        return -1;
    }
    // init à 0
    for (int i = 0; i < w * h; i++) {
        visite[i] = 0;
    }
    
    // recup pos actuelle
    int x, y;
    JoueurPosition(j, &x, &y);
    
    // ouvre fichier
    FILE *f = fopen(fichierTrace, "w");
    if (f == NULL) {
        free(visite);
        return -1;
    }
    
    fprintf(f, "START %d %d\n", x, y);

    // parcours donjon
    int trouve = explorerRec(d, j, x, y, visite, f);

    // pos finale joueur
    JoueurPosition(j, &x, &y);

    fprintf(f, "END %d %d\n", x, y);
    fprintf(f, "FOUND %d\n", trouve);

    fclose(f);
    free(visite);

    return 0;
}

static int explorerRec(tDonjon d, tJoueur j, int x, int y, unsigned char *visite, FILE *f) {
    if (d == NULL || j == NULL || visite == NULL || f == NULL) {
        return -1;
    }
    
    int w = DonjonW(d);
    int h = DonjonH(d);
    if (x < 0 || x >= w || y < 0 || y >= h) {
        return -1;
    }
    
    tSalle salle = DonjonSalle(d, x, y);
    
    // verif salle existe et pas deja visitée
    if (salle == NULL || !SalleExiste(salle) || visite[y * w + x]) {
        return -1;
    }
    
    // Marquer la salle comme visitée
    visite[y * w + x] = 1;
    SalleVisiter(salle);
    
    JoueurMajPos(j, x, y);
    
    // verif tresor ici
    if (InventaireQuantite(SalleObjets(salle), "tresor") > 0) {
        return 1;
    }
    
    // appel recursif de chaque direction
    int old_x = x, old_y = y; // pos pour retour arrière
    
    // nord
    if (explorerRec(d, j, x, y - 1, visite, f)) {
        fprintf(f, "N\n");
        return 1;
    }
    fprintf(f, "N (BACK)\n");
    JoueurMajPos(j, old_x, old_y);
    
    // est
    if (explorerRec(d, j, x + 1, y, visite, f)) {
        fprintf(f, "E\n");
        return 1;
    }
    fprintf(f, "E (BACK)\n");
    JoueurMajPos(j, old_x, old_y);
    
    // sud
    if (explorerRec(d, j, x, y + 1, visite, f)) {
        fprintf(f, "S\n");
        return 1;
    }
    fprintf(f, "S (BACK)\n");
    JoueurMajPos(j, old_x, old_y);
    
    // ouest
    if (explorerRec(d, j, x - 1, y, visite, f)) {
        fprintf(f, "W\n");
        return 1;
    }
    fprintf(f, "W (BACK)\n");
    JoueurMajPos(j, old_x, old_y);
    
    return 0;
}