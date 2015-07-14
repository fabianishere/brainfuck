/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Fabian M.
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
 * This header file resembles a specification of a proper implementation of the
 *	brainfuck language.
 */
#ifndef BRAINFUCK_H
#define BRAINFUCK_H

/* Dependencies */
#include <stdio.h>

/**
 * The definitions used in this specification.
 *
 * @addtogroup definitions Definitions
 * @{
 */
/* General information */
#define BRAINFUCK_VERSION "3.0.0" /**< Version of the specification */

/* Error codes */
#define BRAINFUCK_EOK 1     	/**< Indicates everything is OK */
#define BRAINFUCK_ENOMEM -5 	/**< Indicates there is no memory available */
#define BRAINFUCK_ESYNTAX -6	/**< Indiactes a syntax error */
#define BRAINFUCK_EBOUNDS -7	/**< Indiactes the index is out of bounds */
/** @} */

/**
 * The abstract syntax tree that should be utilized by a proper implementation.
 *
 * @addtogroup ast Abstract Syntax Tree
 * @{
 */
/**
 * This structure represents a single operation that could be executed. It 
 *	contains the opcode of the operation and the arguments.
 */
struct BrainfuckInstruction {
	/**
	 * This enum defines the instruction set that should be implemented by a
	 *	proper implementation of this specification.
	 */
	enum BrainfuckType {
		/* Data Manipulation */
		INC,	/**< Increase cell value */
		MOV,	/**< Move in the memory */
		CLR,	/**< Clear cell value */
		/* IO */
		IN, 	/**< Read one byte from a stream and write to cell */
		OUT,	/**< Read one byte from cell and write to a stream */
		/* Control Flow */
		JMP,	/**< Unconditional jump */ 
		JZ, 	/**< Conditional jump: if zero */
		/* Util */
		NOP,	/**< No operation */
	} type;
	
	/**
	 * The argument of this instruction, whose type depends on the operation
	 *	of the instruction.
	 */
	union BrainfuckArgument {
		/**
		 * The change in cell value.
		 */
		int delta;

		/**
		 * The number of times an instruction should be executed.
		 */
		unsigned int n;

		/**
		 * The instruction to jump to in case of a {@link BrainfuckType#JUMP}
		 *	based instruction.
		 */
		struct BrainfuckInstruction *jump;
	} argument;

	/**
	 * The instruction that follows this instruction.
 	 */
	struct BrainfuckInstruction *next;
};

/**
 * Allocate a {@link BrainfuckInstruction} structure.
 *
 * @return A pointer to the allocated structure or 
 *	<code>NULL</code> if the allocation failed.
 */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void);

/** 
 * Deallocate the given instruction.
 * 
 * @param instruction The instruction to deallocate.
 */
void brainfuck_instruction_dealloc(struct BrainfuckInstruction *instruction);

/**
 * A {@link BrainfuckScript} structure should be the first instruction in a 
 *	linked list of instructions.
 * 
 * @see BrainfuckInstruction
 */
#define BrainfuckScript BrainfuckInstruction

/** 
 * Deallocate the given script.
 * 
 * @param script The script to deallocate.
 * @see brainfuck_instruction_dealloc
 */
#define brainfuck_script_dealloc brainfuck_instruction_dealloc
/** @} */

/**
 * The parser interface that should be implemented by a proper implementation.
 *
 * @addtogroup parser Parser
 * @{
 */
/**
 * This structure contains the state of the parser.
 * By capturing the state of the parser, it is possible to parse a script in
 *	segments.
 */
struct BrainfuckParserContext {
	/**
	 * The first {@link BrainfuckInstruction} parsed within the context.
	 */
	struct BrainfuckInstruction *head;
	
	/**
 	* The last {@link BrainfuckInstruction} parsed within the context.
 	*/
	struct BrainfuckInstruction *tail;
	
	/**
	 * A stack that contains addresses to jump to for loop instructions.
	 */
	struct BrainfuckInstruction **loop;
	
	/**
	 * The index of the loop stack.
	 */
	unsigned int loop_index;
	
	/**
	 * The size of the loop stack.
	 */
	size_t loop_size;
};

/**
 * Allocate a {@link BrainfuckParserContext} structure.
 *
 * @return A pointer to allocated structure or  <code>NULL</code> if the 
 *	allocation failed.
 */
struct BrainfuckParserContext * brainfuck_parser_context_alloc(void);

/**
 * Deallocate the given {@link BrainfuckParserContext} structure.
 * 
 * @param ctx The context to deallocate.
 */
void brainfuck_parser_context_dealloc(struct BrainfuckParserContext *ctx);

/**
 * Initialize a {@link BrainfuckParserContext} structure.
 *
 * @param ctx The {@link BrainfuckParserContext} structure to initialize.
 * @return {@link BRAINFUCK_EOK} on success, one of the defined error codes
 *	on failure.
 */
int brainfuck_parser_context_init(struct BrainfuckParserContext *ctx);

/**
 * Validate a script and detect syntax errors in the given script.
 *
 * @param ctx The context of the parser.
 * @return <code>true</code> if the given script does not contain syntax errors,
 * 	<code>false</code> otherwise.
 */
int brainfuck_parser_validate(struct BrainfuckParserContext *ctx);

/**
 * Parse the given string as a segment of a script.
 *
 * It is not gauranteed that all syntax errors will be detected, since the 
 *	parser can only partially parse the script and therefore cannot detect 
 *	syntax errors like unclosed brackets.
 * Use {@link brainfuck_parser_validate} to validate a script after parsing.
 *
 * @param string The string to parse.
 * @param ctx The context of the parser.
 * @param error A pointer to an integer that will be set to either a success 
 *	code or an error code.
 */
void brainfuck_parser_parse_string_partial(const char *string, 
	struct BrainfuckParserContext *ctx, int *error);

/**
 * Parse the given string.
 *
 * @param string The string to parse.
 * @param error A pointer to an integer that will be set to either a 
 * 	success code or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parser_parse_string(const char *string, 
	int *error);

/**
 * Parse the given file.
 *
 * @param file The file to parse.
 * @param error A pointer to an integer that will be set to either a success
 *	or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code>
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parser_parse_file(FILE *file, int *error);
/** @} */


/**
 * The engine interface that should be implemented by a proper implementation.
 *
 * @addtogroup engine Engine
 * @{
 */
/**
 * This structure represents the context in which a script should run.
 */
struct BrainfuckEngineContext {
	/**
	 * Read a character from an input stream.
	 *
	 * @return The integer that was read from the input stream.
	 */
	int (*read)(void);

	/**
	 * Write a character to an output stream.
	 * 
	 * @param character An integer to write to the output stream.
	 * @return On success, the written character is returned. If a writing 
	 * 	error occurs, EOF is returned.
	 */
	int (*write)(const int character);
	
	/** 
	 * The memory a script should operate on.
	 */
	unsigned int *memory;
};

/**
 * Allocate a {@link BrainfuckEngineContext} structure.
 * 
 * The fields of the allocted {@link BrainfuckEngineContext} should be left
 * 	uninitialized, as this should be done by the consumer of the library.
 *
 * @return A pointer to the allocated structure or <code>NULL</code> if the 
 *	allocation failed.
 */
struct BrainfuckEngineContext * brainfuck_engine_context_alloc(void);

/**
 * Free the engine context.
 * 
 * The {@link BrainfuckEngineContext#memory} field should not be deallocated
 *	by this function, as this should be done by the consumer of the library.
 * 
 * @param ctx The context to deallocate.
 */
void brainfuck_engine_context_dealloc(struct BrainfuckEngineContext *ctx);

/**
 * Run the given script.
 *
 * @param script The script to run.
 * @param ctx The context in which the execution takes place.
 * @return {@link BRAINFUCK_EOK} on success, one of the defined error codes
 *	on failure.
 */
int brainfuck_engine_run(const struct BrainfuckScript *script, 
	struct BrainfuckEngineContext *ctx);
/** @} */

#endif /* BRAINFUCK_H */