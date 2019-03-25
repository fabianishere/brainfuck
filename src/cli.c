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

#include <brainiac/brainiac.h>
#include <brainiac/parser.h>
#include <brainiac/vm.h>
#include <brainiac/ir.h>

/* isatty for Windows */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <io.h>
    #define isatty _isatty
    #define STDIN_FILENO 0
#else
    #include <unistd.h>
#endif

/**
 * Print the version information of this program.
 */
void print_version(void)
{
    fprintf(stderr, "brainiac %s\n", brainiac_version);
    fprintf(stderr, "Copyright (c) 2019 Fabian Mastenbroek.\n");
    fprintf(stderr, "Distributed under the MIT License.\n");
}

/**
 * Print the usage message of this program.
 *
 * @param[in] name The name of this program's binary.
 */
void print_usage(char *name)
{
    fprintf(stderr, "usage: %s [-hv] [-x vm] [-p parser] file...\n", name);
    fprintf(stderr, "\t-x\t--vm\t\tspecify the virtual machine to use\n");
    fprintf(stderr, "\t-p\t--parser\tspecify the parser to use\n");
    fprintf(stderr, "\t-v\t--version\tshow the version information of this program\n");
    fprintf(stderr, "\t-h\t--help\t\tshow this help message\n");
}

/**
 * Print the available virtual machines.
 */
void print_vms(void)
{
    struct BrainiacVm **vm;

    fprintf(stderr, "Available virtual machines:\n");
    for (vm = brainiac_vm_list(); *vm; vm++) {
        fprintf(stderr, "%s %s\n", (*vm)->name, (*vm)->version);
    }
}

/**
 * Print the available parsers.
 */
void print_parsers(void)
{
    struct BrainiacParser **parser;

    fprintf(stderr, "Available parsers:\n");
    for (parser = brainiac_parser_list(); *parser; parser++) {
        fprintf(stderr, "%s %s\n", (*parser)->name, (*parser)->version);
    }
}

/* Command line options */
static int list_vms_option = 0;
static int list_parsers_option = 0;

static struct option long_options[] = {
    {"vm", required_argument, 0, 'x'},
    {"list-vms", no_argument, &list_vms_option, 1},
    {"parser", required_argument, 0, 'p'},
    {"list-parsers", no_argument, &list_parsers_option, 1},
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
    char *parser_name  = NULL;

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

    /* Handle long options */
    if (list_vms_option) {
        print_vms();
        return EXIT_SUCCESS;
    } else if (list_parsers_option) {
        print_parsers();
        return EXIT_SUCCESS;
    }

    /* Check if code is piped */
    int pipe = !isatty(STDIN_FILENO);

    /* Print usage if no arguments and no piping */
    if (optind >= argc && !pipe) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *path = argv[optind];
    const char *ext = strrchr(path, '.');
    FILE *file = pipe ? stdin : fopen(path, "r");

    /* Test if file exists */
    if (!file) {
        fprintf(stderr, "error: cannot open file %s (%s).\n", path, strerror(errno));
        return 1;
    }

    /* Parsing */
    struct BrainiacProgram program;
    struct BrainiacParser *parser;
    struct BrainiacParserContext *pctx;
    int err;

    if (pipe || parser_name || !ext) {
        parser = brainiac_parser_find(parser_name);
    } else {
        parser = brainiac_parser_find_by_extension(ext + 1);
    }

    if (!parser || (pctx = brainiac_parser_alloc(parser, &program)) == NULL) {
        goto err_parser;
    }

    brainiac_parser_start(pctx);
    if ((err = brainiac_parser_consume_file(pctx, file)) != BRAINIAC_EOK ||
        (err = brainiac_parser_end(pctx)) != BRAINIAC_EOK) {
        goto err_parsing;
    }

    fclose(file);
    brainiac_parser_dealloc(pctx);

    /* Execution */
    struct BrainiacVm *vm;
    struct BrainiacVmContext *vctx;

    if ((vm = brainiac_vm_find(vm_name)) == NULL ||
        (vctx = brainiac_vm_alloc(vm)) == NULL) {
        goto err_vm;
    }

    vctx->read = &getchar;
    vctx->write = &putchar;
    vctx->memory = calloc(30000, sizeof(uint8_t));

    if ((err = brainiac_vm_run(vctx, &program)) != BRAINIAC_EOK) {
        goto err_vm_run;
    }

    free(vctx->memory);
    brainiac_vm_dealloc(vctx);
    brainiac_ir_clear(&program);

    return 0;

    err_parser:
        fprintf(stderr, "error: parser not available: \"%s\".\n", parser_name);
        fclose(file);
        return 1;
    err_parsing:
        fprintf(stderr, "error: parser exited with code %i.\n", err);
        fclose(file);
        brainiac_parser_dealloc(pctx);
        brainiac_ir_clear(&program);
        return 1;
    err_vm:
        fprintf(stderr, "error: no virtual machine available\n");
        return 1;
    err_vm_run:
        fprintf(stderr, "error: vm exited with code %i.\n", err);
        free(vctx->memory);
        brainiac_vm_dealloc(vctx);
        brainiac_ir_clear(&program);
        return 1;
}
