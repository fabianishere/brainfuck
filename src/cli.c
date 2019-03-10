/*
 * Copyright (c) 2019 Fabian Mastenbroek
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
#include <brainfuck/vm.h>
#include <brainfuck/ir.h>

/* isatty for Windows */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#   define isatty _isatty
#endif

/**
 * Print the version information of this program.
 */
void print_version(void) {
    fprintf(stderr, "brainfuck %s\n", brainfuck_version);
    fprintf(stderr, "Copyright (c) 2019 Fabian Mastenbroek.\n");
    fprintf(stderr, "Distributed under the MIT License.\n");
}

/**
 * Print the usage message of this program.
 *
 * @param[in] name The name of this program's binary.
 */
void print_usage(char *name) {
    fprintf(stderr, "usage: %s [-hv] [-x vm] [-p parser] file...\n", name);
    fprintf(stderr, "\t-x\t--vm\t\tspecify the virtual machine to use\n");
    fprintf(stderr, "\t-p\t--parser\tspecify the parser to use\n");
    fprintf(stderr, "\t-v\t--version\tshow the version information of this program\n");
    fprintf(stderr, "\t-h\t--help\t\tshow this help message\n");
}

/* Command line options */
static struct option long_options[] = {
    {"vm", required_argument, 0, 'x'},
    {"parser", required_argument, 0, 'p'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

/**
 * Main entry point of the program.
 */
int main(int argc, char *argv[])
{
    int opt, opt_index = 0;
    char *vm_name = NULL;
    char *parser_name  = "brainfuck";

    while ((opt = getopt_long(argc, argv, "hvx:p:", long_options, &opt_index)) != -1) {
        switch (opt) {
            case 0:
                if (long_options[opt_index].flag != 0)
                    break;
                break;
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            case 'v':
                print_version();
                return EXIT_SUCCESS;
            case 'x':
                vm_name = optarg;
                break;
            case 'p':
                parser_name = optarg;
                break;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    /* Check if code is piped */
    int pipe = !isatty(fileno(stdin));

    /* Print usage if no arguments and no piping */
    if (optind >= argc && !pipe) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = pipe ? stdin : fopen(argv[optind], "r");

    /* Test if file exists */
    if (!file) {
        fprintf(stderr, "error: cannot open file %s (%s).\n", argv[optind], strerror(errno));
        return 1;
    }

    /* Parsing */
    struct BrainfuckProgram program;
    struct BrainfuckParser *parser;
    struct BrainfuckParserContext *pctx;
    int err;

    if ((parser = brainfuck_parser_find(parser_name)) == NULL ||
        (pctx = brainfuck_parser_alloc(parser, &program)) == NULL) {
        goto err_parser;
    }

    brainfuck_parser_start(pctx);
    if ((err = brainfuck_parser_consume_file(pctx, file)) != BRAINFUCK_EOK ||
        (err = brainfuck_parser_end(pctx)) != BRAINFUCK_EOK) {
        goto err_parsing;
    }

    fclose(file);
    brainfuck_parser_dealloc(pctx);

    /* Execution */
    struct BrainfuckVm *vm;
    struct BrainfuckVmContext *vctx;

    if ((vm = brainfuck_vm_find(vm_name)) == NULL ||
        (vctx = brainfuck_vm_alloc(vm)) == NULL) {
        goto err_vm;
    }

    vctx->read = &getchar;
    vctx->write = &putchar;
    vctx->memory = calloc(30000, sizeof(uint8_t));

    if ((err = brainfuck_vm_run(vctx, &program)) != BRAINFUCK_EOK) {
        goto err_vm_run;
    }

    free(vctx->memory);
    brainfuck_vm_dealloc(vctx);
    brainfuck_ir_clear(&program);
    return 0;

    err_parser:
        fprintf(stderr, "error: parser not available: \"brainfuck\".\n");
        fclose(file);
        return 1;
    err_parsing:
        fprintf(stderr, "error: parser exited with code %i.\n", err);
        fclose(file);
        brainfuck_parser_dealloc(pctx);
        brainfuck_ir_clear(&program);
        return 1;
    err_vm:
        fprintf(stderr, "error: no virtual machine available\n");
        return 1;
    err_vm_run:
        fprintf(stderr, "error: vm exited with code %i.\n", err);
        free(vctx->memory);
        brainfuck_vm_dealloc(vctx);
        brainfuck_ir_clear(&program);
        return 1;
}
