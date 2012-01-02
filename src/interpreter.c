/* Author: Fabian M. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define VERSION "1.1"
#define MAX_CELLS 65536

/* Program */
typedef struct
{
    char *file;
    int cells;
} Program;


/* Output */
void output(char c) {
	printf("%c", c);
	fflush(stdout);
}
void program_error(char* message) {
	printf("Error at line %i, file \"%s\": %s\n",__LINE__, __FILE__, message);

	exit(EXIT_FAILURE);
}
void error(char* message) {
	printf("%s", message);
	cli_help();
	exit(EXIT_FAILURE);
}

/* Interpreter */
void interpret(Program *program) {
	if(program->file == "") {
	    error("Missing required arguments.");
	}
	program->cells = MAX_CELLS;

	FILE *input = fopen(program->file, "r");

	int tape[program->cells];
	int pointer = 0;

	fseek(input, 0, SEEK_END);
	int file_size = ftell(input);
	fseek(input, 0, SEEK_SET);

	char c;
	char *chars[file_size];
	int char_pointer = 0;

	while((c = fgetc(input)) != EOF) {
		chars[char_pointer] = (char) c;
		char_pointer++;
	}

	while(char_pointer < file_size) {
		c = chars[char_pointer];
		switch(c) {
		case '>':
			if ((pointer + 1) > sizeof(tape))
				program_error("Pointer higher than max-size.");
			pointer++;
			break;
		case '<':
			if ((pointer - 1) < 0)
				program_error("Pointer is negative.");
			pointer--;
			break;
		case '+':
			tape[pointer]++;
			break;
		case '-':
			tape[pointer]--;
			break;
		case '.':
			output(tape[pointer]);
			break;
		case ',':
			tape[pointer] = (int) fgetc(stdin);
			break;
		case '[': {	
				int i = 1;

				while (i > 0) {
					char next = chars[++char_pointer];
					if (next == '[')
						i++;
					else if (next == ']')
						i--;
				}
			}
			break;
		case ']': {
				int i = 1;
				while (i > 0) {
					char previous = chars[--char_pointer];
					if (previous == '[')
						i--;
					else if (previous == ']')
						i++;
				}
				char_pointer--;
			}
			break;
		}
	}
}


/* Command line interface */
void cli_parse(Program *program, int argc, char *argv[]) {
	program->file = "";
	int i = 0;

    for(i = 1; i < argc; i++) {
        if(!strcmp("-v", argv[i])) {
        	cli_version();
        	exit(EXIT_SUCCESS);
        } else if(!strcmp("-h", argv[i])) {
        	cli_help();
        	exit(EXIT_SUCCESS);
        } else {
    		program->file = argv[i];
    	}
    }
}

void cli_version() {
    printf("Brainfuck interpreter, version %s.\n\n", VERSION);
    printf("For more info, try \"./brainfuck -h\".\n");
}

void cli_help() {
    printf("Brainfuck interpreter, version %s.\n\n", VERSION);

    printf("Usage: ./brainfuck <input_file>.\n");
    printf("\t\"-v\"    Prints out the version of the program.\n");
    printf("\t\"-h\"    Prints out the help (what you are seeing).\n");
}

int main(int argc, char *argv[]) {
	Program program;
	cli_args(&program, argc, argv);
	interpret(&program);

	return EXIT_SUCCESS;
}


