#include "donjon.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tDonjon DonjonCreer(int w, int h) {
    if (w <= 0 || h <= 0) {
        return NULL;
    }

    tDonjon nv_donjon = malloc(sizeof(struct sDonjon));
    if (nv_donjon == NULL) {
        return NULL;
    }

    nv_donjon->w = w;
    nv_donjon->h = h;

    nv_donjon->cases = malloc(w * h * sizeof(tSalle));
    if (nv_donjon->cases == NULL) {
        free(nv_donjon);
        return NULL;
    }

    return nv_donjon;
}

void DonjonLiberer(tDonjon *pd) {
    if (pd == NULL || *pd == NULL) {
        return;
    }

    // libere toutes les salles
    int nb_salles = (*pd)->w * (*pd)->h;
    for (int i = 0; i < nb_salles; i++) {
        SalleLiberer(&(*pd)->cases[i]);
    }

    free((*pd)->cases);
    free(*pd);
    *pd = NULL;
}

int DonjonH(const tDonjon d) {
    if (d == NULL) {
        return -1;
    }

    return d->h;
}

int DonjonW(const tDonjon d) {
    if (d == NULL) {
        return -1;
    }

    return d->w;
}

tSalle DonjonSalle(const tDonjon d, int x, int y) {
    if (d == NULL || x < 0 || y < 0 || x >= d->w || y >= d->h) {
        return NULL;
    }

    return d->cases[y * d->w + x];
}

int DonjonCharger(const char *fichier, tDonjon *dj, int *departX, int *departY) {
    if (fichier == NULL || dj == NULL || departX == NULL || departY == NULL) {
        return 0;
    }

    // ouvre le fichier
    FILE *f = fopen(fichier, "r");
    if (f == NULL) {
        return 0;
    }

    // taille du donjon
    int w, h;
    fscanf(f, "%d %d", &w, &h);
    fscanf(f, "%*c");

    // cree le donjon
    *dj = DonjonCreer(w, h);
    if (*dj == NULL) {
        fclose(f);
        return 0;
    }

    // salles
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            char c;
            fscanf(f, "%c", &c);

            // ne pas prendre en compte espace et saut de ligne
            while (c == ' ' || c == '\n') {
                fscanf(f, "%c", &c);
            }

            if (c == '#') {
                tSalle nv_salle = SalleCreerMur();
                (*dj)->cases[y * w + x] = nv_salle;
            } else if (c == '.') {
                tSalle nv_salle = SalleCreerVide();
                (*dj)->cases[y * w + x] = nv_salle;  
            } else if (c == '@') {
                tSalle nv_salle = SalleCreerVide();
                (*dj)->cases[y * w + x] = nv_salle;
                SalleVisiter(nv_salle);

                *departX = x;
                *departY = y;
            }
        }
    }

    // directives : descriptions / items salles
    char directive[20];
    int x, y, qte;
    char desc[256];
    char nom[50];
    
    while (fscanf(f, "%s", directive) == 1) { // premier mot
        if (strcmp(directive, "DESC") == 0) { // si description
            fscanf(f, "%d %d", &x, &y);
            fscanf(f, "%[^\n]", desc);

            SalleMajDescription(DonjonSalle(*dj, x, y), desc);
        } else if (strcmp(directive, "ITEM") == 0) { // si item
            fscanf(f, "%d %d %s %d", &x, &y, nom, &qte);

            InventaireAjouter(SalleObjets(DonjonSalle(*dj, x, y)), nom, qte);
        }
    }

    fclose(f);
    return 1;
}