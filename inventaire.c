#include "inventaire.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tInventaire InventaireCreer(void) {
    tInventaire inv = malloc(sizeof(struct sInventaire));

    if (inv == NULL) {
        return NULL;
    }

    inv->debut = NULL; // inventaire vide
    return inv;
}

void InventaireVider(tInventaire inv) {
    if (inv == NULL) {
        return;
    }

    while (inv->debut != NULL) {
        struct sItem *tmp = inv->debut; // pointe sur premier
        inv->debut = tmp->suiv; // on décale d'un cran
        free(tmp); // on libère l'ancien premier
    }
}


void InventaireLiberer(tInventaire *pinv) {
    if (pinv != NULL && *pinv != NULL) {
        InventaireVider(*pinv);
        free(*pinv);
        *pinv = NULL;
    }
}

int InventaireAjouter(tInventaire inv, const char *nom, int qte) {
    if (inv == NULL || nom == NULL || qte <= 0) {
        return 0;
    }

    struct sItem *tmp = inv->debut;
    while (tmp != NULL) {
        if (strcmp(tmp->nom, nom) == 0) { // objet trouvé
            tmp->quantite += qte;
            return 1;
        }
        tmp = tmp->suiv;
    }

    // si objet non trouvé
    struct sItem *nouveau = malloc(sizeof(struct sItem));
    if (nouveau == NULL) {
        return 0;
    }

    strcpy(nouveau->nom, nom);
    nouveau->quantite = qte;
    nouveau->suiv = inv->debut;
    inv->debut = nouveau; // nouveau en début de liste
    return 1;
}

int InventaireRetirer(tInventaire inv, const char *nom, int qte) {
    if (inv == NULL || nom == NULL || qte <= 0) {
        return 0;
    }

    struct sItem *tmp = inv->debut;
    struct sItem *prev = NULL; // pointe sur l'objet précédent

    while (tmp != NULL) {
        if (strcmp(tmp->nom, nom) == 0) { // objet trouvé
            tmp->quantite -= qte;

            if (tmp->quantite <= 0) { // si qte <= 0, on le supprime
                if (prev == NULL) {  // si premier objet
                    inv->debut = tmp->suiv;
                } else {
                    prev->suiv = tmp->suiv;
                }
                free(tmp);
            }
            return 1;
        }
        prev = tmp;
        tmp = tmp->suiv;
    }

    return 0;
}

int InventaireQuantite(const tInventaire inv, const char *nom) {
    if (inv == NULL || nom == NULL) {
        return 0;
    }

    struct sItem *tmp = inv->debut;
    while (tmp != NULL) {
        if (strcmp(tmp->nom, nom) == 0) { // objet trouvé
            return tmp->quantite;
        }
        tmp = tmp->suiv;
    }

    return 0;
}

int InventaireCompter(const tInventaire inv) {
    if (inv == NULL) {
        return 0;
    }

    int nb_objets = 0;
    struct sItem *tmp = inv->debut;
    while (tmp != NULL) {
        nb_objets++;
        tmp = tmp->suiv;
    }
    return nb_objets;
}

char *InventaireVersChaine(const tInventaire inv) {
    if (inv == NULL) {
        return NULL;
    }

    // si inventaire vide
    if (InventaireCompter(inv) == 0) {
        char *vide = malloc(5);
        if (vide == NULL) {
            return NULL;
        }
        strcpy(vide, "vide");
        return vide;
    }

    char *cont_inv = malloc(999);
    if (cont_inv == NULL) {
        return NULL;
    }

    cont_inv[0] = '\0';

    struct sItem *tmp = inv->debut;
    sprintf(cont_inv, "%s(%d)", tmp->nom, tmp->quantite);
    tmp = tmp->suiv;
    while (tmp != NULL) {
        strcat(cont_inv, ", ");
        char temp[50]; // buffer pour ajouter à cont_inv
        sprintf(temp, "%s(%d)", tmp->nom, tmp->quantite);
        strcat(cont_inv, temp);
        tmp = tmp->suiv;
    }

    return cont_inv;
}