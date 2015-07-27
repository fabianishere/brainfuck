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
#define BRAINFUCK_EOK      1	/**< Indicates everything is OK */
#define BRAINFUCK_ENOMEM  -5	/**< Indicates there is no memory available */
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
		DEC,	/**< Decrease cell value */
		MOVL,	/**< Move left in the memory */
		MOVR,	/**< Move right in the memory */
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
		 * The offset in cell value or instruction address.
		 */
		int offset;

		/**
		 * The number of times an instruction should be executed.
		 */
		unsigned int n;
	} argument;
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
 * A {@link BrainfuckScript} is a collection of {@link BrainfuckInstruction}
 * 	structures.
 * 
 * @see BrainfuckInstruction
 */
struct BrainfuckScript {
	/**
	 * The index within the array.
	 */
	int index;
	
	/**
	 * The amount of instructions in this collection.
	 */
	size_t size;
	
	/**
	 * The array containing the instructions.
	 */
	struct BrainfuckInstruction *array;
	
	/**
 	 * The previous collection of instructions.
 	 */
	struct BrainfuckScript *previous;
	
	/**
	 * The next collection of instructions if this collection is not large
	 *	enough.
	 */
	struct BrainfuckScript *next;
};

/**
 * Allocate a {@link BrainfuckScript} structure.
 *
 * @return A pointer to the allocated structure or 
 *	<code>NULL</code> if the allocation failed.
 */
struct BrainfuckScript * brainfuck_script_alloc(void);

/** 
 * Deallocate the given script including the structures referenced by the
 *  <code>next</code> field.
 * 
 * @param script The script to deallocate.
 */
void brainfuck_script_dealloc(struct BrainfuckScript *script);

/**
 * Initialize a {@link BrainfuckScript} structure.
 *
 * @param script The {@link BrainfuckScript} structure to initialize.
 * @param previous The previous {@link BrainfuckScript} structure.
 * @param size The size of the array to allocate.
 * @return {@link BRAINFUCK_EOK} on success, one of the defined error codes
 *	on failure.
 */
int brainfuck_script_init(struct BrainfuckScript *script, 
	struct BrainfuckScript *previous, size_t size);

/**
 * Dump the AST of the given {@link BrainfuckScript} structure.
 *
 * @param script The script to dump.
 * @param file A pointer to a {@link FILE} object to write the script to.
 */
void brainfuck_script_dump(const struct BrainfuckScript *script, FILE *file);
/** @} */

/**
 * A parser interface for the brainfuck language.
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
	 * The first {@link BrainfuckScript} parsed within the context.
	 */
	struct BrainfuckScript *head;
	
	/**
 	* The last {@link BrainfuckScript} parsed within the context.
 	*/
	struct BrainfuckScript *tail;
	
	/**
	 * A stack containing the jump offsets.
	 */
	struct BrainfuckOffsetStack {
		/**
		* The index of the stack.
		*/
		int index;
	
		/**
		* The size of the stack.
		*/
		size_t size;		
		
		/**
		 * The current offset.
		 */
		int current;
		
		/**
		 * The underlying array of this stack.
		 */
		struct BrainfuckInstruction **array;
	} offsets;
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
int brainfuck_parser_validate(const struct BrainfuckParserContext *ctx);

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
struct BrainfuckScript * brainfuck_parser_parse_file(FILE *file, 
	int *error);
/** @} */

/**
 * Passes that can perform transformations to or analysis of the abstract
 * 	syntax tree.
 *
 * @addtogroup pass Pass
 * @{
 */
/**
 * Apply a 'clear' optimization to the given script by transformating a
 * 	common clear operation ([-] or [+]) into a single {@link BrainfuckType#CLR}
	instruction.
 *
 * @param script The script to apply the optimization pass to.
 * @return {@link BRAINFUCK_EOK} on success, one of the defined error codes
 *	on failure.
 */
int brainfuck_pass_clear(const struct BrainfuckScript *script);
/** @} */
	
/**
 * An interface of an engine that is able to run a {@link BrainfuckScript}.
 *
 * @addtogroup engine Engine
 * @{
 */
/**
 * This structure represents the context in which a script should run.
 */
struct BrainfuckEngineContext {
	/** 
	 * The memory a script should operate on.
	 */
	unsigned char *memory;
	
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