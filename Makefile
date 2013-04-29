# Brainfuck Makefile
SRC ?= $(shell pwd)/src
BIN ?= $(shell pwd)/bin
CC ?= gcc
CFLAGS ?= -Wall 


all:
	@echo "Building"
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(BIN)/brainfuck $(SRC)/*
clean:
	@echo "Cleaning"
	rm -f src/*.o
	rm -r bin/
install:
	@echo "Installing" 
	cp $(BIN)/brainfuck /usr/local/bin/brainfuck
	cp man/brainfuck.1 /usr/local/share/man/man1/brainfuck.1
uninstall:
	@echo "Uninstalling"
	rm -f /usr/local/bin/brainfuck
	rm -f /usr/local/share/man/man1/brainfuck.1
