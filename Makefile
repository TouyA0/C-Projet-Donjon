CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lncurses

SOURCES = inventaire.c joueur.c salle.c donjon.c ennemi.c ui_nc.c jeu.c save.c explore.c
OBJECTS = inventaire.o joueur.o salle.o donjon.o ennemi.o ui_nc.o jeu.o save.o explore.o

# test inventaire
TEST_INV_SOURCES = test_inventaire.c
TEST_INV_OBJECTS = test_inventaire.o

# test donjon
TEST_DONJON_SOURCES = test_donjon.c
TEST_DONJON_OBJECTS = test_donjon.o

# donjon (par defaut)
MAIN_SOURCES = main.c
MAIN_OBJECTS = main.o

.PHONY: donjon test_inventaire test_donjon clean help

donjon: $(MAIN_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o donjon $(MAIN_OBJECTS) $(OBJECTS) $(LDFLAGS)

test_inventaire: $(TEST_INV_OBJECTS) inventaire.o
	$(CC) $(CFLAGS) -o test_inventaire $(TEST_INV_OBJECTS) inventaire.o $(LDFLAGS)

test_donjon: $(TEST_DONJON_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o test_donjon $(TEST_DONJON_OBJECTS) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o donjon test_inventaire test_donjon donjon.exe test_inventaire.exe test_donjon.exe