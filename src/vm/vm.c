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
#include <brainfuck/vm.h>

#include "../brainfuck.h"

/**
 * Internal array containing the available virtual machine implementations.
 */
static struct BrainfuckVm *vms[] = {NULL};

struct BrainfuckVmContext * brainfuck_vm_alloc(struct BrainfuckVm *vm)
{
    return vm->alloc(vm);
}

void brainfuck_vm_dealloc(struct BrainfuckVmContext *ctx)
{
    ctx->vm->dealloc(ctx);
}

int brainfuck_vm_run(struct BrainfuckVmContext *ctx,
                     const struct BrainfuckProgram *program)
{
    return ctx->vm->run(ctx, program);
}

struct BrainfuckVm ** brainfuck_vm_list(void)
{
    return vms;
}

struct BrainfuckVm * brainfuck_vm_find(const char *name)
{
    struct BrainfuckVm **vm;

    for (vm = vms; vm; vm++) {
        if (!name || !strcmp((*vm)->name, name)) {
            return *vm;
        }
    }

    return NULL;
}
