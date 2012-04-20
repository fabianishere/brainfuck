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
ifeq ($(OS), Linux)
	cp src/brainfuck_linux.1 /usr/local/man/man1/brainfuck.1
endif
ifeq ($(OS), Darwin)
	cp src/brainfuck_darwin.1 /usr/local/man/man1/brainfuck.1
endif
clean:
	rm -f src/*.o
	rm -f src/brainfuck

uninstall:
	rm -f /usr/local/bin/brainfuck
	rm -f /usr/local/man/man1/brainfuck.1