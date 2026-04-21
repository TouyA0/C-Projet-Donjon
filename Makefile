CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 

SOURCES = inventaire.c
OBJECTS = inventaire.o
TEST_SOURCES = test_inventaire.c
TEST_OBJECTS = test_inventaire.o

.PHONY: test_inventaire clean

test_inventaire: $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o test_inventaire $(TEST_OBJECTS) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o test_inventaire test_inventaire.exe