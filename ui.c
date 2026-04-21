#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

/* ==============================
   Structure privée
   ============================== */
struct sUI {
    char message[256];
};

/* ==============================
   Création UI
   ============================== */
tUI UI_Creer(void) {

    tUI ui = malloc(sizeof(struct sUI));
    if (!ui) return NULL;
    ui->message[0] = '\0';
    return ui;
}

/* ==============================
   Destruction UI
   ============================== */
void UI_Liberer(tUI *pui) {

    if (pui && *pui) {
        free(*pui);
        *pui = NULL;
    }
}

/* ==============================
   Message
   ============================== */
void UI_DefinirMessage(tUI ui, const char *fmt, ...) {

    if (!ui || !fmt) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(ui->message, sizeof(ui->message), fmt, ap);
    va_end(ap);
}

/* ---- Affichage mini carte ---- */
static void render_map(tDonjon d, tJoueur j) {

    // cadre + - |, intérieur w*h
    // règles: @, #, ?, ., E (niveau 1 => pas d’ennemis, donc jamais E)
    int w = DonjonW(d), h = DonjonH(d);
    int jx, jy;
    JoueurPosition(j, &jx, &jy);

    printf("%s+", CYAN);
    for (int x = 0; x < w; x++) printf("-");
    printf("+%s\n", RESET);

    for (int y = 0; y < h; y++) {
        printf("%s|%s", CYAN, RESET);
        for (int x = 0; x < w; x++) {
            tSalle s = DonjonSalle(d, x, y);
            char c = '?';
            if (jx == x && jy == y) {
                c = '@';
            } else if (!s || !SalleExiste(s)) {
                c = '#';
            } else if (!SalleEstVisitee(s)) {
                c = '?';
#ifndef NIV01
            } else if (SalleEnnemi(s) && EnnemiEstVivant(SalleEnnemi(s))) {
                c = 'E';
#endif
            } else {
                c = '.';
            }
            if (c == '@') {
                printf("%s%c%s", RED, c, RESET);
            }
            else if (c == '#') {
                printf("%s%c%s", GREEN, c, RESET);
            } else {
                printf("%c", c);
            }
        }
        printf("%s|%s\n", CYAN, RESET);
    }

    printf("%s+", CYAN);
    for (int x = 0; x < w; x++) printf("-");
    printf("+%s\n", RESET);
}

/* ---- Bloc salle ---- */
static void render_room(tDonjon d, tJoueur j) {

    int jx, jy;
    JoueurPosition(j, &jx, &jy);
    tSalle s = DonjonSalle(d, jx, jy);
    if (!s) {
        fprintf(stderr, "ERREUR: impossible de trouver la salle dans laquelle est le joueur.\n");
        return;
    }
    int id = jy * DonjonW(d) + jx;

    printf("ROOM [id=%d] (x=%d y=%d)\n", id, jx, jy);
    printf("Desc: %s\n", s ? SalleDescription(s) : "");

    // sorties NESW si case voisine existe
    printf("Exits:");
    int any = 0;
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0};
    const char *lab[4] = {" N", " E", " S", " W"};
    for (int k = 0; k < 4; k++) {
        tSalle v = DonjonSalle(d, jx + dx[k], jy + dy[k]);
        if (v && SalleExiste(v)) { printf("%s", lab[k]); any = 1; }
    }
    if (!any) printf(" (none)");
    printf("\n");

    // objets au sol
    char *txt = InventaireVersChaine(SalleObjets(s));
    printf("Items: %s\n", txt);
    free(txt);

    // ennemi
#ifndef NIV01
    if (!SalleEnnemi(s))
        printf("Enemy: aucun\n");
    else {
        tEnnemi e = SalleEnnemi(s);
        printf("Enemy: %s (pv=%d/%d, atk=%d, def=%d)\n",
            EnnemiNom(e), EnnemiPV(e), EnnemiPVMax(e), EnnemiAttaque(e), EnnemiDefense(e));
    }
#endif
}

/* ---- Bloc joueur ---- */
static void render_player(const tUI ui, const tJoueur j) {

    printf("PLAYER pv=%d/%d atk=%d def=%d\n",
        JoueurPV(j), JoueurPVMax(j), JoueurAttaque(j), JoueurDefense(j));

    char *txt = InventaireVersChaine(JoueurInventaire(j));
    printf("Inv: %s\n", txt);
    free(txt);
    printf("Msg: %s\n", ui->message[0] ? ui->message : "(none)");
}

/* ==============================
   Affichage global
   ============================== */
void UI_Afficher(tUI ui, tDonjon d, tJoueur j) {

    printf("==================== DUNGEON EXPLORER ====================\n");
    render_map(d, j);
    printf("\n");
    render_room(d, j);
    printf("\n");
    render_player(ui, j);
    printf("\n");
    printf("> ");
    fflush(stdout);
}

/* ==============================
   Lecture ligne
   ============================== */
char *UI_LireLigne(tUI ui) {

    (void) ui;
    // fgets dans buffer, puis allocation au plus juste
    char buf[256];
    if (!fgets(buf, sizeof(buf), stdin)) return NULL;

    size_t len = strlen(buf);
    if (len && buf[len-1] == '\n') buf[len-1] = '\0';

    char *s = malloc(strlen(buf) + 1);
    if (!s) return NULL;
    strcpy(s, buf);
    return s;
}
