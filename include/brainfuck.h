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
 * This enum defines the operations a proper implementation of this interface 
 *	should implement.
 */
enum BrainfuckOperation {
	OP_CLEAR,     	/**< Set a tape cell to zero */
	OP_READ,     	/**< Read one character from an input stream into a cell */
	OP_WRITE,    	/**< Write a cell value to an output stream */ 
	OP_PROCEDURE,	/**< Procedure */ 
	OP_LOOP,     	/**< Loop */
};

/**
 * This structure represents a single operation that could be executed. It 
 *	contains the opcode of the operation and the arguments.
 */
struct BrainfuckInstruction {
	/**
	 * The operation of the instruction.
	 */
	enum BrainfuckOperation operation;
	
	/**
	 * This structure represents the changes in memory and index relative to their
	 * 	old values.
	 */
	struct offset {
		/**
		 * The index offset.
		 */
		int index;
	
		/**
		 * The memory offset.
		 */
		int *mem;
	
		/**
		 * The minimium index of the memory offset.
		 */
		int mem_min;
	
		/**
		 * The maximum index of the memory offset.
		 */ 
		int mem_max;
	} offset;
	
	/**
	 * The argument of this instruction, whose type depends on the operation
	 *	of the instruction.
	 */
	union argument {
		/**
		 * The number of times the instruction should be executed.
		 */
		unsigned int n;
	
		/**
		 * The arguments for a {@link BrainfuckOperation#OP_PROCEDURE}-based 
		 *	instruction.
		 */
		struct procedure {
			/**
			 * The head of a linked list containing the instructions of the
			 *	procedure.
			 */
			struct BrainfuckInstruction *head;
		
			/**
			 * The tail of a linked list containing the instructions of the
			 *	procedure.
			 */
			struct BrainfuckInstruction *tail;
		} procedure;
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
void brainfuck_instruction_free(struct BrainfuckInstruction *instruction);

/**
 * A collection of instructions represented as a 
 *	{@link BrainfuckOperation#OP_PROCEDURE} instruction.
 * 
 * @see BrainfuckInstruction
 */
#define BrainfuckScript BrainfuckInstruction

/**
 * Deallocate the given script.
 *
 * The structure and all instructions of this procedure should be 
 *	deallocated by this function.
 *
 * @param script The script to deallocate.
 * @see brainfuck_instruction_procedure_free
 */
#define brainfuck_script_free brainfuck_instruction_procedure_free

/**
 * Add the given instruction to the given script.
 * 
 * @param script The script to add the instruction to.
 * @param instruction The instruction to add to the script.
 * @see brainfuck_instruction_procedure_add
 */
#define brainfuck_script_add brainfuck_instruction_procedure_add

/**
 * Add the given instruction to the given procedure instruction.
 * 
 * @param procedure The procedure to add the instruction to.
 * @param instruction The instruction to add to the procedure.
 */
void brainfuck_instruction_procedure_add(struct BrainfuckInstruction *procedure,
	struct BrainfuckInstruction *instruction);

/**
 * Deallocate a procedure-based instruction.
 * 
 * @param instruction The instruction to deallocate.
 */
void brainfuck_instruction_procedure_free(
	struct BrainfuckInstruction *instruction);

/**
 * Deallocate the tail of a procedure-based instruction.
 * 
 * @param instruction The instruction to deallocate the tail of.
 */
void brainfuck_instruction_procedure_free_tail(
	struct BrainfuckInstruction *instruction);
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
	 * The {@link BrainfuckScript} structure that contains the segments that
	 *	have been parsed already.
	 */
	struct BrainfuckScript *script;

	/**
	 * A stack that contains the active scopes of the script.
	 */
	struct BrainfuckInstruction **scope;
	
	/**
	 * The index of the scope stack.
	 */
	unsigned int scope_index;
	
	/**
	 * The size of the scope stack.
	 */
	size_t scope_size;
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
void brainfuck_parser_context_free(struct BrainfuckParserContext *ctx);

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
 * @param script The script to check for syntax errors.
 * @return <code>1</code> if the given script does not contain syntax errors,
 * 	<code>0</code> otherwise.
 */
int brainfuck_parser_validate(struct BrainfuckScript *script);

/**
 * Parse the given string as a segment of a script.
 *
 * It is not gauranteed that syntax errors will be detected, since the parser
 *	only partially parses the script and therefore cannot detect all syntax 
 *	errors.
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
	unsigned int *mem;
	
	/**
	 * The index in the memory.
	 */
	unsigned int mem_index;
	
	/**
	 * The size of the memory.
	 */
	size_t mem_size;
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
void brainfuck_engine_context_free(struct BrainfuckEngineContext *ctx);

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