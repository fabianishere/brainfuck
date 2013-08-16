/*
 * Copyright 2013 Fabian M.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "brainfuck.h"

/*
 * Prints the usage message of this program.
 */
void print_usage() {
	fprintf(stderr, "usage: brainfuck [-feihd] <filename>\n");
	fprintf(stderr, "\t-f  run the given file(s)\n");
	fprintf(stderr,	"\t-e  run code directly\n");
	fprintf(stderr,	"\t-i  open the interactive console\n");
	fprintf(stderr,	"\t-h  show a help message\n");
	fprintf(stderr,	"\t-d  enable debugging\n");
}

/*
 * Runs the given brainfuck file.
 *
 * @param file The brainfuck file to run.
 * @param debug_flag Set to 0 to disable debugging, 1 to enable.
 * @return EXIT_SUCCESS if no errors are encountered, otherwise EXIT_FAILURE.
 */
int run_file(FILE *file, int debug_flag) {
	BrainfuckState *state = brainfuck_new_state(debug_flag);
	if (file == NULL) {
		printf("failed to open file\n");
		brainfuck_end_state(state);
		return EXIT_FAILURE;
	}
	brainfuck_put_instruction(state, brainfuck_read_stream(file));
	brainfuck_execute(state, state->root);
	brainfuck_end_state(state);
	fclose(file);
	return EXIT_SUCCESS;
}

/*
 * Runs the given brainfuck string.
 *
 * @param code The brainfuck string to run.
 * @param debug_flag Set to 0 to disable debugging, 1 to enable.
 * @return EXIT_SUCCESS if no errors are encountered, otherwise EXIT_FAILURE.
 */
int run_string(char *code, int debug_flag) {
	BrainfuckState *state = brainfuck_new_state(debug_flag);
 	brainfuck_put_instruction(state, brainfuck_read_string(code));
 	brainfuck_execute(state, state->root);
 	brainfuck_end_state(state);
 	return EXIT_SUCCESS;
}

/*
 * Run the brainfuck interpreter in interactive mode.
 *
 * @param debug_flag 
 */
void run_interactive_console(int debug_flag) {
	printf("brainfuck %s (%s, %s)\n", BRAINFUCK_VERSION, __DATE__, __TIME__);
	BrainfuckState *state = brainfuck_new_state(debug_flag);
	
	printf(">> ");
	while(1) {
		fflush(stdout);
		brainfuck_execute(state, brainfuck_put_instruction(state, brainfuck_read_stream_until(stdin, '\n')));
		printf("\n>> ");
	}
}


/* Variables used by getopt.h */
static int debug_flag;
static int file_flag;
static struct option long_options[] = {
	{"debug", no_argument, 0, 'd'},
	{"file", no_argument, 0, 'f'},
	{"help", no_argument, 0, 'h'},
	{"eval", no_argument, 0, 'e'},
	{0, 0, 0, 0}
};

/*
 * Main entry point of the program.
 *
 * @param argc The amount of arguments given.
 * @param argv The array with arguments.
 */
int main(int argc, char *argv[]) {
	int c;
	char *eval_code = 0;
	int eval = 0;
	int option_index = 0;
	
	while (1) {
		option_index = 0;
		c = getopt_long (argc, argv, "hfde",
			long_options, &option_index);
		if (c == -1)
			break;
			
		switch (c) {
		case 0:
			if (long_options[option_index].flag != 0)
				break;
			break;
		case 'h':
			print_usage();
			return EXIT_SUCCESS;
		case 'e':	
			eval = 1;
			if (optarg)
				eval_code = (char *) optarg;
			break;
		case 'f':
			file_flag = 1;
			break;
		case 'd':
			debug_flag = 1;
			break;
		case '?':
			print_usage();
			return EXIT_FAILURE;
		default:
			abort();
		}
	}
	
	// this variable is true when the -e or --eval flag is set.
 	if (eval) {
 		if (isatty(fileno(stdin))) {// check if someone is trying to pipe code.
 			return run_string(eval_code, debug_flag);
 		} else {
 			BrainfuckState *state = brainfuck_new_state(debug_flag);
			brainfuck_put_instruction(state, brainfuck_read_stream(stdin));
			brainfuck_execute(state, state->root);
			brainfuck_end_state(state);
			return EXIT_SUCCESS;
		}
 	} else if (optind < argc && file_flag) {
		while (optind < argc)
			run_file(fopen(argv[optind++], "r"), debug_flag);
		return EXIT_SUCCESS;
	}
	
	run_interactive_console(debug_flag);
	return EXIT_SUCCESS;
}
