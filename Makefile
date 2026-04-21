CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -DNIV01
LDFLAGS = 

# Fichiers sources communs
SOURCES = inventaire.c joueur.c salle.c donjon.c ui.c
OBJECTS = inventaire.o joueur.o salle.o donjon.o ui.o

# Test inventaire
TEST_INV_SOURCES = test_inventaire.c
TEST_INV_OBJECTS = test_inventaire.o

# Test donjon
TEST_DONJON_SOURCES = test_donjon.c
TEST_DONJON_OBJECTS = test_donjon.o

# Cibles
.PHONY: test_inventaire test_donjon clean help

test_inventaire: $(TEST_INV_OBJECTS) inventaire.o
	$(CC) $(CFLAGS) -o test_inventaire $(TEST_INV_OBJECTS) inventaire.o $(LDFLAGS)

test_donjon: $(TEST_DONJON_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o test_donjon $(TEST_DONJON_OBJECTS) $(OBJECTS) $(LDFLAGS)

# Règles de compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f *.o test_inventaire test_donjon test_inventaire.exe test_donjon.exe