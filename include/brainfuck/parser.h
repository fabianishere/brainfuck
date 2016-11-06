/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Fabian Mastenbroek
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
 * This header file defines the parser interface for this library.
 */
#ifndef BRAINFUCK_PARSER_H
#define BRAINFUCK_PARSER_H

/* Dependencies */
#include <stdio.h>
#include <stdint.h>

/**
 * This module defines the library's parser interface for the brainfuck 
 * language.
 *
 * @addtogroup parser Parser
 * @{
 */
/**
 * This submodule defines the Intermediate Representation (IR) to which input 
 * is converted to by the parser. This intermediate representation is then
 * consumed by for example (optimizing) passes or an interpreter.
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
	 * This enum defines a basic instruction set which brainfuck code can be
	 * compiled to, with the inclusion of multiple optimizing/debugging 
	 * instructions.
	 */
	enum BrainfuckInstructionSet {
		/* Data Manipulation */
		/**
		 * Add value to memory cell.
		 *
		 * @param[in] x The delta to add to the memory cell as 
		 * <code>int32_t</code>.
		 * @param[in] y The memory offset from the current cell as 
		 * <code>int32_t</code>.
		 */
		ADD,
		
		/**
		 * Subtract value from memory cell.
		 *
		 * @param[in] x The delta to subtract from the memory cell as 
		 * <code>int32_t</code>.
		 * @param[in] y The memory offset from the current memory cell as 
		 * <code>int32_t</code>.
		 */
		DEC,
		
		/**
		 * Move left in the memory.
		 *
		 * @param[in] x The amount of memory cells to move to the left as 
		 * <code>int32_t</code>.
		 */
		MOVL,
		
		/**
		 * Move right in the memory.
		 *
		 * @param[in] x The amount of memory cells to move to the right as 
		 * <code>int32_t</code>.
		 */
		MOVR,
		
		/**
		 * Clear memory cell value.
		 *
		 * @param[in] x The memory offset from the current memory cell as 
		 * <code>int32_t</code>.
		 */
		CLR,
		
		/**
		 * Copy the memory cell value of <code>x</code> to the memory cell 
		 * value of <code>y</code>.
		 *
		 * @param[in] x The memory offset from the current cell to
		 * the source as <code>int32_t</code>.
		 * @param[in] y The memory offset from the current cell to
		 * the destination as <code>int32_t</code>.
		 */
		CPY,
		
		/**
		 * Copy the memory cell value of <code>x</code> to the memory cell 
		 * value of <code>y</code> and multiply that value by <code>z</code>.
		 *
		 * @param[in] x The memory offset from the current cell to the source 
		 * as <code>int32_t</code>.
		 * @param[in] y The memory offset from the current cell to
		 * the destination as <code>int32_t</code>.
		 * @param[in] z The multiplier as <code>int32_t</code>.
		 */
		MUL,
		
		/* Control Flow */
		/**
		 * Unconditionally jump to the given instruction.
		 *
		 * @param[in] x The instruction to jump to as pointer to a
		 *	<code>BrainfuckInstruction</code>.
		 * @param[in] y The jump flags as <code>uint32_t</code>.
		 */
		JMP,
		
		/**
		 * Conditionally jump to the given instruction if
		 * the value of the current memory cell is not zero.
		 *
		 * @param[in] x The instruction to jump to as 
		 * pointer to a <code>BrainfuckInstruction</code>.
		 * @param[in] y The jump flags as <code>uint32_t</code>.
		 */
		JNZ,
		
		/* IO */
		/**
		 * Read one byte from a stream and write to cell at the given 
		 * offset.
		 *
		 * @param[in] x The memory offset to write the byte to as 
		 * <code>int32_t</code>.
		 */
		IN,
		
		/**
		 * Write the byte at the given memory offset to a stream.
		 *
		 * @param[in] x The memory offset to read the byte from as
		 * <code>int32_t</code>.
		 */
		OUT,
		
		/* Miscellaneous */
		/**
		 * No operation.
		 */
		NOP,
	} opcode;
	
	/**
	 * The first operand of this instruction, its type depending on the opcode 
	 * of the operation.
	 */
	union {
		/**
		 * The memory or value offset.
		 */
		int32_t offset;
	
		/**
		 * The jump target.
		 */
		struct BrainfuckInstruction *target;
	} x;

	/**
	 * The second operand of this instruction, its type depending on the opcode
	 * of the operation.
	 */
	union {
		/**
		 * The memory or value offset.
		 */
		int32_t offset;

		/**
		 * The jump flags.
		 */
		uint32_t flags;
	} y;

	/**
	 * The third operand of this instruction, its type depending on the opcode
	 * of the operation.
	 */
	 union {
		/**
		 * The multiplier for a multiplication loop.
		 */
		int32_t multiplier;
	 } z;
};

/**
 * Allocate a {@link BrainfuckInstruction} on the heap.
 *
 * @return A pointer to the allocated structure or 
 *	<code>NULL</code> if the allocation failed.
 */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void);

/** 
 * Deallocate the given {@link BrainfuckInstruction}.
 * 
 * @param[in] instruction The instruction to deallocate.
 */
void brainfuck_instruction_dealloc(struct BrainfuckInstruction *instruction);

/**
 * A {@link BrainfuckScript} is a collection of {@link BrainfuckInstruction}s.
 * 
 * @see BrainfuckInstruction
 */
struct BrainfuckScript {
	/**
	 * The head of the instruction list in this script.
	 */
	struct BrainfuckInstruction *head;
	
	/**
	 * The tail of the instruction list in this script.
	 */
	struct BrainfuckInstruction *tail;
};

/**
 * Allocate a {@link BrainfuckScript} on the heap.
 *
 * @return A pointer to the allocated structure or 
 *	<code>NULL</code> if the allocation failed.
 */
struct BrainfuckScript * brainfuck_script_alloc(void);

/** 
 * Deallocate the given script.
 * 
 * @param[in] script The script to deallocate.
 */
void brainfuck_script_dealloc(struct BrainfuckScript *script);
/** @} */

/**
 * This structure contains the current state of a parser.
 * By capturing the state of the parser, it is possible to parse a script in
 * multiple segments.
 */
struct BrainfuckParserContext;

/**
 * Allocate a {@link BrainfuckParserContext} on the heap.
 *
 * @return A pointer to allocated structure or <code>NULL</code> if the 
 *	allocation failed.
 */
struct BrainfuckParserContext * brainfuck_parser_context_alloc(void);

/**
 * Deallocate the given {@link BrainfuckParserContext}.
 * 
 * @param[in] ctx The parser context to deallocate.
 */
void brainfuck_parser_context_dealloc(struct BrainfuckParserContext *ctx);

/**
 * Determine whether the state of the parser is finished and contains no
 * syntax errors.
 *
 * @param[in] ctx The context of the parser.
 * @return <code>true</code> if the given script does not contain syntax errors,
 * 	<code>false</code> otherwise.
 */
int brainfuck_parser_validate(const struct BrainfuckParserContext *ctx);

/**
 * Parse the given string as a segment of a script.
 *
 * This function assumes the {@link BrainfuckParserContext} initialized, and
 * by doing not so results in undefined behaviour.
 *
 * It is not guaranteed that all syntax errors will be detected, since the 
 * parser can only partially parse the script and therefore cannot detect 
 * syntax errors like unclosed brackets.
 * Use {@link brainfuck_parser_validate} after parsing to check for syntax 
 * errors.
 *
 * @param[in] string The string to parse.
 * @param[in] ctx The context of the parser.
 * @param[out] error A pointer to an integer that will be set to either a 
 * success code or an error code.
 */
void brainfuck_parser_parse_string_segment(const char *string, 
	struct BrainfuckParserContext *ctx, int *error);

/**
 * Parse the given string.
 *
 * @param[in] string The string to parse.
 * @param[out] error A pointer to an integer that will be set to either a 
 * success code or an error code.
 * @return A pointer to a {@link BrainfuckScript} or <code>NULL</code> if the
 * parsing failed.
 */
struct BrainfuckScript * brainfuck_parser_parse_string(const char *string, 
	int *error);

/**
 * Parse the given file.
 *
 * @param[in] file The handle to the file to parse.
 * @param[out] error A pointer to an integer that will be set to either a success
 *	or an error code.
 * @return A pointer to a {@link BrainfuckScript} or <code>NULL</code> if the 
 * parsing failed.
 */
struct BrainfuckScript * brainfuck_parser_parse_file(FILE *file, 
	int *error);
/** @} */
#endif /* BRAINFUCK_PARSER_H */
