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

#include <assert.h>

#include <brainiac/brainiac.h>
#include <brainiac/ir.h>

void test_ir_append_empty()
{
    struct BrainiacProgram program = {0};
    struct BrainiacInstruction *nop = brainiac_ir_alloc(NOP);

    brainiac_ir_append(&program, nop);

    assert(program.head == nop && program.tail == nop);
}

void test_ir_append_singleton()
{
    struct BrainiacProgram program = {0};
    struct BrainiacInstruction *x = brainiac_ir_alloc(NOP);
    struct BrainiacInstruction *y = brainiac_ir_alloc(NOP);

    brainiac_ir_append(&program, x);
    brainiac_ir_append(&program, y);

    assert(program.head == x);
    assert(program.tail == y);
    assert(x->next == y);
    assert(y->prev == x);
}

void test_ir_append_multiple()
{
    struct BrainiacProgram program = {0};
    struct BrainiacInstruction *x = brainiac_ir_alloc(NOP);
    struct BrainiacInstruction *y = brainiac_ir_alloc(NOP);
    struct BrainiacInstruction *z = brainiac_ir_alloc(NOP);

    brainiac_ir_append(&program, x);
    brainiac_ir_append(&program, y);
    brainiac_ir_append(&program, z);

    assert(program.head == x);
    assert(program.tail == z);
    assert(x->next == y);
    assert(y->prev == x);
    assert(y->next == z);
    assert(z->prev == y);
}

void test_ir_clear()
{
    struct BrainiacProgram program = {0};
    struct BrainiacInstruction *x = brainiac_ir_alloc(NOP);
    struct BrainiacInstruction *y = brainiac_ir_alloc(NOP);

    brainiac_ir_append(&program, x);
    brainiac_ir_append(&program, y);
    brainiac_ir_clear(&program);

    assert(program.head == NULL && program.tail == NULL);
}

/**
 * Test functions related to the IR module
 */
int main() {
    test_ir_append_empty();
    test_ir_append_singleton();
    test_ir_append_multiple();

    test_ir_clear();
}
