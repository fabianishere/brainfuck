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
#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include <stdio.h>

/* Library information */
#define BRAINFUCK_VERSION "3.0.0"

/* Default values */
#define BRAINFUCK_DMEMSIZE 30000   	/* Size of the allocated memory block */ 
#define BRAINFUCK_DINITIALDEPTH 10 	/* Initial scope depth */
#define BRAINFUCK_DMAXDEPTH 8      	/* Maximum scope depth */
#define BRAINFUCK_DFILEBUFSIZE 1024	/* Size of the file buffer */

/* Error codes */
#define BRAINFUCK_EOK 1     	/* Everything is OK */
#define BRAINFUCK_ENOMEM -5 	/* Out of memory */
#define BRAINFUCK_ESYNTAX -6	/* Syntax error */
#define BRAINFUCK_EBOUNDS -7	/* Index is out of bounds */

/**
 * The {@link BrainfuckType} enum contains the legal types 
 *	{@link BrainfuckInstruction}s can have.
 */
enum BrainfuckType {
	MUTATE,		/* Cell value mutation */
	MOVE, 		/* Memory index mutation */
	READ,		/* Read one character from the input stream into the cell */
	WRITE,		/* Write cell value to output stream */ 
	PROCEDURE,	/* Procedure construct */ 
	LOOP		/* Loop construct */
} BrainfuckType;

/**
 * A {@link BrainfuckInstrcution} represents an instruction that can be 
 * 	executed and is part of a linked list of instructions created by a 
 *	parser.
 */
struct BrainfuckInstruction;

/**
 * Allocate a new {@link BrainfuckInstruction} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void);

/** 
 * Free the given {@link BrainfuckInstruction} from the heap.
 * 
 * @param instruction The instruction to free from the heap.
 */
void brainfuck_instruction_free(struct BrainfuckInstruction *instruction);

/**
 * A {@link BrainfuckScript} should be a {@link BrainfuckInstruction} with type
 *	PROCEDURE, that contains the instructions of the script.
 * 
 * @see BrainfuckInstruction
 */
#define BrainfuckScript BrainfuckInstruction
#define brainfuck_script_free brainfuck_instruction_free_procedure

/**
 * Initialise the given instruction as MUTATE instruction.
 *
 * @param instruction The instruction to initialise.
 * @param dx The change in cell value.
 */
void brainfuck_instruction_mutate(
	struct BrainfuckInstruction *instruction, const int dx);

/**
 * Initialise the given instruction as MOVE instruction.
 *
 * @param instruction The instruction to initialise.
 * @param dy the change in index.
 */
void brainfuck_instruction_move(
	struct BrainfuckInstruction *instruction, const int dy);

/**
 * Initialise the given instruction as READ instruction.
 *
 * @param instruction The instruction to initialise.
 * @param k The amount of times this instruction will be executed.
 * @return The READ instruction that has been created or <code>NULL</code>
 *	if the creation failed.
 */
void brainfuck_instruction_read(
	struct BrainfuckInstruction *instruction, const unsigned int k);

/**
 * Initialise the given instruction as WRITE instruction.
 *
 * @param instruction The instruction to initialise.
 * @param k The amount of times this instruction will be executed.
 * @return The READ instruction that has been created or <code>NULL</code>
 *	if the creation failed.
 */
void brainfuck_instruction_write(
	struct BrainfuckInstruction *instruction, const unsigned int k);

/**
 * Initialise the given instruction as PROCEDURE instruction.
 *
 * @param instruction The instruction to initialise.
 */
void brainfuck_instruction_procedure(struct BrainfuckInstruction *instruction);

/**
 * Initialise the given instruction as LOOP instruction.
 *
 * @param instruction The instruction to initialise.
 */
void brainfuck_instruction_loop(struct BrainfuckInstruction *instruction);

/**
 * Free the tail of the given {@link BrainfuckInstruction.}
 * 
 * @param instruction The instruction to free the tail from the heap.
 */
void brainfuck_instruction_free_tail(struct BrainfuckInstruction *instruction);

/**
 * Free a procedure (or procedure-based) instruction from the heap.
 * 
 * @param instruction The instruction to free from the heap.
 */
void brainfuck_instruction_free_procedure(
	struct BrainfuckInstruction *instruction);

/**
 * The {@link BrainfuckParserState} struct holds the state of the parser.
 */
struct BrainfuckParserState;

/**
 * Allocate a new {@link BrainfuckParserState} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckParserState * brainfuck_parser_state_alloc(void);

/**
 * Free the given {@link BrainfuckParserState} from the heap.
 * 
 * @param state The state to free from the heapy.
 */
void brainfuck_parser_state_free(struct BrainfuckParserState *state);

/**
 * Parse the given string with the given state.
 *
 * @param string The string to read the script from.
 * @param state The state of the parser.
 * @param error A pointer to an integer that will be set to either a success
 *	or an error code.
 */
void brainfuck_parse_string_state(
		const char *string, struct BrainfuckParserState *state, int *error);

/**
 * Parse the given string.
 *
 * @param string The string to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_string(const char *string, int *error);

/**
 * Parse the given file.
 *
 * @param file The file to read the script from.
 * @param error A pointer to an integer that will be set to either a success
 *	or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code>
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_file(FILE *file, int *error);

/**
 * A {@link BrainfuckExecutionContext} structure is passed to the execution
 *	engine and provides communication for the execution engine with the 
 *  environment, for example providing the input and output and 
 *	memory management.
 */
struct BrainfuckExecutionContext {
	
	/*
	 * Pointer to a function which can read from the environment's input.
	 *
	 * @return The character that was read from the environment's input.
	 */
	int (*read)(void);

	/*
 	 * Pointer to a function which can write to the environment's output.
 	 * 
 	 * @param character The character to write to the environment's output.
  	 * @return On success, the written character is returned. If a writing 
 	 * 	error occurs, BRAINFUCK_EOF is returned.
 	 */
	int (*write)(const int character);
	
	/*
	 * The size of the allocated memory block.
	 */
	size_t mem_size;
	
	/*
	 * Pointer to the allocated memory the program can work with.
	 */
	unsigned int *memory;
	
	/*
	 * The current index in the memory.
	 */
	unsigned long index;
	
} BrainfuckExecutionContext;

/**
 * Allocate a new {@link BrainfuckExecutionContext} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckExecutionContext * brainfuck_execution_context_alloc(void);

/**
 * Free the given {@link BrainfuckExecutionContext} from the heap.
 * 
 * @param ctx The context to free from the heapy.
 */
void brainfuck_execution_context_free(struct BrainfuckExecutionContext *ctx);

/**
 * Initialise the given {@link BrainfuckExecutionContext}.
 * 
 * @param ctx The execution context to initialise.
 * @param read The read function.
 * @param write The write function.
 * @param mem_size The size of the memory block.
 * @param memory The memory block the program can work with.
 * @param index The initial index.
 */
void brainfuck_execution_context_init(
	struct BrainfuckExecutionContext *ctx, int (*read)(void), 
	int (*write)(const int character), const size_t mem_size, 
	unsigned int *memory, const unsigned long index);

/**
 * Interpret the instructions of the given {@link BrainfuckScript} with the
 *	given {@link BrainfuckExecutionContext}.
 *
 * @param script The script to interpret.
 * @param ctx The execution context that will provide environment.
 * @return a integer with a value of zero or higher if the script executed 
 *	successfully, a value lower than zero otherwise.
 */
int brainfuck_execution_interpret(const struct BrainfuckScript *script, 
	struct BrainfuckExecutionContext *ctx);

#endif /* BRAINFUCK_H */
