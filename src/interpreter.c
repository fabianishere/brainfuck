57
58
59
60
61
62
63
64
65
66
67
68
/**
 * Lightweight, open-source brainfuck interpreter written in C.
 *
 * @author Fabian M.
 */
// Includes.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
// Constants.
#define FILE_SELF "interpreter.c"
#define interpreter_VERSION "1.0"
// Program
typedef struct
{
	/**
	 * The file to interpret.
	 */
    char *file;
    /**
     * The amount of memory cells to reserve.
     */
    uint32_t cells;
} Program;

/**
 * Output the given char.
 */
void interpreter_output_char(char c) {
	printf("%c", c);
	fflush(stdout);
}

/**
 * Generates error message.
 */
void interpreter_error(char* message, char *method) {
	printf("Error at file \"%s\": %s\n", FILE_SELF, message);
	printf("Caused by: %s()\n", method);
	exit(EXIT_FAILURE);
}

/*
 * Prints out the version of the program.
 */
void interpreter_version() {
    printf("Brainfuck interpreter, version %s.\n\n", interpreter_VERSION);
    printf("For more info, try \"./brainfuck -h\".\n");

    exit(EXIT_SUCCESS);
}

/*
 * Prints out the help of the program.
 */
void interpreter_help() {
    printf("Brainfuck interpreter, version %s.\n\n", interpreter_VERSION);

    printf("Usage: ./brainfuck <input_file>.\n");
    printf("\t\"-v\"    Prints out the version of the program.\n");
    printf("\t\"-h\"    Prints out the help (what you are seeing).\n");

    exit(EXIT_SUCCESS);
}
/**
 * Parses the command line arguments.
 *
 * @param Program *program, the program information.
 * @param int argc, the number of arguments.
 * @param char *argv[], the arguments passed string.
 */
void parse_args(Program *program, int argc, char *argv[]) {
	// Clear program->file value.
	program->file = "";
	// Make it compatible with C90.
	int i = 0;

    for(i = 1; i < argc; i++) {
        if(!strcmp("-v", argv[i])) {
        	interpreter_version();
        } else if(!strcmp("-h", argv[i])) {
        	interpreter_help();
        } else {
    		program->file = argv[i];
    	}
    }
}

/**
 * Interprets the file given at startup.
 */
void interpret(Program *program) {
	if(!program->file || program->file == "") {
	    printf("Missing required arguments, interpreter terminating. \n");
		interpreter_help();
		exit(EXIT_FAILURE);
	}

	// Open file wrapper
	FILE *input = fopen(program->file, "r");

	if(!input) {
		interpreter_error("Could not access input file.", "interpret");
	}

	// Set the amount of cells to 30000
	program->cells = 30000;

	// Contains all memory.
	int data[program->cells];
	// Points to the current Loop through all characters.index.
	int dataPointer = 0;

	// Get size of file.interpreter_output_char(c);
	fseek(input, 0, SEEK_END); // seek to end of file
	int file_size = ftell(input); // get current file pointer
	fseek(input, 0, SEEK_SET); // seek back to beginning of file

	// Current char.
	char c;
	// Array that contains all characters.
	char *chars[file_size];
	// Index at the character array.
	int charPointer = 0;

	// Get file contents.
	while((c = fgetc(input)) != EOF) {
		chars[charPointer] = (char) c;
		charPointer++;
	}

	// Loop through all characters.
	for (charPointer = 0; charPointer < file_size; charPointer++) {
		c = chars[charPointer];
		switch(c) {
		case '>':
			if ((dataPointer + 1) > sizeof(data)) {
				interpreter_error("Data pointer is too big.", "interpret");
			}
			dataPointer++;
			break;
		case '<':
			if ((dataPointer - 1) < 0) {
				interpreter_error("Data pointer is negative.", "interpret");
			}
			dataPointer--;
			break;
		case '+':
			data[dataPointer]++;
			break;
		case '-':
			data[dataPointer]--;
			break;
		case '.':
			interpreter_output_char(data[dataPointer]);
			break;
		case ',':
			data[dataPointer] = (int) fgetc(stdin);
			break;
		case '[':
			if (data[dataPointer] == 0) {
				int i = 1;
				while (i > 0) {
					char next = chars[++charPointer];
					if (next == '[')
						i++;
					else if (next == ']')
						i--;
				}
			}
			break;
		case ']':
			// Pointless if statement
			// , but otherwise we could not declare a variable.
			if (0 == 0) {
				int i = 1;
				while (i > 0) {
					char previous = chars[--charPointer];
					if (previous == '[')
						i--;
					else if (previous == ']')
						i++;
				}
				charPointer--;
			}
			break;
		// Allow hashtag (#) to allow user to put "#!/usr/bin/brainfuck" in their code.
		case '#':
			// Pointless if statement
			// , but otherwise we could not declare a variable.
			if (0 == 0) {
				int i = 1;
				while (i > 0) {
					char next = chars[++charPointer];
					
					if (next == '\n')
						i--;
				}
			}
			break;
		}
	}
}

/**
 * Main execution method of the BrainFuck interpreter.
 */
int main(int argc, char *argv[]) {
    Program program;
    // Parse arguments.
    parse_args(&program, argc, argv);
    // Interpret the program.
    interpret(&program);
    return 0;
}
