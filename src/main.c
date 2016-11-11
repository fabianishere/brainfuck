/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Fabian Mastenbroek
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
/*
 * A basic command line interface for this brainfuck library.
 */

/* Dependencies */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <brainfuck/brainfuck.h>
#include <brainfuck/parser.h>
#include <brainfuck/engine.h>

/* isatty for Windows */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#	define isatty _isatty
#endif

/**
 * Print the version information of this program.
 */
void print_version() {
	fprintf(stderr, "brainfuck %s (%s, %s)\n", brainfuck_misc_version, __DATE__, 
			__TIME__);
	fprintf(stderr, "Copyright (c) 2016 Fabian Mastenbroek.\n");
	fprintf(stderr, "Distributed under the MIT License Version.\n");
	fprintf(stderr, "Available modules: parser:brainfuck engine:interpreter ");
	fprintf(stderr, "\n");
}

/**
 * Print the usage message of this program.
 *
 * @param[in] name The name of this program's binary.
 */
void print_usage(char *name) {
	fprintf(stderr, "usage: %s [-hv] file...\n", name);
	fprintf(stderr, "\t-v\t--version\tshow the version information of this program\n");
	fprintf(stderr, "\t-h\t--help\t\tshow this help message\n");
}

/**
 * Main entry point of the program.
 */
int main(int argc, char *argv[])
{
	/* Stack variables */
	int error;
	int opt;
	int pipe;
	FILE *file;
	struct BrainfuckScript *script;
	struct BrainfuckEngineContext ctx;


	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch (opt) {
			case 'h':
				print_usage(argv[0]);
				return EXIT_SUCCESS;
			case 'v':
				print_version();
				return EXIT_SUCCESS;
			default:
				print_usage(argv[0]);
				return EXIT_FAILURE;
		}
	}
	
	/* Check if code is piped */
	pipe = !isatty(fileno(stdin));
	
	/* Print usage if no arguments and no piping */
	if (optind >= argc && !pipe) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	/* Initialize the stack variables */
	ctx.read = &getchar;
	ctx.write = &putchar;
	ctx.memory = calloc(30000, sizeof(uint8_t));
	file = pipe ? stdin : fopen(argv[1], "r");
	
	/* Test if file exists */
	if (!file) {
		fprintf(stderr, "error: cannot open file %s (%s).\n", argv[1], strerror(errno));
		free(ctx.memory);
		return 1;
	}

	/* Parsing */
	script = brainfuck_parser_parse_file(file, &error);

	if (error != BRAINFUCK_EOK) {
		fprintf(stderr, "error: parser exited with code %i.\n", error);
		fclose(file);
		free(ctx.memory);
		return 1;
	}

	/* Execution */
	error = brainfuck_engine_run(script, &ctx);

	/* Cleaning up */
	fclose(file);
	free(ctx.memory);
	brainfuck_script_dealloc(script);

	/* Exit */
	return error != BRAINFUCK_EOK;
}