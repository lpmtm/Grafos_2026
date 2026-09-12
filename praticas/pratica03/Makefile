# Makefile - Pratica 03: Ordenacao Topologica (Kahn / DFS)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = dag.c main.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
