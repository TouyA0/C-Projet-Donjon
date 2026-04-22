#include "jeu.h"
#include "save.h"
#include "explore.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// déclarations privées
static void execute_command(struct sJeu *g, char *ligne_cmd);
static void cmd_help(struct sJeu *g);
static int cmd_move(struct sJeu *g, int dx, int dy);
static void cmd_take(struct sJeu *g, const char *name);
static void cmd_drop(struct sJeu *g, const char *name);
static void cmd_fight(struct sJeu *g);

struct sJeu *JeuCreer(const char *fichierDonjon) {
    if (fichierDonjon == NULL) {
        return NULL;
    }

    struct sJeu *nv_jeu = malloc(sizeof(struct sJeu));
    if (nv_jeu == NULL) {
        return NULL;
    }

    int departX, departY;
    
    // donjon
    if (!DonjonCharger(fichierDonjon, &nv_jeu->d, &departX, &departY)) {
        free(nv_jeu);
        return NULL;
    }

    // joueur
    nv_jeu->j = JoueurCreer(departX, departY);
    if (nv_jeu->j == NULL) {
        DonjonLiberer(&nv_jeu->d);
        free(nv_jeu);
        return NULL;
    }

    // interface ui
    nv_jeu->ui = UI_Creer();
    if (nv_jeu->ui == NULL) {
        JoueurLiberer(&nv_jeu->j);
        DonjonLiberer(&nv_jeu->d);
        free(nv_jeu);
        return NULL;
    }
    UI_DefinirMessage(nv_jeu->ui, "Bienvenue ! Tapez help pour avoir la liste des commandes");

    nv_jeu->fini = 0;

    return nv_jeu;
}

void JeuLiberer(struct sJeu *g) {
    if (g != NULL) {
        UI_Liberer(&g->ui);
        JoueurLiberer(&g->j);
        DonjonLiberer(&g->d);
        free(g);
    }
}

void JeuJouer(struct sJeu *g) {
    if (g == NULL) {
        return;
    }

    while (g->fini != 1) {
        // affichage
        UI_Afficher(g->ui, g->d, g->j);
        
        // cmd
        char *ligne_cmd = UI_LireLigne(g->ui);
        if (ligne_cmd == NULL) {
            break;
        }
        execute_command(g, ligne_cmd);

        free(ligne_cmd);
    }
}

void execute_command(struct sJeu *g, char *ligne_cmd) {
    if (g == NULL || ligne_cmd == NULL) {
        return;
    }

    // recup premier mot
    char *cmd = strtok(ligne_cmd, " ");
    if (cmd == NULL) {
        return;
    }

    if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
        g->fini = 1;
    } else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "h") == 0) {
        cmd_help(g);
    } else if (strcmp(cmd, "n") == 0) {
        cmd_move(g, 0, -1);
    } else if (strcmp(cmd, "e") == 0) {
        cmd_move(g, 1, 0);
    } else if (strcmp(cmd, "s") == 0) {
        cmd_move(g, 0, 1);
    } else if (strcmp(cmd, "w") == 0) {
        cmd_move(g, -1, 0);
    } else if (strcmp(cmd, "take") == 0) {
        char *obj = strtok(NULL, " "); // recup deuxieme mot
        cmd_take(g, obj);
    } else if (strcmp(cmd, "drop") == 0) {
        char *obj = strtok(NULL, " "); // recup deuxieme mot
        cmd_drop(g, obj);
    } else if (strcmp(cmd, "fight") == 0 || strcmp(cmd, "f") == 0) {
        cmd_fight(g);
    } else if (strcmp(cmd, "save") == 0) {
        char *fichier = strtok(NULL, " "); // recup nom fichier
        if (fichier == NULL) {
            UI_DefinirMessage(g->ui, "Usage: save <fichier>");
        } else {
            if (SaveEcrire(fichier, g->d, g->j)) {
                UI_DefinirMessage(g->ui, "Partie sauvegardee dans %s", fichier);
            } else {
                UI_DefinirMessage(g->ui, "Erreur lors de la sauvegarde");
            }
        }
    } else if (strcmp(cmd, "load") == 0) {
        char *fichier = strtok(NULL, " "); // recup nom fichier 
        if (fichier == NULL) {
            UI_DefinirMessage(g->ui, "Usage: load <fichier>");
        } else {
            if (SaveLire(fichier, g->d, g->j)) {
                UI_DefinirMessage(g->ui, "Partie chargee depuis %s", fichier);
            } else {
                UI_DefinirMessage(g->ui, "Erreur lors du chargement");
            }
        }
    } else if (strcmp(cmd, "cheat") == 0) {
        char *fichier = strtok(NULL, " "); // recup nom fichier 
        if (fichier == NULL) {
            UI_DefinirMessage(g->ui, "Usage: cheat <tracefile>");
        } else {
            if (ExploreTrouverTresor(g->d, g->j, fichier)) {
                UI_DefinirMessage(g->ui, "Tresor trouve ! Trace: %s", fichier);
            } else {
                UI_DefinirMessage(g->ui, "Tresor non trouve. Trace: %s", fichier);
            }
        }
    } else {
        UI_DefinirMessage(g->ui, "Commande inconnue");
    }
}

void cmd_help(struct sJeu *g) {
    UI_DefinirMessage(g->ui, "Commandes: n/e/s/w | take <obj> | drop <obj> | fight | save <fichier> | load <fichier> | cheat <tracefile> | quit | help");
}

int cmd_move(struct sJeu *g, int dx, int dy) {
    if (g == NULL) {
        return 0;
    }

    // recup pos actuelle
    int x, y;
    JoueurPosition(g->j, &x, &y);
    
    // nouvelle pos
    int nx = x + dx;
    int ny = y + dy;
    
    // verif si salle existe et si pas un mur
    tSalle salle = DonjonSalle(g->d, nx, ny);
    if (salle == NULL || !SalleExiste(salle)) {
        UI_DefinirMessage(g->ui, "Deplacement impossible");
        return 0;
    }
    
    JoueurMajPos(g->j, nx, ny);
    SalleVisiter(salle);
    
    return 1;
}

void cmd_take(struct sJeu *g, const char *name) {
    if (g == NULL || name == NULL) {
        return;
    }

    // recup salle actuelle
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tSalle salle = DonjonSalle(g->d, x, y);
    if (salle == NULL) {
        return;
    }

    // verif si objet dans salle
    if (InventaireQuantite(SalleObjets(salle), name) <= 0) {
        UI_DefinirMessage(g->ui, "Cet objet n'existe pas ici");
        return;
    }

    // retire l'objet de la salle et l'ajoute au joueur
    if (!(InventaireRetirer(SalleObjets(salle), name, 1) && InventaireAjouter(JoueurInventaire(g->j), name, 1))) {
        UI_DefinirMessage(g->ui, "Erreur lors de l’ajout à l’inventaire");
    }
}

void cmd_drop(struct sJeu *g, const char *name) {
    if (g == NULL || name == NULL) {
        return;
    }

    // recup salle actuelle
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tSalle salle = DonjonSalle(g->d, x, y);
    if (salle == NULL) {
        return;
    }

    // verif si joueur a objet
    if (InventaireQuantite(JoueurInventaire(g->j), name) <= 0) {
        UI_DefinirMessage(g->ui, "Le joueur ne possède pas cet item");
        return;
    }

    // retire l'objet de l'inv du joueur et l'ajoute à la salle
    if (!(InventaireRetirer(JoueurInventaire(g->j), name, 1) && InventaireAjouter(SalleObjets(salle), name, 1))) {
        UI_DefinirMessage(g->ui, "Erreur lors de l’ajout dans la salle");
    }
}

void cmd_fight(struct sJeu *g) {
    if (g == NULL) {
        return;
    }

    // recup salle actuelle
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tSalle salle = DonjonSalle(g->d, x, y);
    if (salle == NULL) {
        return;
    }
    
    tEnnemi ennemi = SalleEnnemi(salle);

    // si aucun ennemi dans la salle
    if (ennemi == NULL || !EnnemiEstVivant(ennemi)) {
        UI_DefinirMessage(g->ui, "Aucun ennemi ici");
        return;
    }

    // joueur attaque ennemi
    int degats_joueur = EnnemiSubirAttaque(ennemi, g->j);

    // si ennemi vivant
    if (EnnemiEstVivant(ennemi)) {
        int degats_ennemi = EnnemiAttaquerJoueur(ennemi, g->j);
        
        if (JoueurPV(g->j) > 0) {
            UI_DefinirMessage(g->ui, "Vous infligez %d degats. L'ennemi riposte (%d).", degats_joueur, degats_ennemi);
        } else {
            UI_DefinirMessage(g->ui, "Vous infligez %d degats. L'ennemi riposte (%d). Vous etes mort.", degats_joueur, degats_ennemi);
        }
    } else { // si ennemi mort
        UI_DefinirMessage(g->ui, "Vous infligez %d degats. L'ennemi est mort.", degats_joueur);

        // retirer ennemi de la salle
        EnnemiLiberer(&ennemi);
        SalleAjouterEnnemi(salle, NULL);
    }
}