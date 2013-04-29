#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "brainfuck.h"

#define USAGEMSG "usage: brainfuck [-fc] <filename>\n\t-f  <filename>\n\t-c  run code directly\n"
/* Read the file and pass it to the brainfuck_eval() function */
void brainfuck_file(char filename[]) {
	FILE *file;
	char c;
	int pointer = 0;
	int size = 0;
	/* Get the size of the file */
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char chars[size];
	/* Place each character from the file into the array */
	while ((c = fgetc(file)) != EOF)
		chars[pointer++] = (char) c;
	fclose(file);
	/* Run the code */
	brainfuck_eval(chars);
}

int main(int argc, char *argv[]) {
	int i = 0;
	char pipe[65536];
	if (argc < 2) {
		printf(USAGEMSG);
		return EXIT_FAILURE;
	} else if(strcmp(argv[1], "-f") == 0) {
		if((argc < 3) && (isatty(fileno(stdin))) == 1)
		{
			printf("error: no file specified!\n");
		}
		else
		{
			if(isatty(fileno(stdin)))
			{
				brainfuck_file(argv[2]);
			}
			else
			{
				while(-1 != (pipe[i++] = getchar()));
				brainfuck_eval(pipe);
			}
		}
	} else if(strcmp(argv[1], "-c") == 0) {
		if((argc < 3) && (isatty(fileno(stdin))) == 1)
		{
			printf("error: no code specified!\n");
		}
		else
		{
			if(isatty(fileno(stdin)))
			{
				brainfuck_eval(argv[2]);
			}
			else
			{
				while(-1 != (pipe[i++] = getchar()));
				brainfuck_eval(pipe);
			}
		}
	} else {
		printf("error: invalid command line option!\n%s", USAGEMSG);
	}
	return EXIT_SUCCESS;
}

