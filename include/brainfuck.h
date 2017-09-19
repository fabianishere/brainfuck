/*
 * Copyright 2016 Fabian Mastenbroek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#define BRAINFUCK_VERSION_MAJOR 2 
#define BRAINFUCK_VERSION_MINOR 7
#define BRAINFUCK_VERSION_PATCH 2
#define BRAINFUCK_TAPE_SIZE 30000
/* 1: EOF leaves cell unchanged; 0: EOF == 0; 1: EOF ==  1 */
#define BRAINFUCK_EOF_BEHAVIOR 1

#define BRAINFUCK_TOKEN_PLUS '+'
#define BRAINFUCK_TOKEN_MINUS '-'
#define BRAINFUCK_TOKEN_PREVIOUS '<'
#define BRAINFUCK_TOKEN_NEXT '>'
#define BRAINFUCK_TOKEN_OUTPUT '.'
#define BRAINFUCK_TOKEN_INPUT ','
#define BRAINFUCK_TOKEN_LOOP_START '['
#define BRAINFUCK_TOKEN_LOOP_END ']'
#ifdef BRAINFUCK_EXTENSION_DEBUG
#	define BRAINFUCK_TOKEN_BREAK '#'
#else
#	define BRAINFUCK_TOKEN_BREAK -10
#endif

#define READLINE_HIST_SIZE 20

/**
 * Represents a brainfuck instruction.
 */
typedef struct BrainfuckInstruction {
	/**
	 * The difference between the value of the byte at the currect pointer and
	 *   the value we want.
	 */
	int difference;
	/**
	 * The type of this instruction.
	 */
	char type;
	/**
	 * The next instruction in the linked list.
	 */
	struct BrainfuckInstruction *next;
	/**
	 * The previous instruction in the linked list.
	 */
	struct BrainfuckInstruction *previous;
	/**
	 * The first instruction of a loop if this instruction is a loop. Otherwise
	 * 	<code>NULL</code>
	 */
	struct BrainfuckInstruction *loop;
} BrainfuckInstruction;

/**
 * The state structure contains the head and the root of the linked list containing
 * 	the instructions of the program.
 */
typedef struct BrainfuckState {
	/**
	 * The root instruction of the instruction linked list.
	 */
	struct BrainfuckInstruction *root;
	/**
	 * The head instruction of the instruction linked list.
	 */
	struct BrainfuckInstruction *head;
} BrainfuckState;

/**
 * The callback that will be invoked when the BRAINFUCK_TOKEN_OUTPUT token is found.
 * 
 * @param chr The value of the current cell.
 */
typedef int (*BrainfuckOutputHandler) (int chr);

/**
 * The callback that will be invoked when the BRAINFUCK_TOKEN_INPUT token is found.
 * 
 * @return The character that is read.
 */
typedef char (*BrainfuckInputHandler) (void);

/**
 * This structure is used as a layer between a brainfuck program and
 * 	the outside. It allows control over input, output and memory.
 */
typedef struct BrainfuckExecutionContext {
	/**
	 * The callback that will be invoked when the BRAINFUCK_TOKEN_OUTPUT token is found.
	 */
	BrainfuckOutputHandler output_handler;
	/**
	 * The callback that will be invoked when the BRAINFUCK_TOKEN_INPUT token is found.
	 */
	BrainfuckInputHandler input_handler;
	/**
	 * An array containing the memory cells the program can use.
	 */
	unsigned char *tape;
	/**
	 * Index into <code>tape</code>. Modified during execution.
	 */
	int tape_index;
	/**
	 * size of the tape in number of cells.
	 */
	size_t tape_size;
	/**
	 * A flag that, if set to true, indicates that execution should stop.
	 */
	int shouldStop;
} BrainfuckExecutionContext;

/**
 * Creates a new state.
 */
BrainfuckState * brainfuck_state();

/**
 * Creates a new context.
 *
 * @param size The size of the tape.
 */
BrainfuckExecutionContext * brainfuck_context(int);

/**
 * Removes the given instruction from the linked list.
 * 
 * @param state The state
 * @param instruction The instruction to remove.
 * @return The instruction that is removed.
 */
BrainfuckInstruction * brainfuck_remove(struct BrainfuckState *, struct BrainfuckInstruction *);

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_add(struct BrainfuckState *state, struct BrainfuckInstruction *);

/**
 * Adds an instruction to the front of the instruction list.
 *
 * @param state The state.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_add_first(struct BrainfuckState *state, struct BrainfuckInstruction *);

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param before The instruction you want to add another instruction before.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_insert_before(struct BrainfuckState *, struct BrainfuckInstruction *, 
	struct BrainfuckInstruction *);

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param after The instruction you want to add another instruction after.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_insert_after(struct BrainfuckState *, struct BrainfuckInstruction *, 
	struct BrainfuckInstruction *);

/**
 * Reads a character, converts it to an instruction and repeats until the EOF character
 * 	occurs and will then return a linked list containing all instructions.
 *
 * @param stream The stream to read from.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_stream(FILE *);

/**
 * Reads a character, converts it to an instruction and repeats until the given character
 * 	occurs and will then return a linked list containing all instructions.
 *
 * @param stream The stream to read from.
 * @param until If this character is found in the stream, we will quit reading and return.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_stream_until(FILE *, int);

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 *
 * @param str The string to read from.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_string(char *);

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 *
 * @param str The string to read from.
 * @param begin The index you want to start parsing at.
 * @param end The index you want to stop parsing at.
 *	When <code>-1</code> is given, it will stop at the end of the string.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_substring(char *, int, int);

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 * This method is special because it uses the begin index as counter, so this variable
 * 	will increase.
 *
 * @param str The string to read from.
 * @param ptr The pointer to the integer holding the index you want to start parsing at.
 *	Since this will be used as counter, the value of the pointer will be increased.
 * @param end The index you want to stop parsing at.
 *	When <code>-1</code> is given, it will stop at the end of the string.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_substring_incremental(char *, int *, int);

/**
 * Converts the given character to an instruction.
 *
 * @param c The character to convert.
 * @param The character that's converted into an instruction.
 */
BrainfuckInstruction * brainfuck_parse_character(char);

/**
 * Destroys the given instruction.
 * 
 * @param instruction The instruction to destroy.
 */
void brainfuck_destroy_instruction(struct BrainfuckInstruction *);

/**
 * Destroys a linked list containing instructions.
 * 
 * @param head The start of the instruction list.
 */
void brainfuck_destroy_instructions(struct BrainfuckInstruction *);

/**
 * Destroys a state.
 * 
 * @param state The state to destroy
 */
void brainfuck_destroy_state(struct BrainfuckState *);

/**
 * Destroys a context.
 * 
 * @param context The context to destroy
 */
void brainfuck_destroy_context(struct BrainfuckExecutionContext *);

/**
 * Executes the given linked list containing instructions.
 *
 * @param root The start of the linked list of instructions you want
 * 	to execute.
 * @param context The context of this execution that contains the tape and
 *	other execution related variables.
 */
void brainfuck_execute(struct BrainfuckInstruction *, struct BrainfuckExecutionContext *);

/**
 * Stops the currently running program referenced by the given execution context.
 *
 * @param context The context of this execution that contains the tape and
 *	other execution related variables.
 */
void brainfuck_execution_stop(BrainfuckExecutionContext *);

/**
 * Reads exactly one char from stdin.
 * @return The character read from stdin. 
 */
char brainfuck_getchar(void);

#endif /* BRAINFUCK_H */
