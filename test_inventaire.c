#include <stdio.h>
#include <stdlib.h>
#include "inventaire.h"

int main(void) {
    printf("=== TEST MODULE INVENTAIRE ===\n\n");

    printf("1. Creation d'un inventaire vide\n");
    tInventaire inv = InventaireCreer();

    printf("\n2. Ajout d'objets\n");
    InventaireAjouter(inv, "Potion de soin", 3);
    InventaireAjouter(inv, "Tresor", 1);
    InventaireAjouter(inv, "Cle", 5);
    printf("3 objets ajoutes\n");

    printf("\n4. Affichage de l'inventaire avant retrait\n");
    char *str = InventaireVersChaine(inv);
    printf("Inventaire : %s\n", str);
    free(str);

    printf("\n3. Retrait d'objets\n");
    InventaireRetirer(inv, "Tresor", 2);
    InventaireRetirer(inv, "Cle", 2);
    printf("Retrait effectue\n");

    printf("\n5. Test des quantites\n");
    printf("Quantite de 'Potion de soin' : %d\n", InventaireQuantite(inv, "Potion de soin"));
    printf("Quantite de 'Tresor' : %d\n", InventaireQuantite(inv, "Tresor"));
    printf("Quantite de 'Cle' : %d\n", InventaireQuantite(inv, "Cle"));
    printf("Nombre d'objets distincts : %d\n", InventaireCompter(inv));

    printf("\n4. Affichage de l'inventaire apres retrait\n");
    str = InventaireVersChaine(inv);
    printf("Inventaire : %s\n", str);
    free(str);

    printf("\n6. Vidage et liberation\n");
    InventaireVider(inv);
    str = InventaireVersChaine(inv);
    printf("Apres vidage : %s\n", str);
    free(str);
    InventaireLiberer(&inv);
    printf("Inventaire libere\n");

    printf("\n=== FIN DES TESTS ===\n");
    return 0;
}