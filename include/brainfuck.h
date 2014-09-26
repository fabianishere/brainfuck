/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Fabian M.
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

#define BRAINFUCK_VERSION "3.0.0"

#define BRAINFUCK_OK 1 /* Everything is OK */
#define BRAINFUCK_EOF EOF /* End of file */
#define BRAINFUCK_ENOMEM -5 /* Out of memory */
#define BRAINFUCK_ESYNTAX -6 /* Syntax error */
#define BRAINFUCK_EPARAM -7 /* Illegal parameter */
#define BRAINFUCK_EBOUNDS -8 /* Index is out of bounds */
#define BRAINFUCK_EINTERNAL -9 /* Internal error */

#define BRAINFUCK_ICELL 1 /* Cell value mutation */
#define BRAINFUCK_IINDEX 2 /* Index mutation */
#define BRAINFUCK_IOUTPUT 3 /* Write cell value to the output */
#define BRAINFUCK_IINPUT 4 /* Set cell value to the retrieved input */
#define BRAINFUCK_ILOOP 5 /* Indicate the start of a loop */
#define BRAINFUCK_IBREAK 6 /* Indicate the end of a loop */

#define BRAINFUCK_DEFAULT_MEMSIZE 30000

/*
 * A {@link BrainfuckScript} is the head of a linked list of instructions.
 * 
 * @see BrainfuckInstruction
 */
#define BrainfuckScript BrainfuckInstruction
#define brainfuck_script_free brainfuck_instruction_free

/*
 * A {@link BrainfuckInstrcution} represents an instruction that can be 
 * 	executed and is part of a linked list of instructions created by a 
 *	parser.
 */
struct BrainfuckInstruction {
	
	/* 
	 * The id of this instruction.
	 */
	unsigned char id;

	/*
	 * The attributes of this instruction.
	 */
	union attributes {		
		/*
		 * Either the difference between de current cell value or 
		 * 	index and the next one or the amount of times the 
		 * 	execution of this instruction should be repeated.
		 */
		int delta;
		
		/*
		 * A pointer to a {@link BrainfuckInstruction} that we need to 
		 * 	jump back to if this instruction is a break 
		 *	instruction.
		 */
		struct BrainfuckInstruction *jump;
	} attributes;
	
	/*
	 * The next instruction in the linked list of instructions.
	 */
	struct BrainfuckInstruction *next;
	
} BrainfuckInstruction;

/*
 * Allocate a new {@link BrainfuckInstruction} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void);

/* 
 * Free the given {@link BrainfuckInstruction} from the memory.
 * 
 * @param instruction The instruction to free from the memory.
 */
void brainfuck_instruction_free(struct BrainfuckInstruction *instruction);

/*
 * Create a cell value mutation instruction and return it.
 *
 * @param delta The difference between the current cell value and the value
 * 	that is going to be set.
 * @return The cell value mutation instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_cell_mutation(
	int delta);

/*
 * Create an index mutation instruction and return it.
 *
 * @param delta The difference between the current index and the index
 * 	that is going to be set.
 * @return The index mutation instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_index_mutation(
	int delta);

/*
 * Create an output instruction and return it.
 *
 * @param times The amount of times this instruction will be executed.
 * @return The output instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_output(int times);

/*
 * Create an input instruction and return it.
 *
 * @param times The amount of times this instruction will be executed.
 * @return The input instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_input(int times);

/*
 * Create a loop instruction and return it.
 *
 * @return The loop instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_loop(void);

/*
 * Create a break instruction and return it.
 *
 * @param jump The jump of this break instruction.
 * @return The break instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_break(
	struct BrainfuckInstruction *jump);

/*
 * A {@link BrainfuckStream} is a stream from which we read characters or to
 *	which we write characters.
 */
struct BrainfuckStream {

	/*
	 * Read a character from the stream and return it.
	 * 
	 * @param stream The stream to unget the character from.
	 * @return The character read from the stream.
	 */
	int (*get)(struct BrainfuckStream *stream);
	
	/*
	 * Unget a character from the stream.
	 *
	 * @param stream The stream to unget the character from.
	 * @param character The character to unget.
	 */
	void (*unget)(struct BrainfuckStream *stream, int character);

} BrainfuckStream;

/*
 * Cast the given variable to {@link BrainfuckStream}.
 *
 * @param var The variable to cast to {@link BrainfuckStream}.
 */
#define brainfuck_stream_cast(var) ((struct BrainfuckStream *) var)

/*
 * A {@link BrainfuckBufferStream} is a stream that is connected to a memory
 *	buffer.
 */
struct BrainfuckBufferStream {
	
	/*
	 * The base {@link BrainfuckStream} structure.
	 */
	struct BrainfuckStream base;
	
	/*
 	 * The memory buffer of this {@link BrainfuckBufferStream}.
	 */
	char *buffer;

	/*
 	 * The index in memory buffer of this {@link BrainfuckBufferStream}.
	 */
	int index;
	
	/*
	 * The length of the memory buffer of this {@link BrainfuckBufferStream}.
	 */
	int length;
	
} BrainfuckBufferStream;

/*
 * Cast the given variable to {@link BrainfuckBufferStream}.
 *
 * @param var The variable to cast to {@link BrainfuckBufferStream}.
 */
#define brainfuck_stream_buffer_cast(var) ((struct BrainfuckBufferStream *) var)

/*
 * Allocate a new {@link BrainfuckBufferStream} from the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckBufferStream * brainfuck_stream_buffer_alloc(void);

/* 
 * Free the given {@link BrainfuckBufferStream} from the memory.
 * 
 * @param stream The stream to free from the memory.
 */
void brainfuck_stream_buffer_free(struct BrainfuckBufferStream *stream);

/*
 * Initialise the given {@link BrainfuckBufferStream} with
 * 	the given buffer, index and length.
 *
 * @param stream The {@ink BrainfuckBufferStream} to initialise.
 * @param buffer The memory buffer of this {@link BrainfuckBufferStream}.
 * @param index The index in memory buffer of this 
 *	{@link BrainfuckBufferStream}.
 * @param length The length of the memory buffer of this 
 *	{@link BrainfuckBufferStream}.
 */
void brainfuck_stream_buffer_init(struct BrainfuckBufferStream *stream, 
	char *buffer, int index, int length);

/*
 * Create a new {@link BrainfuckBufferStream} with the given string.
 *
 * @param string The string to create the new {@link BrainfuckBufferStream} 
 * 	with.
 * @return The created {@link BrainfuckBufferStream}.
 */
struct BrainfuckBufferStream * brainfuck_stream_string(char *string);

/*
 * A {@link BrainfuckFileStream} is a stream that is connected to a file.
 */
struct BrainfuckFileStream {
	
	/*
	 * The base {@link BrainfuckStream} structure.
	 */
	struct BrainfuckStream base;
	
	/*
	 * The {@link FILE} structure to read from and write to.
	 */
	FILE *file;
	
} BrainfuckFileStream;

/*
 * Cast the given variable to {@link BrainfuckFileStream}.
 *
 * @param var The variable to cast to {@link BrainfuckFileStream}.
 */
#define brainfuck_stream_file_cast(var) ((struct BrainfuckFileStream *) var)

/*
 * Allocate a new {@link BrainfuckFileStream} from the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckFileStream * brainfuck_stream_file_alloc(void);

/* 
 * Free the given {@link BrainfuckStringStream} from the memory.
 * 
 * @param stream The stream to free from the memory.
 */
void brainfuck_stream_file_free(struct BrainfuckFileStream *stream);

/*
 * Initialise the given {@link BrainfuckFileStream} with
 * 	the given {@link FILE}.
 *
 * @param stream The {@ink BrainfuckFileStream} to initialise.
 * @param file The {@link FILE} of the file stream.
 */
void brainfuck_stream_file_init(struct BrainfuckFileStream *stream, FILE *file);

/*
 * Create a new {@link BrainfuckFileStream} with the given file.
 *
 * @param file The file to create the new file stream with.
 * @return The file stream.
 */
struct BrainfuckFileStream * brainfuck_stream_file(FILE *file);

/*
 * A {@link BrainfuckContext} structure is passed to the execution
 *	engine and provides communication for the execution engine with the 
 * 	outside, like providing the input and output and memory management.
 */
struct BrainfuckContext {
	
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
	int (*write)(int character);
	
	/*
	 * The size of the allocated memory block.
	 */
	size_t mem_size;
	
	/*
	 * Pointer to the allocated memory a program can work with.
	 */
	int *memory;
	
	/*
	 * The current index in the memory.
	 */
	unsigned long index;
	
	/*
	 * This flag is set to <code>1</code> when a program is being run using
	 * 	this context. If set to <code>0</code> by another thread, the 
	 *	execution of instructions will stop.
	 */
	int running;
	
	/*
	 * The {@link BrainfuckInstruction} that is currently being executed by
	 *	the execution engine.
	 */
	struct BrainfuckInstruction *instruction;
	
} BrainfuckContext;

/*
 * Allocate a new {@link BrainfuckContext} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckContext * brainfuck_context_alloc(void);

/* 
 * Free the given {@link BrainfuckContext} from the memory.
 * 
 * @param ctx The context to free from the memory.
 */
void brainfuck_context_free(struct BrainfuckContext *ctx);

/*
 * Return a new {@link BrainfuckContext} initialised with the default
 * 	values.
 * 
 * @return The default {@link BrainfuckContext}.
 */
struct BrainfuckContext * brainfuck_context_default(void);

/* 
 * Parse the given string.
 *
 * @param string The string to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_string(char *string, int *error);

/* 
 * Parse the given {@link FILE}.
 *
 * @param string The string to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_file(FILE *file, int *error);

/* 
 * Parse the given {@link BrainfuckStream}.
 *
 * @param stream The stream to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse(struct BrainfuckStream *stream, 
	int *error);

/*
 * Run the given {@link BrainfuckScript} with the given 
 *	{@link BrainfuckContext}.
 *
 * @param script The script to run.
 * @param ctx The execution context that will provide the memory management and
 *	the environment for the execution. 
 * @return a integer with a value of zero or higher if the script executed 
 *	successfully, a value lower than zero otherwise.
 */
int brainfuck_run(struct BrainfuckScript *script, 
	struct BrainfuckContext *ctx);

#endif /* BRAINFUCK_H */
