CC = gcc
CFLAGS = -Wall -Wextra -g -O2 -I.
LIBS = -lcunit

SRC = engine.c validador.c save_load.c dsl_parser.c undo.c conversor.c interface.c
OBJ = $(SRC:.c=.o)

all: jogo_paciencias

jogo_paciencias: $(OBJ) main.o
	$(CC) $(CFLAGS) -o $@ $^

testes: $(OBJ) testes.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o jogo_paciencias testes
