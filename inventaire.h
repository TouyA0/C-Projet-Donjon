#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#define NOM_MAX 32

// Structure privée d’un objet de l’inventaire
struct sItem {
    char nom[NOM_MAX ]; // chaîne de caractères , taille statique NOM_MAX =32
    int quantite;
    struct sItem *suiv;
};

// Structure privée d’un inventaire (liste chaînée)
struct sInventaire {
    struct sItem *debut;
};

// Type public représentant un inventaire (pointeur vers structure privée)
typedef struct sInventaire *tInventaire;

tInventaire InventaireCreer(void);

void InventaireVider(tInventaire inv);

void InventaireLiberer(tInventaire *pinv);

int InventaireAjouter(tInventaire inv, const char *nom, int qte);

int InventaireRetirer(tInventaire inv, const char *nom, int qte);

int InventaireQuantite(const tInventaire inv, const char *nom);

int InventaireCompter(const tInventaire inv);

char *InventaireVersChaine(const tInventaire inv);

int InventaireVisiter (const tInventaire inv, int (*visiter)(const char *nom, int quantite, void *contexte), void *contexte);

#endif