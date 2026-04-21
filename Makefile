CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lncurses

# Fichiers sources communs
SOURCES = inventaire.c joueur.c salle.c donjon.c ennemi.c ui_nc.c jeu.c
OBJECTS = inventaire.o joueur.o salle.o donjon.o ennemi.o ui_nc.o jeu.o

# Test inventaire
TEST_INV_SOURCES = test_inventaire.c
TEST_INV_OBJECTS = test_inventaire.o

# Test donjon
TEST_DONJON_SOURCES = test_donjon.c
TEST_DONJON_OBJECTS = test_donjon.o

# Application donjon (Niveau 2)
MAIN_SOURCES = main.c
MAIN_OBJECTS = main.o

# Cibles
.PHONY: donjon test_inventaire test_donjon clean help

# Cible par défaut (première)
donjon: $(MAIN_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o donjon $(MAIN_OBJECTS) $(OBJECTS) $(LDFLAGS)

test_inventaire: $(TEST_INV_OBJECTS) inventaire.o
	$(CC) $(CFLAGS) -o test_inventaire $(TEST_INV_OBJECTS) inventaire.o $(LDFLAGS)

test_donjon: $(TEST_DONJON_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o test_donjon $(TEST_DONJON_OBJECTS) $(OBJECTS) $(LDFLAGS)

# Règles de compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f *.o donjon test_inventaire test_donjon donjon.exe test_inventaire.exe test_donjon.exe

help:
	@echo "Commandes disponibles:"
	@echo "  make donjon           - Compile et crée l'exécutable donjon (par défaut)"
	@echo "  make test_inventaire  - Compile et crée l'exécutable test_inventaire"
	@echo "  make test_donjon      - Compile et crée l'exécutable test_donjon"
	@echo "  make clean            - Supprime les fichiers objets et exécutables"