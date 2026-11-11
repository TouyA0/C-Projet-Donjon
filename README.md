# C-Projet-Donjon

Projet universitaire (Licence Informatique, Université Toulouse III) : jeu d'exploration de donjon en terminal, écrit en C avec ncurses.

## Fonctionnalités

- Chargement d'un donjon depuis un fichier texte (exemples de 16, 32 et 64 salles fournis)
- Exploration, ennemis, inventaire et sauvegarde de la partie
- Tests unitaires de l'inventaire et du donjon

## Compiler et lancer

```bash
make
./donjon donjon16.txt
```

Prérequis : `gcc` et `ncurses`.
