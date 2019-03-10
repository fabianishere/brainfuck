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

#ifndef BRAINFUCK_IR_H
#define BRAINFUCK_IR_H

#include <stdio.h>
#include <stdint.h>

/**
 * This submodule defines the Intermediate Representation (IR) to which input
 * is converted to by the parser. This intermediate representation is then
 * consumed by (optimizing) passes or a {@link BrainfuckVm}.
 *
 * @addtogroup ir Intermediate Representation
 * @{
 */
/**
 * This structure represents a single operation that could be executed. It
 * contains the opcode of the operation and its operands.
 */
struct BrainfuckInstruction {
    /**
     * The instruction that follows this instruction.
     */
    struct BrainfuckInstruction *next;

    /**
     * The instruction that precedes this instruction.
     */
    struct BrainfuckInstruction *prev;

    /**
     * This enum defines a basic instruction set which brainfuck code can be
     * compiled to, with the inclusion of multiple optimizing/debugging
     * instructions.
     */
    enum BrainfuckOpcode {
        /* Data Manipulation */
        /**
         * Add value to memory cell.
         */
        ADD,

        /**
         * Move in the memory.
         */
        MOV,

        /**
         * Clear memory cell value.
         */
        CLR,

        /**
         * Copy the memory cell value of <code>x</code> to the memory cell
         * value of <code>y</code>.
         */
        CPY,

        /**
         * Copy the memory cell value of <code>x</code> to the memory cell
         * value of <code>y</code> and multiply that value by <code>z</code>.
         */
        MUL,

        /* Control Flow */
        /**
         * Unconditionally jump to the given instruction.
         */
        JMP,

        /**
         * Conditionally jump to the given instruction if
         * the value of the current memory cell is not zero.
         */
        JNZ,

        /* IO */
        /**
         * Read one byte from the input stream and write to cell at the given
         * offset.
         */
        IN,

        /**
         * Write the byte at the given memory offset to the output stream.
         */
        OUT,

        /* Miscellaneous */
        /**
         * No operation.
         */
        NOP
    } opcode;

    /**
     * The three operand values of the instruction.
     */
    union {
        /**
         * A signed 8-bit value.
         */
        int8_t i8;

        /**
         * A unsigned 8-bit value.
         */
        uint8_t u8;

        /**
         * A signed 32-bit value.
         */
        int32_t i32;

        /**
         * A unsigned 32-bit value.
         */
        uint32_t u32;

        /**
         * A pointer to an instruction.
         */
        struct BrainfuckInstruction *ref;
    } operands[3];
};

/**
 * Allocate a {@link BrainfuckInstruction}.
 *
 * @param[in] opcode The opcode of the instruction to allocate.
 * @return The allocated instruction or <code>NULL</code> on allocation failure.
 */
struct BrainfuckInstruction * brainfuck_ir_alloc(enum BrainfuckOpcode opcode);

/**
 * Deallocate an individual {@link BrainfuckInstruction}.
 *
 * @param[in] inst The instruction to deallocate.
 */
void brainfuck_ir_dealloc(struct BrainfuckInstruction *inst);

/**
 * This structure represents a linked list of {@link BrainfuckInstruction}s.
 *
 * @see BrainfuckInstruction
 */
struct BrainfuckProgram {
    /**
     * The head of the instruction list.
     */
    struct BrainfuckInstruction *head;

    /**
     * The tail of the instruction list.
     */
    struct BrainfuckInstruction *tail;
};

/**
 * Append the specified instruction to the given program.
 *
 * @param[in] program The program to append the instruction to.
 * @param[in] inst The instruction to append to the program.
 */
void brainfuck_ir_append(struct BrainfuckProgram *program,
                         struct BrainfuckInstruction *inst);

/**
 * Remove all instructions in the specified program.
 *
 * @param[in] program The program remove all instructions from.
 */
void brainfuck_ir_clear(struct BrainfuckProgram *program);
/** @} */

#endif /* BRAINFUCK_IR_H */
