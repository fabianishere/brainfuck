#include "brainfuck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__ || defined __unix__ || defined unix || defined _unix || defined __unix
#include <unistd.h>
#endif
/* Read the file and pass it to the brainfuck_eval() function */
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
	/* Strange fix required in order to run most brainfuck programs... */
	chars[pointer++] = ' ';
	/* Place each character from the file into the array */
	while ((c = fgetc(file)) != EOF)
		chars[pointer++] = (char) c;
	fclose(file);
	/* Run the code */
	brainfuck_eval(chars);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: brainfuck [-fi] <filename>\n");
		return EXIT_FAILURE;
	} else if(strcmp(argv[1], "-i") == 0) {
		printf("Entering interactive mode...\n");
		printf("Welcome to the Brainfuck Interpreter v0.2!\n");
		char c;
		int pointer = 0;
		for(;;) {
			printf("brainfuck> ");
			int size = 36000;
			char chars[size];
			chars[pointer++] = ' ';
			while ((c = getchar()) != 10)
				chars[pointer++] = (char) c;
			/* Run the code */
			brainfuck_eval(chars);
		}
	} else if(strcmp(argv[1], "-f") == 0) {
		if(argc < 3)
		{
			printf("Error: No file specified!\n");
		}
		else
		{
			brainfuck_file(argv[2]);
		}
	} else {
		printf("Error: Invalid command line option!\nUsage: brainfuck [-fi] <filename>\n\t-f  <filename>\n\t-i  Interactive Mode\n");
	}
	return EXIT_SUCCESS;
}

