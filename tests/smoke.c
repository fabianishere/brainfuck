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

#include <stdio.h>
#include <stdlib.h>

#include <brainfuck/brainfuck.h>
#include <brainfuck/vm.h>
#include <brainfuck/parser.h>

/**
 * Smoke test running a simple "Hello World" brainfuck program.
 */
int main() {
    int err;
    const char *str = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

    /* Parsing */
    struct BrainfuckProgram program;
    struct BrainfuckParser *parser;
    struct BrainfuckParserContext *pctx;

    if ((parser = brainfuck_parser_find("brainfuck")) == NULL ||
        (pctx = brainfuck_parser_alloc(parser, &program)) == NULL) {
        goto err_parser;
    }

    brainfuck_parser_start(pctx);
    if ((err = brainfuck_parser_consume_string(pctx, str)) != BRAINFUCK_EOK ||
        (err = brainfuck_parser_end(pctx)) != BRAINFUCK_EOK) {
        goto err_parsing;
    }

    brainfuck_parser_dealloc(pctx);

    /* Execution */
    struct BrainfuckVm *vm;
    struct BrainfuckVmContext *vctx;

    if ((vm = brainfuck_vm_find(NULL)) == NULL ||
        (vctx = brainfuck_vm_alloc(vm)) == NULL) {
        goto err_vm;
    }

    vctx->read = &getchar;
    vctx->write = &putchar;
    vctx->memory = calloc(30000, sizeof(uint8_t));

    if ((err = brainfuck_vm_run(vctx, &program)) != BRAINFUCK_EOK) {
        goto err_vm_run;
    }

    brainfuck_vm_dealloc(vctx);
    brainfuck_ir_clear(&program);

    return 0;

    err_parser:
        fprintf(stderr, "error: parser not available: \"brainfuck\".\n");
        return 1;
    err_parsing:
        fprintf(stderr, "error: parser exited with code %i.\n", err);
        brainfuck_parser_dealloc(pctx);
        brainfuck_ir_clear(&program);
        return 1;
    err_vm:
        fprintf(stderr, "error: no virtual machine available\n");
        return 1;
    err_vm_run:
        fprintf(stderr, "error: vm exited with code %i.\n", err);
        brainfuck_vm_dealloc(vctx);
        brainfuck_ir_clear(&program);
        return 1;
}
