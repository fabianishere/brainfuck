CC = gcc
OS := $(shell uname)

all:
ifeq ($(OS), Darwin)
	$(CC) -o src/brainfuck src/brainfuck.c
endif
ifeq ($(OS), Linux)
	$(CC) -o src/brainfuck src/brainfuck.c
endif
ifeq ($(OS), CYGWIN_NT-5.1)
	$(CC) -c src/brainfuck.c -o src/brainfuck
endif
install:
	cp src/brainfuck /usr/local/bin/brainfuck

clean:
	rm -f src/*.o
	rm -f src/brainfuck

uninstall:
	rm -f /usr/local/bin/brainfuck