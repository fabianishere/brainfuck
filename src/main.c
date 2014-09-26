/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Fabian M.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include <brainfuck.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#	define isatty _isatty
#endif

/*
 * Print the usage message of this program.
 */
void print_usage() {
	fprintf(stderr, "usage: brainfuck [-evh] file...\n");
	fprintf(stderr,	"\t-e\trun code directly\n");
	fprintf(stderr, "\t-v\tshow version information\n");
	fprintf(stderr,	"\t-h\tshow this help message\n");
}

/*
 * Print the version information. 
 */
void print_version() {
	fprintf(stderr, "brainfuck %s (%s, %s)\n", BRAINFUCK_VERSION, __DATE__, 
		__TIME__);
	fprintf(stderr, "Copyright (c) 2014 Fabian M. ");
	fprintf(stderr, "Distributed under the MIT license.\n");
}

/*
 * Run the given brainfuck file.
 *
 * @param file The brainfuck file to run.
 * @return EXIT_SUCCESS if no errors are encountered, otherwise EXIT_FAILURE.
 */
int run_file(FILE *file) {
	struct BrainfuckContext *ctx = brainfuck_context_default();
	struct BrainfuckScript *script = NULL;
	int error = BRAINFUCK_OK;
	
	if (!ctx) {
		fprintf(stderr, "error: %i\n", BRAINFUCK_ENOMEM);
		return EXIT_FAILURE;
	}
	
	script = brainfuck_parse_file(file, &error);
	if (error != BRAINFUCK_OK) {
		brainfuck_context_free(ctx);
		fprintf(stderr, "error: %i\n", error);
		return EXIT_FAILURE;
	}
	error = brainfuck_run(script, ctx);
	brainfuck_context_free(ctx);
	brainfuck_script_free(script);
	if (error != BRAINFUCK_OK) {
		fprintf(stderr, "error: %i\n", error);
		return EXIT_FAILURE;
	}
	
 	return EXIT_SUCCESS;
}

/*
 * Run the given brainfuck string.
 *
 * @param code The brainfuck string to run.
 * @return EXIT_SUCCESS if no errors are encountered, otherwise EXIT_FAILURE.
 */
int run_string(char *code) {
	struct BrainfuckContext *ctx = brainfuck_context_default();
	struct BrainfuckScript *script = NULL;
	int error = BRAINFUCK_OK;
	
	if (!ctx) {
		fprintf(stderr, "error: %i\n", BRAINFUCK_ENOMEM);
		return EXIT_FAILURE;
	}
	
	script = brainfuck_parse_string(code, &error);
	if (error != BRAINFUCK_OK) {
		brainfuck_context_free(ctx);
		fprintf(stderr, "error: %i\n", error);
		return EXIT_FAILURE;
	}
	error = brainfuck_run(script, ctx);
	brainfuck_context_free(ctx);
	brainfuck_script_free(script);
	if (error != BRAINFUCK_OK) {
		fprintf(stderr, "error: %i\n", error);
		return EXIT_FAILURE;
	}
 	return EXIT_SUCCESS;
}

/*
 * Run the brainfuck interpreter in interactive mode.
 */
int run_interactive_console() {
	struct BrainfuckContext *ctx = brainfuck_context_default();
	struct BrainfuckScript *script = NULL;
	int error = BRAINFUCK_OK;
	
	printf("brainfuck %s (%s, %s)\n", BRAINFUCK_VERSION, __DATE__, __TIME__);
	
	if (!ctx) {
		fprintf(stderr, "error: %i\n", BRAINFUCK_ENOMEM);
		return EXIT_FAILURE;
	}

	printf(">> ");
	while(1) {
		fflush(stdout);
		script = brainfuck_parse_file(stdin, &error);
		if (error != BRAINFUCK_OK) {
			brainfuck_context_free(ctx);
			fprintf(stderr, "error: %i\n", error);
			continue;
		}
		error = brainfuck_run(script, ctx);
		brainfuck_context_free(ctx);
		brainfuck_script_free(script);
		if (error != BRAINFUCK_OK)
			fprintf(stderr, "error: %i\n", error);
		printf("\n>> ");
	}
	return EXIT_SUCCESS;
}

/* Command line options */
static struct option options[] = {
	{"help", no_argument, 0, 'h'},
	{"version", no_argument, 0, 'v'},
	{"eval", required_argument, 0, 'e'},
	{0, 0, 0, 0}
};


/*
 * Main entry point of the program.
 *
 * @param argc The amount of arguments given.
 * @param argv The array with arguments.
 */
int main(int argc, char *argv[])
{
	int option;
	int index = 1;
	int option_index = 0;
	FILE *file;
	
	while (1) {
		option_index = 0;
		option = getopt_long (argc, argv, "hve:", options, &option_index);
		if (option == -1)
			break;
		switch (option) {
		case 0:
			if (options[option_index].flag != 0)
				break;
			break;
		case 'h':
			print_usage();
			return EXIT_SUCCESS;
		case 'v':
			print_version();
			return EXIT_SUCCESS;
		case 'e':
 			return run_string((char *) optarg);
		case '?':
			print_usage();
			return EXIT_FAILURE;
		default:
			abort();
		}
	}
	if (argc > 1) {
		while (index < argc) {
			file = fopen(argv[index++], "r");
			if (run_file(file) == EXIT_FAILURE)
				fprintf(stderr, "error: failed to open %s\n", argv[index - 1]);
			fclose(file);
		}
	} else {
		/* check whether someone is piping code */
		if (isatty(fileno(stdin)))
			return run_interactive_console();
		else
			return run_file(stdin);
	}
	return EXIT_SUCCESS;
}