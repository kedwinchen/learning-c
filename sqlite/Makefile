CC=gcc
CFLAGS = -g -I. -lsqlite3
SRC := expexec.c
BIN = expexec

$(BIN): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	rm -vf *.out $(BIN)
