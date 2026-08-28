CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main

all: $(TARGET)

$(TARGET): main.c grafo_matriz.c grafo_lista.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c grafo_matriz.c grafo_lista.c -lm

clean:
	rm -f $(TARGET) *.exe

.PHONY: all clean