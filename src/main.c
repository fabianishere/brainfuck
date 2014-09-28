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

/*
 * Main entry point of the program.
 *
 * @param argc The amount of arguments given.
 * @param argv The array with arguments.
 */
int main(void)
{
	int error = BRAINFUCK_EOK;
	FILE *file = fopen("../examples/mandel.bf", "r");
	if (!file) {
		fprintf(stderr, "error: failed to open file.\n");
		return 0;
	}
	struct BrainfuckContext *ctx = brainfuck_context_default();
	/*struct BrainfuckScript *script = brainfuck_parse_string("+[[[[[[[[[[[[-]]]]]]]]]]]]+.", &error);
	*/struct BrainfuckScript *script = brainfuck_parse_file(file, &error);
	if (error != BRAINFUCK_EOK) {
		fprintf(stderr, "error: program failed with error code %i\n", error);
		return 0;
	}
	brainfuck_run(script, ctx);
	brainfuck_script_free(script);
	brainfuck_context_free(ctx);
}
