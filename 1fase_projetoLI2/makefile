CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
SRCS = main.c ui.c jogo.c baralho.c carta.c
OBJS = $(SRCS:.c=.o)
TARGET = golf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

: 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
