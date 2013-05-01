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

void print_usage() {
	printf("usage: brainfuck [-feihd] <filename>\n\t-f  run the given file(s)\n");
	printf("\t-e  run code directly\n\t-i  open the interactive console.\n");
	printf("\t-h  show a help message.\n\t-d  enable debugging.\n");
}

void run_file(FILE *file, int debug_flag) {
	BrainfuckState *state = brainfuck_new_state(debug_flag);
	if (file == NULL) {
		printf("failed to open file\n");
		exit(EXIT_FAILURE);
	}
	brainfuck_put_instruction(state, brainfuck_read_stream(file));
	brainfuck_execute(state, state->root);
	brainfuck_end_state(state);
	fclose(file);
}

static int debug_flag;
static int file_flag;
static int interactive_flag;

int main(int argc, char *argv[]) {
	int c;
	char *eval_code = 0;
	while (1) {
		static struct option long_options[] = {
			{"debug", no_argument, 0, 'd'},
			{"file", no_argument, 0, 'f'},
			{"help", no_argument, 0, 'h'},
			{"interactive", no_argument, 0, 'i'},
			{"eval", required_argument, 0, 'e'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "hifde:",
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
			exit(EXIT_SUCCESS);
			break;
		case 'e':
			if (optarg)
				eval_code = (char *) optarg;
			break;
		case 'i':
			interactive_flag = 1;
			break;
		case 'f':
			file_flag = 1;
			break;
		case 'd':
			debug_flag = 1;
			break;
		case '?':
			print_usage();
			exit(EXIT_FAILURE);
			break;
		default:
			abort();
		}
	}
 	if (eval_code) {
 		BrainfuckState *state = brainfuck_new_state(debug_flag);
 		brainfuck_put_instruction(state, brainfuck_read_string(eval_code));
 		brainfuck_execute(state, state->root);
 		brainfuck_end_state(state);
 	} else if (optind < argc && file_flag) {
		while (optind < argc)
			run_file(fopen(argv[optind++], "r"), debug_flag);
	} else if (interactive_flag) {
	} 
	exit(EXIT_SUCCESS);
}
