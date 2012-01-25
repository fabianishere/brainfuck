// Copyright (c) 2012, Fabian M.
// Please see the AUTHORS file for other authors. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1.2"
#define MAX_CELLS 65536

/* Interpreter */
void interpret(char *filename) {
	/* Variable declaration */
	char c;

	FILE *file;

	int pointer = 0;
	int size = 0;
	int char_pointer = 0;
	int loop = 0;
	int tape[MAX_CELLS];

	/* Get the size of the file */
	file = fopen(filename, "r");
	if (file == NULL)
		exit(EXIT_FAILURE);

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char chars[size];

	/* Put every character in character array */
	while ((c = fgetc(file)) != EOF)
		chars[pointer++] = (char) c;


	/* Close file */
	fclose(file);

	/* Set the pointer back to 0 */
	pointer = 0;

	/* Loop through all character in the character array */
	while (char_pointer++ < sizeof(chars))
		switch (chars[char_pointer]) {
		case '>':
			pointer++;
			break;
		case '<':
			pointer--;
			break;
		case '+':
			tape[pointer]++;
			break;
		case '-':
			tape[pointer]--;
			break;
		case '.':
			putchar(tape[pointer]);
			fflush(stdout);
			break;
		case ',':
			tape[pointer] = (int) getchar();
			break;
		case '[':
			if (tape[pointer] == 0) {
				loop = 1;
				while (loop > 0) {
					c = chars[++char_pointer];
					if (c == '[')
						loop++;
					else if (c == ']')
						loop--;
				}
			}
			break;
		case ']':
			loop = 1;
			while (loop > 0) {
				c = chars[--char_pointer];
				if (c == '[')
					loop--;
				else if (c == ']')
					loop++;
			}
			char_pointer--;
			break;
		case '#':
			while (chars[++char_pointer] != '\n');
			break;
		}
}

/* Main */
int main(int argc, char *argv[]) {
	if (argc < 2)
		return EXIT_FAILURE;
	interpret(argv[1]);
	return EXIT_SUCCESS;
}

