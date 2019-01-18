
CC=gcc
CFLAGS=-Wall -ansi -pedantic -g
BASE=linkedlist
BIN=$(BASE).out
INCL=.
SRC=$(BASE).c node.c

all: $(SRC)
	$(CC) -o $(BIN) $^ -I $(INCL)

.PHONY: clean
clean:
	rm -rvf $(BIN) $(BASE) *.o
