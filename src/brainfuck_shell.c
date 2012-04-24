#include "brainfuck.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#if defined __WIN32__ || defined _WIN32_ || defined _WIN32
#include <time.h>
#include <windows.h>
#elif defined __APPLE__ || defined __unix__ || defined unix || defined _unix || defined __unix
#include <unistd.h>
#endif

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
	while ((c = fgetc(file)) != EOF)
		chars[pointer++] = (char) c;
	fclose(file);
	brainfuck_eval(chars);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: brainfuck [-fi] <filename>\n");
		return EXIT_FAILURE;
	} else if(strcmp(argv[1], "-i") == 0) {
		printf("Entering interactive mode...\n");
		#if defined __APPLE__ || defined __unix__ || defined unix || defined _unix || defined __unix
			fflush(stdout);
		#endif
		#if !defined __WIN32__ || !defined _WIN32_ || !defined _WIN32
			sleep(1);
		#endif
		#if !defined __WIN32__ || !defined _WIN32_ || !defined _WIN32
			sleep(1);
		#endif
		printf("Welcome to the Brainfuck Interpreter!\n");
		#if defined __APPLE__ || defined __unix__ || defined unix || defined _unix || defined __unix
		fflush(stdout);
		#endif
		for(;;) {
			printf("brainfuck> ");
			char c;
			int pointer = 0;
			/* Put every character in character array */
			int size = 1048576;
			char chars[size];
			c = ' ';
			chars[pointer++] = (char) c;
			while ((c = getchar()) != '\n')
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

