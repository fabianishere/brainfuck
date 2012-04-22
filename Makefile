CC = gcc
OS := $(shell uname)

all:
	mkdir bin
ifeq ($(OS), Darwin)
	$(CC) -o bin/brainfuck src/*
endif
ifeq ($(OS), Linux)
	$(CC) -o bin/brainfuck src/*
endif
ifeq ($(OS), CYGWIN_NT-5.1)
	$(CC) -c src/* -o bin/brainfuck
endif
install:
	cp bin/brainfuck /usr/local/bin/brainfuck
ifeq ($(OS), Linux)
	cp man/brainfuck_linux.1 /usr/local/share/man/man1/brainfuck.1
endif
ifeq ($(OS), Darwin)
	cp man/brainfuck_darwin.1 /usr/local/share/man/man1/brainfuck.1
endif
clean:
	rm -f src/*.o
	rm -r bin/

uninstall:
	rm -f /usr/local/bin/brainfuck
	rm -f /usr/local/share/man/man1/brainfuck.1