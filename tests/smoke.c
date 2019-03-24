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

#include <brainiac/brainiac.h>
#include <brainiac/vm.h>
#include <brainiac/parser.h>

/**
 * Smoke test running a simple "Hello World" brainfuck program.
 */
int main() {
    int err;
    const char *str = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

    /* Parsing */
    struct BrainiacProgram program;
    struct BrainiacParser *parser;
    struct BrainiacParserContext *pctx;

    if ((parser = brainiac_parser_find("brainfuck")) == NULL ||
        (pctx = brainiac_parser_alloc(parser, &program)) == NULL) {
        goto err_parser;
    }

    brainiac_parser_start(pctx);
    if ((err = brainiac_parser_consume_string(pctx, str)) != BRAINIAC_EOK ||
        (err = brainiac_parser_end(pctx)) != BRAINIAC_EOK) {
        goto err_parsing;
    }

    brainiac_parser_dealloc(pctx);

    /* Execution */
    struct BrainiacVm *vm;
    struct BrainiacVmContext *vctx;

    if ((vm = brainiac_vm_find(NULL)) == NULL ||
        (vctx = brainiac_vm_alloc(vm)) == NULL) {
        goto err_vm;
    }

    vctx->read = &getchar;
    vctx->write = &putchar;
    vctx->memory = calloc(30000, sizeof(uint8_t));

    if ((err = brainiac_vm_run(vctx, &program)) != BRAINIAC_EOK) {
        goto err_vm_run;
    }

    brainiac_vm_dealloc(vctx);
    brainiac_ir_clear(&program);

    return 0;

    err_parser:
        fprintf(stderr, "error: parser not available: \"brainfuck\".\n");
        return 1;
    err_parsing:
        fprintf(stderr, "error: parser exited with code %i.\n", err);
        brainiac_parser_dealloc(pctx);
        brainiac_ir_clear(&program);
        return 1;
    err_vm:
        fprintf(stderr, "error: no virtual machine available\n");
        return 1;
    err_vm_run:
        fprintf(stderr, "error: vm exited with code %i.\n", err);
        brainiac_vm_dealloc(vctx);
        brainiac_ir_clear(&program);
        return 1;
}
