CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g # -Wsign-compare #-Wimplicit-function-declaration
EXEC=hgen

all: $(EXEC)

hgen: main.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(EXEC)
