/*
 * Copyright 2013 Fabian M.
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
#ifndef _BRAINFUCK_H
#define _BRAINFUCK_H

#include <stdio.h>

#define BRAINFUCK_VERSION "2.0"

#define BRAINFUCK_DEBUG_ENABLED 1
#define BRAINFUCK_DEBUG_DISABLED 0

#define BRAINFUCK_TOKEN_PLUS '+'
#define BRAINFUCK_TOKEN_MINUS '-'
#define BRAINFUCK_TOKEN_PREVIOUS '<'
#define BRAINFUCK_TOKEN_NEXT '>'
#define BRAINFUCK_TOKEN_OUTPUT '.'
#define BRAINFUCK_TOKEN_INPUT ','
#define BRAINFUCK_TOKEN_LOOP_START '['
#define BRAINFUCK_TOKEN_LOOP_END ']'



/*
 * Represents an instruction.
 */
typedef struct BrainfuckInstruction {
	/*
	 * The quantity of this instruction.
	 */
	long quantity;
	/*
	 * The type of this instruction.
	 */
	char type;
	/*
	 * The next instruction.
	 */
	struct BrainfuckInstruction *next;
	/*
	 * The first instruction of a loop if this instruction is a loop.
	 */
	struct BrainfuckInstruction *loop;
} BrainfuckInstruction;


/*
 * Structure that keeps the state of the whole Brainfuck interpeter.
 */
typedef struct BrainfuckState {
	/**
	 * The root instruction of the instruction list.
	 */
	struct BrainfuckInstruction *root;
	/**
	 * The last instruction of the interpreter.
	 */
	struct BrainfuckInstruction *head;
	/*
	 * The environment of this state.
	 */
	struct BrainfuckEnvironment *environment;
	/*
	 * The data pointer.
	 */
	char *pointer;
	/*
	 * When zero the interpreter does not show debug messages, otherwise
	 *	it does.
	 */
	int debug;
} BrainfuckState;

/*
 * The callback that will be invoked when the BRAINFUCK_TOKEN_OUTPUT token is found.
 * 
 * @param chr The value of the current cell.
 */
typedef void (*brainfuck_on_output) (const int chr);

/*
 * The callback that will be invoked when the BRAINFUCK_TOKEN_INPUT token is found.
 * 
 * @return The input character.
 */
typedef int (*brainfuck_on_input) (void);

/*
 * Structure that is used as layer between a brainfuck program and the outside.
 * This way you have control over how input enters your program and output leaves
 * 	your program.
 */
typedef struct BrainfuckEnvironment {
	/*
	 * The callback that will be invoked when the BRAINFUCK_TOKEN_OUTPUT token is found.
	 */
	brainfuck_on_output output_handler;
	/*
	 *The callback that will be invoked when the BRAINFUCK_TOKEN_INPUT token is found.
	 */
	brainfuck_on_input input_handler;
} BrainfuckEnvironment;

/*
 * Adds an instruction to the instruction array.
 *
 * @param state The state to use.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_put_instruction(BrainfuckState *, BrainfuckInstruction *);

/*
 * Reads from a stream and converts it into a list of instructions.
 *
 * @param stream The stream to read from.
 * @return The first instruction read.
 */
BrainfuckInstruction * brainfuck_read_stream(FILE *);

/*
 * Reads from a stream until the given character and converts it into a list of instructions.
 *
 * @param stream The stream to read from.
 * @param until If this character is found in the stream, we will quit reading and return.
 */
BrainfuckInstruction * brainfuck_read_stream_until(FILE *, int);

/*
 * Reads a string and converts it into a list of instructions.
 *
 * @param str The string to convert.
 * @return The first instruction read.
 */
BrainfuckInstruction * brainfuck_read_string(char *);

/*
 * Creates a new BrainfuckState.
 *
 * @param debug 0 to disable debug, otherwise debug is enabled.
 */
BrainfuckState * brainfuck_new_state(const int);

/*
 * Ends a list of instructions and removes it from the memory.
 * 
 * @param instruction The start of the instruction list.
 */
void brainfuck_end_instruction_list(BrainfuckInstruction *);

/*
 * Ends a state and removes it from the memory.
 * 
 * @param state The state to end.
 */
void brainfuck_end_state(BrainfuckState *);

/*
 * Runs the brainfuck program with the given state.
 *
 * @param state The state to run.
 * @param instruction The instruction to execute.
 */
void brainfuck_execute(BrainfuckState *, BrainfuckInstruction *);

/*
 * Default output handler which writes the given character to STDOUT and flushes
 *	it.
 *
 * @param chr The character that will be written to STDOUT.
 */
void brainfuck_default_output_handler(int);

/*
 * Default input handler which reads a character from STDIN and returns it.
 *
 * @return The character read from STDIN.
 */
int brainfuck_default_input_handler(void);
#endif
