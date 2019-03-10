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

#include <string.h>

#include <brainfuck/brainfuck.h>
#include <brainfuck/ir.h>

#include "brainfuck.h"

/* brainfuck/brainfuck.h */
const char brainfuck_version[] = BRAINFUCK_VERSION;

const int brainfuck_version_number = BRAINFUCK_VERSION_NUMBER;

/* brainfuck/ir.h */
struct BrainfuckInstruction * brainfuck_ir_alloc(enum BrainfuckOpcode opcode)
{
    struct BrainfuckInstruction *inst = BRAINFUCK_ALLOC(sizeof(struct BrainfuckInstruction));

    if (inst) {
        memset(inst, 0, sizeof(struct BrainfuckInstruction));
        inst->opcode =  opcode;
    }

    return inst;
}

void brainfuck_ir_dealloc(struct BrainfuckInstruction *inst)
{
    BRAINFUCK_DEALLOC(inst);
}

void brainfuck_ir_append(struct BrainfuckProgram *program,
                         struct BrainfuckInstruction *inst)
{
    if (!program->head) {
        program->head = inst;
    }

    if (program->tail) {
        program->tail->next = inst;
        inst->prev = program->tail;
    }

    program->tail = inst;
}

void brainfuck_ir_clear(struct BrainfuckProgram *program)
{
    struct BrainfuckInstruction *inst = program->head;
    struct BrainfuckInstruction *tmp;

    program->head = NULL;
    program->tail = NULL;

    while (inst) {
        tmp = inst;
        inst = inst->next;
        brainfuck_ir_dealloc(tmp);
    }
}
