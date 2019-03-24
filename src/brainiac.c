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

#include <brainiac/brainiac.h>
#include <brainiac/ir.h>

#include "brainiac.h"

/* brainiac/brainiac.h */
const char brainiac_version[] = BRAINIAC_VERSION;

const int brainiac_version_number = BRAINIAC_VERSION_NUMBER;

/* brainiac/ir.h */
struct BrainiacInstruction * brainiac_ir_alloc(enum BrainiacOpcode opcode)
{
    struct BrainiacInstruction *inst = BRAINIAC_ALLOC(sizeof(struct BrainiacInstruction));

    if (inst) {
        memset(inst, 0, sizeof(struct BrainiacInstruction));
        inst->opcode =  opcode;
    }

    return inst;
}

void brainiac_ir_dealloc(struct BrainiacInstruction *inst)
{
    BRAINIAC_DEALLOC(inst);
}

void brainiac_ir_append(struct BrainiacProgram *program,
                         struct BrainiacInstruction *inst)
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

void brainiac_ir_clear(struct BrainiacProgram *program)
{
    struct BrainiacInstruction *inst = program->head;
    struct BrainiacInstruction *tmp;

    program->head = NULL;
    program->tail = NULL;

    while (inst) {
        tmp = inst;
        inst = inst->next;
        brainiac_ir_dealloc(tmp);
    }
}
