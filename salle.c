#include "salle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tSalle SalleCreerMur(void) {
    tSalle nv_salle = malloc(sizeof(struct sSalle));
    if (nv_salle == NULL) {
        return NULL;
    }


    nv_salle->existe = 0;
    nv_salle->visitee = 0;
    nv_salle->desc = NULL;
    nv_salle->objets = NULL;

    return nv_salle;
}

tSalle SalleCreerVide(void) {
    tSalle nv_salle = malloc(sizeof(struct sSalle));
    if (nv_salle == NULL) {
        return NULL;
    }

    nv_salle->existe = 1;
    nv_salle->visitee = 0;

    nv_salle->desc = malloc(100); // taille pas spécifié
    if (nv_salle->desc == NULL) {
        free(nv_salle);
        return NULL;
    }
    strcpy(nv_salle->desc, "Une salle sans particularite.");

    nv_salle->objets = InventaireCreer();
    if (nv_salle->objets == NULL) {
        free(nv_salle->desc);
        free(nv_salle);
        return NULL;
    }

    return nv_salle;
}

void SalleLiberer(tSalle *ps) {
    if (ps == NULL || *ps == NULL) {
        return;
    }

    // si pas un mur -> libérer inventaire + description
    if ((*ps)->existe == 1) {
        InventaireLiberer(&(*ps)->objets);
        free((*ps)->desc);
    }

    // libérer la salle
    free(*ps);
    *ps = NULL;
}

int SalleExiste(const tSalle s) {
    if (s == NULL) {
        return -1;
    }

    return s->existe;
}

int SalleEstVisitee(const tSalle s) {
    if (s == NULL) {
        return -1;
    }

    return s->visitee;
}

const char *SalleDescription(const tSalle s) {
    if (s == NULL) {
        return NULL;
    }

    return s->desc;
}

tInventaire SalleObjets(const tSalle s) {
    if (s == NULL) {
        return NULL;
    }

    return s->objets;
}

int SalleMajDescription(tSalle s, const char *desc) {
    if (s == NULL || desc == NULL) {
        return 0;
    }

    // libère ancienne desc
    if (s->desc != NULL) {
        free(s->desc);
    }

    // nouvelle desc
    s->desc = malloc(strlen(desc) + 1);
    if (s->desc == NULL) {
        return 0;
    }

    strcpy(s->desc, desc);
    return 1;
}

void SalleVisiter(tSalle s) {
    if (s != NULL) {
        s->visitee = 1;
    }
}