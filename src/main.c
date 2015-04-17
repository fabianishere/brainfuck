/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Fabian M.
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define isatty _isatty
#endif

#include <brainfuck.h>

/*
 * Print the usage message of this program.
 */
void print_usage() {
	fprintf(stderr, "usage: brainfuck [-vhme] file...\n");
	fprintf(stderr,	"\t-v --version\t\t\tshow the version information\n");
	fprintf(stderr,	"\t-h --help\t\t\tshow a help message\n");
	fprintf(stderr,	"\t-m --memory\t<int>\t\tthe size of the memory block to" \
		" allocate for the program (default: %i elements)\n", BRAINFUCK_DMEMSIZE);
	fprintf(stderr,	"\t-e --eval\t<string>\trun code directly\n");
}

/*
 * Print the version information. 
 */
void print_version() {
	fprintf(stderr, "brainfuck %s (%s, %s)\n", BRAINFUCK_VERSION, __DATE__, 
		__TIME__);
	fprintf(stderr, "Copyright (c) 2015 Fabian M.\n");
	fprintf(stderr, "Distributed under the MIT license.\n");
}

/*
 * Run the given file containg brainfuck code.
 *
 * @param file The file containing the brainfuck code to run.
 * @param ctx The execution context.
 * @return an integer with a value of zero or higher if the script executed 
 *	successfully, a value lower than zero otherwise.
 */
int run_file(FILE *file, struct BrainfuckExecutionContext *ctx) {
	int error = BRAINFUCK_EOK;
	struct BrainfuckScript *script = brainfuck_parse_file(file, &error);
	if (error != BRAINFUCK_EOK)
		return error;
	error = brainfuck_execution_interpret(script, ctx);
	brainfuck_script_free(script);
	return error;
}

/*
 * Run the given string containg brainfuck code.
 *
 * @param string The string containing the brainfuck code to run.
 * @param ctx The execution context.
 * @return an integer with a value of zero or higher if the script executed 
 *	successfully, a value lower than zero otherwise.
 */
int run_string(char *code, struct BrainfuckExecutionContext *ctx)
{
	int error = BRAINFUCK_EOK;
	struct BrainfuckScript *script = brainfuck_parse_string(code, &error);
	if (error != BRAINFUCK_EOK)
		return error;
	error = brainfuck_execution_interpret(script, ctx);
	brainfuck_script_free(script);
	return error;
}

/* Command line options */
static struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{"eval", required_argument, 0, 'e'},
	{"memory", required_argument, 0, 'm'},
	{"version", no_argument, 0, 'v'},
	{0, 0, 0, 0}
};

/*
 * Main entry point of the program.
 *
 * @param argc The amount of arguments given.
 * @param argv The array with arguments.
 */
int main(int argc, char *argv[]) {
	int character = 0;
	int index = 0;
	int option_index = 0;
	int error = BRAINFUCK_EOK;
	unsigned int *memory = NULL;
	char *eval = NULL;
	size_t mem_size = BRAINFUCK_DMEMSIZE;
	FILE *file;
	struct BrainfuckExecutionContext ctx;
	
	while (1) {
		option_index = 0;
		character = getopt_long (argc, argv, "vhm:e:",
			long_options, &option_index);
		if (character == -1)
			break;
		switch (character) {
		case 0:
			if (long_options[option_index].flag != 0)
				break;
			break;
		case 'h':
			print_usage();
			return EXIT_SUCCESS;
		case 'v':
			print_version();
			return EXIT_SUCCESS;
		case 'm':
			mem_size = atoi(optarg);
			break;
		case 'e':	
			eval = optarg;
			break;
		case '?':
			print_usage();
			return EXIT_FAILURE;
		default:
			abort();
		}
	}
	memory = calloc(mem_size, sizeof(unsigned int));
	if (!memory) {
		fprintf(stderr, "error: failed to allocate program memory" \
			"(%lu bytes)\n", sizeof(unsigned int) * mem_size);
		return EXIT_FAILURE;
	}
	brainfuck_execution_context_init(&ctx, getchar, putchar,
		mem_size, memory, 0);
	
	/* execute the given string */
	if (eval) {
		error = run_string(eval, &ctx);
		if (error != BRAINFUCK_EOK)
			fprintf(stderr, "error: program failed with error %i\n", error);
		memset(memory, 0, mem_size);
	}

	/* execute files */
	for (index = optind; index < argc; index++) {
		file = fopen(argv[index], "r");
		if (!file) {
			fprintf(stderr, "error: failed to open file %s\n", argv[index]);
			continue;
		}
		error = run_file(file, &ctx);
		if (error != BRAINFUCK_EOK)
			fprintf(stderr, "error: program %s failed with error %i\n", 
				argv[index], error);
		memset(memory, 0, mem_size);
		fclose(file);
	}
	/* execute code from pipe */
	if (!isatty(STDOUT_FILENO)) {
		error = run_file(stdin, &ctx);
		if (error != BRAINFUCK_EOK)
			fprintf(stderr, "error: program failed with error %i\n", error);
	}
	return EXIT_SUCCESS;
}
