#ifndef SALLE_H
#define SALLE_H

#include "inventaire.h"

// Structure privée d’une salle
struct sSalle {
    int existe ; // 0 = mur , 1 = salle
    int visitee ; // 0 = non , 1 = oui
    char * desc ; // description ( allouée dynamiquement )
    tInventaire objets ; // liste d’objets au sol sous la forme d’un inventaire
};

// Type public ( pointeur vers structure privée)
typedef struct sSalle * tSalle ;

tSalle SalleCreerMur(void);

tSalle SalleCreerVide(void);

void SalleLiberer(tSalle *ps);

int SalleExiste(const tSalle s);

int SalleEstVisitee(const tSalle s);

const char *SalleDescription(const tSalle s);

tInventaire SalleObjets(const tSalle s);

int SalleMajDescription(tSalle s, const char *desc);

void SalleVisiter(tSalle s);

#endif