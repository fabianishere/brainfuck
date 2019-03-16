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

#include <brainfuck/brainfuck.h>
#include <brainfuck/vm.h>
#include <brainfuck/ir.h>

#include "../brainfuck.h"


static struct BrainfuckVmContext * alloc(struct BrainfuckVm *vm)
{
    struct BrainfuckVmContext *ctx = BRAINFUCK_ALLOC(sizeof(struct BrainfuckVmContext));

    if (ctx) {
        ctx->vm = vm;
        ctx->memory = NULL;
        ctx->memory_size = 0;
        ctx->read = NULL;
        ctx->write = NULL;
    }

    return ctx;
}

static void dealloc(struct BrainfuckVmContext *ctx)
{
    BRAINFUCK_DEALLOC(ctx);
}

static int run(struct BrainfuckVmContext *ctx,
               const struct BrainfuckProgram *program)
{
    register struct BrainfuckInstruction *inst = program->head;
    register uint8_t *mem = ctx->memory;
    register int index = 0;

    while (inst) {
        switch(inst->opcode) {
            case ADD:
                mem[index + inst->operands[0].i32] += inst->operands[1].i32;
                break;
            case MOV:
                index += inst->operands[0].i32;
                break;
            case CPY:
                mem[index + inst->operands[1].i32] = mem[index + inst->operands[0].i32];
                break;
            case MUL:
                mem[index + inst->operands[1].i32] = mem[index + inst->operands[0].i32] * inst->operands[2].i8;
                break;
            case CLR:
                mem[index + inst->operands[0].i32] = 0;
                break;
            case IN:
                mem[index + inst->operands[0].i32] = (uint8_t) ctx->read();
                break;
            case OUT:
                ctx->write(mem[index + inst->operands[0].i32]);
                break;
            case JNZ:
                if (!mem[index])
                    break;
                /* Fallthrough */
            case JMP:
                inst = inst->operands[0].ref;
                continue;
            case NOP:
                break;
        }
        inst = inst->next;
    }
    return BRAINFUCK_EOK;
}

struct BrainfuckVm brainfuck_vm_interpreter = {
    .name = "interpreter",
    .version = "1.0.0",
    .alloc = &alloc,
    .dealloc = &dealloc,
    .run = &run,
};
