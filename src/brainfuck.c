#include <stdio.h>
#include <stdarg.h>
#if defined __WIN32__ || defined _WIN32_ || defined _WIN32
#include <time.h>
#include <windows.h>
#endif
#if defined __APPLE__ || defined __unix__ || defined unix || defined _unix
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifdef _INTERPRETER_
#include "brainfuck.h"
#endif

#define MAX_CELLS 65536

void brainfuck_eval(char chars[]) {
	char c;
	int pointer = 0;
	int char_pointer = 0;
	int loop = 0;
	int tape[MAX_CELLS];
	/* Loop through all character in the character array */
	while (char_pointer++ < strlen(chars)) 
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
	{
		#if defined _INTERPRETER_
			printf("Usage: brainfuck [-if] <filename>\n");
		#else
			printf("Usage: brainfuck -f <filename>\n");
		#endif
		return EXIT_FAILURE;
	}
	#if defined _INTERPRETER_
		else if(strcmp(argv[1], "-i") == 0)
		{
			brainfuck_interpreter();
		}
	#endif
	else if(strcmp(argv[1], "-f") == 0)
	{
		brainfuck_file(argv[2]);
	}
	else
	{
		#if defined _INTERPRETER_
			printf("Error: Invalid command line option!\nUsage: brainfuck [-if] <filename>\n\t-i  Interactive Mode\n\t-f  <filename>\n");
		#else
			printf("Error: Invalid command line option!\nUsage: brainfuck -f <filename>\n\t-f  <filename>\n");
		#endif
	}
	return EXIT_SUCCESS;
}
