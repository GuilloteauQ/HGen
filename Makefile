CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g # -Wsign-compare #-Wimplicit-function-declaration
EXEC=hgen stack.o

all: $(EXEC)

hgen: main.c stack.c
	$(CC) -o $@ $^ $(CFLAGS)

stack.o: stack.h stack.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm $(EXEC)
