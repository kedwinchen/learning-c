CC=gcc
CFLAGS=-g -I.
DEPS=logger.h
OBJ = dologging.o logger.o
BIN = logging

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean: 
	rm -vf $(OBJ) $(BIN)
