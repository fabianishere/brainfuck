#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CELLS 65536

void brainfuck_eval(char chars[]) {
	char c;
	int pointer = 0;
	int char_pointer = 0;
	int loop = 0;
	int tape[MAX_CELLS];
	/* Loop through all character in the character array */
	while (char_pointer++ < strlen(chars)) {
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
}
void brainfuck_file(char filename[]) {

	FILE *file;
	char c;
	int pointer = 0;
	int size = 0;

	/* Get the size of the file */
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char chars[size];
	/* Put every character in character array */
	while ((c = fgetc(file)) != EOF)
		chars[pointer++] = (char) c;
	/* Close file */
	fclose(file);
	/* Run the code */
	brainfuck_eval(chars);
}
int main(int argc, char *argv[]) {
	if (argc < 2)
		return EXIT_FAILURE;
	brainfuck_file(argv[1]);
	return EXIT_SUCCESS;
}
