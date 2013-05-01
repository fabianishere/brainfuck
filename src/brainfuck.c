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
 */
void brainfuck_put_instruction(BrainfuckState *state, BrainfuckInstruction *instruction) {
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
}

/*
 * Reads from a stream and converts it into a list of instructions.
 *
 * @param stream The stream to read from.
 */
BrainfuckInstruction * brainfuck_read_stream(FILE *stream) {
	BrainfuckInstruction *instruction = (BrainfuckInstruction *) malloc(sizeof(BrainfuckState));
	BrainfuckInstruction *root = instruction;
	char ch;
	char temp;
	
	while ((ch = fgetc(stream)) != EOF) {
		instruction->type = ch;
		instruction->quantity = 1;
		switch(ch) {
		case '+':
		case '-':
			while ((temp = fgetc(stream)) != EOF && (temp == '+' || temp == '-'))
				if (temp == ch)
					instruction->quantity++;
				else
					instruction->quantity--;
			ungetc(temp, stream);
			break;
		case '<':
		case '>':
			while ((temp = fgetc(stream)) != EOF && (temp == '<' || temp == '>'))
				if (temp == ch)
					instruction->quantity++;
				else
					instruction->quantity--;
			ungetc(temp, stream);
			break;
		case '.':
		case ',':
			while ((temp = fgetc(stream)) != EOF && temp == ch)
				instruction->quantity++;
			ungetc(temp, stream);
			break;
		case '[':
			instruction->loop = brainfuck_read_stream(stream);
			break;
		case ']':
			return root;
		default:
			continue;
		}
		instruction->next = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
		instruction = instruction->next;
	}
	instruction->type = ']';
	return root;
}

/*
 * Reads a string and converts it into a list of instructions.
 *
 * @param buffer The string to convert.
 */
BrainfuckInstruction * brainfuck_read_string(char *buffer) {
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
	
	state->pointer = malloc(30000 * sizeof(char));
	state->debug = debug;
	return state;
}

/*
 * Ends a list of instructions and removes it from the memory.
 * 
 * @param instruction The instruction to end.
 */
void brainfuck_end_instruction(BrainfuckInstruction *instruction) {
	BrainfuckInstruction *tmp;
	
	if (instruction == NULL)
		return;
	while(instruction) {
		tmp = instruction;
		brainfuck_end_instruction(tmp->loop);
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
	brainfuck_end_instruction(state->root);
	state->debug = 0;
	state->pointer = 0;
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
	
	if (state->debug)
		printf("running instruction\n");
	while (instruction && instruction->type != ']') {
		if (state->debug)
			printf("got: %c\n", instruction->type);
		switch (instruction->type) {
		case '+':
			*state->pointer += instruction->quantity;
			if (state->debug)
				printf("plus: %i\n", *state->pointer);
			break;
		case '-':
			*state->pointer -= instruction->quantity;
			if (state->debug)
				printf("minus: %i\n", *state->pointer);
			break;
		case '>':
			state->pointer += instruction->quantity;
			if (state->debug)
				printf("next\n");
			break;
		case '<':
			state->pointer -= instruction->quantity;
			if (state->debug)
				printf("minus\n");
			break;
		case '.':
			for (l = 0; l < instruction->quantity; l++) {
				if (state->debug)
					printf("output: %c\n", *state->pointer);
				else
					putchar(*state->pointer);
				fflush(stdout);
			}
			break;
		case ',':
			for (l = 0; l < instruction->quantity; l++)
				*state->pointer = getchar();
			break;
		case '[':
			if (state->debug)
				printf("loop\n");
			while(*state->pointer)
				brainfuck_execute(state, instruction->loop);
			if (state->debug)
				printf("end\n");
			break;
		default:
			return;
		}
		instruction = instruction->next;
	} 
}
