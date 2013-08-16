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

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "brainfuck.h"

/*
 * Adds an instruction to the instruction array.
 *
 * @param state The state to use.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_put_instruction(BrainfuckState *state, BrainfuckInstruction *instruction) {
	if (instruction == NULL)
		return NULL;
	BrainfuckInstruction *root = instruction;
	if (!state->root)
		state->root = instruction;
	if (state->head)
		state->head->next = instruction;
	while (instruction) {
		if (!instruction->next) {
			state->head = instruction;
			break;
		}
		instruction = instruction->next;
	}
	return root;
}

/*
 * Reads from a stream and converts it into a list of instructions.
 *
 * @param stream The stream to read from.
 */
BrainfuckInstruction * brainfuck_read_stream(FILE *stream) {
	return brainfuck_read_stream_until(stream, EOF);
}

/*
 * Reads from a stream until the given character and converts it into a list of instructions.
 *
 * @param stream The stream to read from.
 * @param until If this character is found in the stream, we will quit reading and return.
 */
BrainfuckInstruction * brainfuck_read_stream_until(FILE *stream, int until) {
	BrainfuckInstruction *instruction = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
	instruction->next = 0;
	instruction->loop = 0;
	BrainfuckInstruction *root = instruction;
	char ch;
	char temp;
	
	while ((ch = fgetc(stream)) != until) {
		instruction->type = ch;
		instruction->quantity = 1;
		switch(ch) {
		case BRAINFUCK_TOKEN_PLUS:
		case BRAINFUCK_TOKEN_MINUS:
			while ((temp = fgetc(stream)) != until && (temp == BRAINFUCK_TOKEN_PLUS || temp == BRAINFUCK_TOKEN_MINUS))
				if (temp == ch)
					instruction->quantity++;
				else
					instruction->quantity--;
			ungetc(temp, stream);
			break;
		case BRAINFUCK_TOKEN_NEXT:
		case BRAINFUCK_TOKEN_PREVIOUS:
			while ((temp = fgetc(stream)) != until && (temp == BRAINFUCK_TOKEN_NEXT || temp == BRAINFUCK_TOKEN_PREVIOUS))
				if (temp == ch)
					instruction->quantity++;
				else
					instruction->quantity--;
			ungetc(temp, stream);
			break;
		case BRAINFUCK_TOKEN_OUTPUT:
		case BRAINFUCK_TOKEN_INPUT:
			while ((temp = fgetc(stream)) != until && temp == ch)
				instruction->quantity++;
			ungetc(temp, stream);
			break;
		case BRAINFUCK_TOKEN_LOOP_START:
			instruction->loop = brainfuck_read_stream_until(stream, until);
			break;
		case BRAINFUCK_TOKEN_LOOP_END:
			return root;
		default:
			continue;
		}
		instruction->next = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
		instruction->next->next = 0;
		instruction->next->loop = 0;
		instruction = instruction->next;
	}
	instruction->type = BRAINFUCK_TOKEN_LOOP_END;
	return root;
}

/*
 * Reads a string and converts it into a list of instructions.
 *
 * @param buffer The string to convert.
 */
BrainfuckInstruction * brainfuck_read_string(char *buffer) {
	if (buffer == NULL)
		return NULL;
	FILE *stream;
	BrainfuckInstruction *instruction;
	
	stream = fmemopen(buffer, strlen(buffer), "r");
	instruction = brainfuck_read_stream(stream);
	fclose(stream);
	return instruction;
}


/*
 * Creates a new state.
 *
 * @param debug 0 to disable debug, otherwise debug is enabled.
 */
BrainfuckState* brainfuck_new_state(int debug) {
	BrainfuckState *state = (BrainfuckState *) malloc(sizeof(BrainfuckState));
	
	state->root = 0;
	state->head = 0;
	state->environment = (BrainfuckEnvironment *) malloc(sizeof(BrainfuckEnvironment));
	state->environment->output_handler = &brainfuck_default_output_handler;
	state->environment->input_handler = &brainfuck_default_input_handler;
	state->pointer = malloc(sizeof(char));
	state->debug = debug;
	return state;
}

/*
 * Ends a list of instructions and removes it from the memory.
 * 
 * @param instruction The start of the instruction list.
 */
void brainfuck_end_instruction_list(BrainfuckInstruction *instruction) {
	BrainfuckInstruction *tmp;
	
	if (instruction == NULL)
		return;
	while(instruction) {
		tmp = instruction;
		brainfuck_end_instruction_list(tmp->loop);
		instruction = instruction->next;
		free(tmp);
		tmp = 0;
	}
}

/*
 * Ends a state and removes it from the memory.
 * 
 * @param state The state to end.
 */
void brainfuck_end_state(BrainfuckState *state) {
	if (state->root) {
		brainfuck_end_instruction_list(state->root);
	}
	state->pointer = 0;
	state->environment->output_handler = 0;
	state->environment->input_handler = 0;
	free(state->environment);
	state->environment = 0;
	free(state->pointer);
	free(state);
	state = 0;
}

/*
 * Runs the brainfuck program with the given state.
 *
 * @param state The state to run.
 * @param instruction The instruction to execute.
 */
void brainfuck_execute(BrainfuckState *state, BrainfuckInstruction *instruction) {
	long l;
	while (instruction && instruction->type != BRAINFUCK_TOKEN_LOOP_END) {
		switch (instruction->type) {
		case BRAINFUCK_TOKEN_PLUS:
			*state->pointer += instruction->quantity;
			break;
		case BRAINFUCK_TOKEN_MINUS:
			*state->pointer -= instruction->quantity;
			break;
		case BRAINFUCK_TOKEN_NEXT:
			state->pointer += instruction->quantity;
			break;
		case BRAINFUCK_TOKEN_PREVIOUS:
			state->pointer -= instruction->quantity;
			break;
		case BRAINFUCK_TOKEN_OUTPUT:
			for (l = 0; l < instruction->quantity; l++) {
				putchar(*state->pointer);
				fflush(stdout);
			}
			break;
		case BRAINFUCK_TOKEN_INPUT:
			for (l = 0; l < instruction->quantity; l++)
				*state->pointer = getchar();
			break;
		case BRAINFUCK_TOKEN_LOOP_START:
			while(*state->pointer)
				brainfuck_execute(state, instruction->loop);
			break;
		default:
			return;
		}
		instruction = instruction->next;
	} 
}

/*
 * Default output handler which writes the given character to STDOUT and flushes
 *	it.
 *
 * @param chr The character that will be written to STDOUT.
 */
void brainfuck_default_output_handler(int chr) {
	putchar(chr);
	fflush(stdout);
}

/*
 * Default input handler which reads a character from STDIN and returns it.
 *
 * @return The character read from STDIN.
 */
int brainfuck_default_input_handler(void) {
	return getchar();
}
