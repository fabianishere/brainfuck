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

#define BRAINFUCK_DEBUG_ENABLED 1
#define BRAINFUCK_DEBUG_DISABLED 0


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
 * Adds an instruction to the instruction array.
 *
 * @param state The state to use.
 * @param instruction The instruction to add.
 */
void brainfuck_put_instruction(BrainfuckState *, BrainfuckInstruction *);

/*
 * Reads from a stream and converts it into a list of instructions.
 *
 * @param stream The stream to read from.
 * @return The first instruction read.
 */
BrainfuckInstruction * brainfuck_read_stream(FILE *);

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
 * @param instruction The instruction to end.
 */
void brainfuck_end_instruction(BrainfuckInstruction *);

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
#endif
