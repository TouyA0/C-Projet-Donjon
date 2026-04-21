#include "ui.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> // vsnprintf

/*
 * Version du jeu qui nécessite la bibliothèque "ncurses" :
 *     https://invisible-island.net/ncurses/
 *
 * ncurses on Ubuntu/Debian
 *   sudo apt install libncurses-dev
 * ncurses on Fedora Linux >= 22
 *   sudo dnf install ncurses-devel
 * ncurses on Mac OS X (with Homebrew https://brew.sh/)
 *   brew install ncurses
 */

/* ==============================
   Structure privée (ncurses)
   ============================== */
struct sUI {
    char message[256];

    WINDOW *winMap;
    WINDOW *winRoom;
    WINDOW *winPlayer;
    WINDOW *winInput;

    int screenW;
    int screenH;
};

/* ==============================
   Création UI
   ============================== */
tUI UI_Creer(void) {

    tUI ui = malloc(sizeof(struct sUI));
    if (!ui) return NULL;

    ui->message[0] = '\0';

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);     // joueur
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // salle visitée
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // ennemi
        init_pair(4, COLOR_BLUE, COLOR_BLACK);    // mur
    }

    getmaxyx(stdscr, ui->screenH, ui->screenW);

    int mapH = 12;
    int mapW = 14;

    ui->winMap = newwin(mapH, mapW, 1, 1);
    ui->winRoom = newwin(6, ui->screenW - mapW - 4, 1, mapW + 2);
    ui->winPlayer = newwin(6, ui->screenW - mapW - 4, 7, mapW + 2);
    ui->winInput = newwin(3, ui->screenW - 2, ui->screenH - 3, 1);
    keypad(ui->winInput, TRUE);

    return ui;
}

/* ==============================
   Destruction UI
   ============================== */
void UI_Liberer(tUI *pui) {

    if (pui == NULL || *pui == NULL) return;

    delwin((*pui)->winMap);
    delwin((*pui)->winRoom);
    delwin((*pui)->winPlayer);
    delwin((*pui)->winInput);

    endwin();

    free(*pui);
    *pui = NULL;
}

/* ==============================
   Message
   ============================== */
void UI_DefinirMessage(tUI ui, const char *fmt, ...) {

    if (!ui) return;

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(ui->message, sizeof(ui->message), fmt, ap);
    va_end(ap);
}

/* ---- Affichage mini carte ---- */
static void afficherCarte(tUI ui, tDonjon d, tJoueur j) {

    werase(ui->winMap);
    box(ui->winMap, 0, 0);
    int jx, jy;
    JoueurPosition(j, &jx, &jy);

    for (int y = 0; y < DonjonH(d); y++) {
        for (int x = 0; x < DonjonW(d); x++) {

            tSalle s = DonjonSalle(d, x, y);
            char c = '?';

            if (jx == x && jy == y)
                c = '@';
            else if (!s || !SalleExiste(s))
                c = '#';
            else if (!SalleEstVisitee(s))
                c = '?';
#ifndef NIV01
            else if (SalleEnnemi(s) && EnnemiEstVivant(SalleEnnemi(s)))
                c = 'E';
#endif
            else
                c = '.';

            if (c == '@') {
                wattron(ui->winMap, COLOR_PAIR(1) | A_BOLD);
                mvwaddch(ui->winMap, y + 1, x + 1, c);
                wattroff(ui->winMap, COLOR_PAIR(1) | A_BOLD);
            } else if (c == '.') {
                wattron(ui->winMap, COLOR_PAIR(2));
                mvwaddch(ui->winMap, y + 1, x + 1, c);
                wattroff(ui->winMap, COLOR_PAIR(2));
            } else if (c == '#') {
                wattron(ui->winMap, COLOR_PAIR(4));
                mvwaddch(ui->winMap, y + 1, x + 1, c);
                wattroff(ui->winMap, COLOR_PAIR(4));
            } else if (c == 'E') {
                wattron(ui->winMap, COLOR_PAIR(3));
                mvwaddch(ui->winMap, y + 1, x + 1, c);
                wattroff(ui->winMap, COLOR_PAIR(3));
            } else {                
                mvwaddch(ui->winMap, y + 1, x + 1, c);
            }
        }
    }

    wrefresh(ui->winMap);
}

/* ---- Bloc salle ---- */
static void afficherSalle(tUI ui, tDonjon d, tJoueur j) {

    werase(ui->winRoom);
    box(ui->winRoom, 0, 0);

    int jx, jy;
    JoueurPosition(j, &jx, &jy);
    tSalle s = DonjonSalle(d, jx, jy);

    mvwprintw(ui->winRoom, 1, 2, "ROOM (%d,%d)", jx, jy);
    mvwprintw(ui->winRoom, 2, 2, "Desc: %s", SalleDescription(s));

    // objets au sol
    char *txt = InventaireVersChaine(SalleObjets(s));
    mvwprintw(ui->winRoom, 3, 2, "Items: %s", txt);
    free(txt);
    
    // ennemi
#ifndef NIV01
    if (!SalleEnnemi(s))
        mvwprintw(ui->winRoom, 4, 2, "Enemy: aucun");
    else {
        tEnnemi e = SalleEnnemi(s);
        mvwprintw(ui->winRoom, 4, 2, "Enemy: %s (pv=%d/%d, atk=%d, def=%d)",
            EnnemiNom(e), EnnemiPV(e), EnnemiPVMax(e), EnnemiAttaque(e), EnnemiDefense(e));
    }
#endif

    wrefresh(ui->winRoom);
}

/* ---- Bloc joueur ---- */
static void afficherJoueur(tUI ui, tJoueur j) {

    werase(ui->winPlayer);
    box(ui->winPlayer, 0, 0);

    mvwprintw(ui->winPlayer, 1, 2,
        "PLAYER pv=%d/%d atk=%d def=%d",
        JoueurPV(j), JoueurPVMax(j), JoueurAttaque(j), JoueurDefense(j));

    char *txt = InventaireVersChaine(JoueurInventaire(j));
    mvwprintw(ui->winPlayer, 2, 2, "Inv: %s", txt);
    free(txt);

    mvwprintw(ui->winPlayer, 3, 2, "Msg: %s",
        ui->message[0] ? ui->message : "(none)");

    wrefresh(ui->winPlayer);
}

/* ==============================
   Affichage global
   ============================== */
void UI_Afficher(tUI ui, tDonjon d, tJoueur j) {

    if (!ui) return;

    afficherCarte(ui, d, j);
    afficherSalle(ui, d, j);
    afficherJoueur(ui, j);

    werase(ui->winInput);
    box(ui->winInput, 0, 0);
    mvwprintw(ui->winInput, 1, 2, "> ");
    wrefresh(ui->winInput);
}

/* ==============================
   Lecture ligne
   ============================== */
static char *direction(char c)
{
    char *r = malloc(2);
    if (!r) return NULL;
    r[0] = c;
    r[1] = '\0';
    return r;
}

char *UI_LireLigne(tUI ui) {

    if (!ui) return NULL;

    char buffer[256];
    int len = 0;
    buffer[0] = '\0';

    werase(ui->winInput);
    box(ui->winInput, 0, 0);
    mvwprintw(ui->winInput, 1, 2, "> ");
    wmove(ui->winInput, 1, 4);
    wrefresh(ui->winInput);

    curs_set(1);
    noecho();

    while (1) {
        int ch = wgetch(ui->winInput);

        switch (ch) {
            case KEY_UP:
                return direction('n');

            case KEY_DOWN:
                return direction('s');

            case KEY_LEFT:
                return direction('w');

            case KEY_RIGHT:
                return direction('e');

            case '\n':
            case '\r':
            case KEY_ENTER:
            {
                buffer[len] = '\0';
                char *res = malloc((size_t)len + 1);
                if (!res) return NULL;
                strcpy(res, buffer);
                return res;
            }

            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (len > 0) {
                    len--;
                    buffer[len] = '\0';
                    mvwprintw(ui->winInput, 1, 4, "%-*s", 250, buffer);
                    wmove(ui->winInput, 1, 4 + len);
                    wrefresh(ui->winInput);
                }
                break;

            default:
                if (ch >= 32 && ch <= 126) {
                    if (len < 255) {
                        buffer[len++] = (char)ch;
                        buffer[len] = '\0';
                        mvwprintw(ui->winInput, 1, 4, "%s", buffer);
                        wmove(ui->winInput, 1, 4 + len);
                        wrefresh(ui->winInput);
                    }
                }
                break;
        }
    }
}