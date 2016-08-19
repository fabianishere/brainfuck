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
 * A basic command line interface for the reference implementation of the
 *	brainfuck.h header, written in C89.
 */

/* Dependencies */
#include <stdlib.h>
#include <stdio.h>
#include <brainfuck.h>

/**
 * Main entry point of the program.
 */
int main()
{
	/* Declaration */
	struct BrainfuckScript *script;
	struct BrainfuckEngineContext ctx;
	int error;
	FILE *file;
	
	/* Initialization */
	ctx.read = &getchar;
	ctx.write = &putchar;
	ctx.memory = calloc(30000, sizeof(unsigned char));
	file = fopen("../examples/hanoi.bf", "r");
	
	/* Parsing */
	script = brainfuck_parser_parse_file(file, &error);
	/*script = brainfuck_parser_parse_string("+[]", &error);*/
	
	if (error != BRAINFUCK_EOK) {
		fprintf(stderr, "error: parser exited with code %i.\n", error);
		fclose(file);
		free(ctx.memory);
		return 1;
	}
	
	/* Passes */
	brainfuck_pass_clear(script);
	
	/* Execution */
	error = brainfuck_engine_run(script, &ctx);
	/*brainfuck_script_dump(script, stdout);*/

	/* Cleaning */
	fclose(file);
	free(ctx.memory);
	brainfuck_script_dealloc(script);
	
	/* Exit */
	return error != BRAINFUCK_EOK;
}
