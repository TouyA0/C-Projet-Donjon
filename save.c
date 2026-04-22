#include "save.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int ecrire_objet(const char *nom, int qte, void *fich) {
    if (nom == NULL || fich == NULL) {
        return 0;
    }

    // on ouvre pas le fichier, on traite le pointeur comme un FILE
    FILE *f = (FILE *)fich;
    
    // ecrire nom
    fwrite(nom, sizeof(char), NOM_MAX, f);
    
    // ecrire quantite
    fwrite(&qte, sizeof(int), 1, f);
    
    return 1;
}

int SaveEcrire(const char *fichier, tDonjon d, tJoueur j) {
    if (fichier == NULL || d == NULL || j == NULL) {
        return 0;
    }

    // ouvre fichier
    FILE *f = fopen(fichier, "wb");
    if (f == NULL) {
        return 0;
    }

    // header
    fwrite("DNG1", sizeof(char), 4, f);
    int version = 1;
    fwrite(&version, sizeof(int), 1, f);
    int w = DonjonW(d);
    fwrite(&w, sizeof(int), 1, f);
    int h = DonjonH(d);
    fwrite(&h, sizeof(int), 1, f);

    // joueur
    int x, y;
    JoueurPosition(j, &x, &y);
    fwrite(&x, sizeof(int), 1, f);
    fwrite(&y, sizeof(int), 1, f);
    int pv = JoueurPV(j);
    fwrite(&pv, sizeof(int), 1, f);

    // inventaire
    tInventaire inv = JoueurInventaire(j);
    int nb_objets_inventaire = InventaireCompter(inv);
    fwrite(&nb_objets_inventaire, sizeof(int), 1, f);
    if (nb_objets_inventaire > 0) {
        InventaireVisiter(inv, ecrire_objet, f);
    }

    // salles (une par une)
    for (int i = 0; i < w * h; i++) {
        tSalle salle = d->cases[i];
        
        // salle existe ou non (on met unsigned char et pas int pour que ça soit sur 1 byte -> prend moins de place)
        unsigned char existe = SalleExiste(salle) ? 1 : 0;
        fwrite(&existe, sizeof(unsigned char), 1, f);
        
        if (existe == 1) {
            // salle visitée ou non
            unsigned char visitee = SalleEstVisitee(salle) ? 1 : 0;
            fwrite(&visitee, sizeof(unsigned char), 1, f);
            
            // objets de la salle un par un + quantité respective
            int nb_objets = InventaireCompter(SalleObjets(salle));
            fwrite(&nb_objets, sizeof(int), 1, f);
            if (nb_objets > 0) {
                InventaireVisiter(SalleObjets(salle), ecrire_objet, f);
            }
            
            // ennemi ou non
            tEnnemi ennemi = SalleEnnemi(salle);
            unsigned char a_ennemi = (ennemi != NULL) ? 1 : 0;
            fwrite(&a_ennemi, sizeof(unsigned char), 1, f);
            
            if (a_ennemi == 1) {
                fwrite(EnnemiNom(ennemi), sizeof(char), NOM_MAX, f);
                int pv_ennemi = EnnemiPV(ennemi);
                fwrite(&pv_ennemi, sizeof(int), 1, f);
                int pvMax = EnnemiPVMax(ennemi);
                fwrite(&pvMax, sizeof(int), 1, f);
                int atk = EnnemiAttaque(ennemi);
                fwrite(&atk, sizeof(int), 1, f);
                int def = EnnemiDefense(ennemi);
                fwrite(&def, sizeof(int), 1, f);
            }
        }
    }

    fclose(f);
    return 1;
}

int SaveLire(const char *fichier, tDonjon d, tJoueur j) {
    if (fichier == NULL || d == NULL || j == NULL) {
        return 0;
    }

    // ouvre fichier
    FILE *f = fopen(fichier, "rb");
    if (f == NULL) {
        return 0;
    }

    // header
    char header[4];
    fread(header, sizeof(char), 4, f);
    if (strcmp(header, "DNG1") != 0) {
        fclose(f);
        return 0;
    }
    int version;
    fread(&version, sizeof(int), 1, f);
    int w, h;
    fread(&w, sizeof(int), 1, f);
    fread(&h, sizeof(int), 1, f);
    // verif taille donjon
    if (w != DonjonW(d) || h != DonjonH(d)) {
        fclose(f);
        return 0;
    }

    // joueur
    int x, y, pv;
    fread(&x, sizeof(int), 1, f);
    fread(&y, sizeof(int), 1, f);
    if (x < 0 || x >= w || y < 0 || y >= h) {
        fclose(f);
        return 0;
    }
    JoueurMajPos(j, x, y);
    fread(&pv, sizeof(int), 1, f);
    if (pv < 0 || pv > JoueurPVMax(j)) {
        fclose(f);
        return 0;
    }
    JoueurMajPV(j, pv);

    // vider inventaire du joueur
    InventaireVider(JoueurInventaire(j));
    // vider tous les objets des salles
    for (int i = 0; i < w * h; i++) {
        int sx = i % w;  // coordonne x de la salle
        int sy = i / w;  // coordonne y de la salle
        InventaireVider(SalleObjets(DonjonSalle(d, sx, sy)));
    }

    // inventaire
    int nb_objets_inventaire;
    fread(&nb_objets_inventaire, sizeof(int), 1, f);
    for (int k = 0; k < nb_objets_inventaire; k++) {
        char nom_lu[NOM_MAX];
        int qte_lu;
        fread(nom_lu, sizeof(char), NOM_MAX, f); // 32 bytes (le nom)
        fread(&qte_lu, sizeof(int), 1, f); // 4 bytes (quantité)
        InventaireAjouter(JoueurInventaire(j), nom_lu, qte_lu);
    }

    // salles une par une
    for (int i = 0; i < w * h; i++) {
        tSalle salle = d->cases[i];
        
        // salle existe ou non
        unsigned char existe;
        fread(&existe, sizeof(unsigned char), 1, f);
        if (existe == 1) {
            // salle visitée ou non
            unsigned char visitee;
            fread(&visitee, sizeof(unsigned char), 1, f);
            if (visitee == 1) {
                SalleVisiter(salle);
            }
            
            // objets de la salle un par un + quantité respective
            int nb_objets;
            fread(&nb_objets, sizeof(int), 1, f);
            for (int k = 0; k < nb_objets; k++) {
                char nom_lu[NOM_MAX];
                int qte_lu;
                fread(nom_lu, sizeof(char), NOM_MAX, f); // 32 bytes (le nom)
                fread(&qte_lu, sizeof(int), 1, f); // 4 bytes (quantité)
                InventaireAjouter(SalleObjets(salle), nom_lu, qte_lu);
            }
            
            // ennemi ou non
            unsigned char a_ennemi;
            fread(&a_ennemi, sizeof(unsigned char), 1, f);
            if (a_ennemi == 1) {
                char nom[NOM_MAX];
                fread(nom, sizeof(char), NOM_MAX, f);
                int pv, pvMax, atk, def;
                fread(&pv, sizeof(int), 1, f);
                fread(&pvMax, sizeof(int), 1, f);
                fread(&atk, sizeof(int), 1, f);
                fread(&def, sizeof(int), 1, f);

                tEnnemi ennemi = EnnemiCreer(nom, pvMax, atk, def);
                EnnemiMajPV(ennemi, pv);
                SalleAjouterEnnemi(salle, ennemi);
            }
        }
    }

    fclose(f);
    return 1;
}