CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

# Ficheiros comuns aos dois executáveis
COMMON_SRCS = jogo.c baralho.c carta.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Executável principal do jogo Simple Simon
GAME_SRCS = main.c ui.c
GAME_OBJS = $(GAME_SRCS:.c=.o)
GAME_TARGET = simon

# Executável de testes (CUnit)
TEST_SRCS = testes.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_TARGET = testes

all: $(GAME_TARGET) $(TEST_TARGET)

$(GAME_TARGET): $(COMMON_OBJS) $(GAME_OBJS)
	$(CC) $(COMMON_OBJS) $(GAME_OBJS) -o $(GAME_TARGET)

$(TEST_TARGET): $(COMMON_OBJS) $(TEST_OBJS)
	$(CC) $(COMMON_OBJS) $(TEST_OBJS) -o $(TEST_TARGET) -lcunit

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(GAME_TARGET) $(TEST_TARGET)
