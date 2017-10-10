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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include <brainfuck.h>

/**
 * Creates a new state.
 */
BrainfuckState * brainfuck_state() {
	BrainfuckState *state = (BrainfuckState *) malloc(sizeof(BrainfuckState));
	state->root = 0;
	state->head = 0;
	return state;
}

/**
 * Creates a new brainfuck context.
 *
 * @param size The size of the tape.
 */
BrainfuckExecutionContext * brainfuck_context(int size) {
	if (size < 0)
		size = BRAINFUCK_TAPE_SIZE;

	unsigned char* tape = calloc(size, sizeof(char));
	
	BrainfuckExecutionContext *context = (BrainfuckExecutionContext *) 
			malloc(sizeof(BrainfuckExecutionContext));
	
	context->output_handler = &putchar;
	context->input_handler = &brainfuck_getchar;
	context->tape = tape;
	context->tape_index = 0;
	context->tape_size = size;
	context->shouldStop = 0;
	return context;
}

/**
 * Removes the given instruction from the linked list.
 * 
 * @param state The state
 * @param instruction The instruction to remove.
 * @return The instruction that is removed.
 */
BrainfuckInstruction * brainfuck_remove(BrainfuckState *state, BrainfuckInstruction *instruction) {
	if (state == NULL || instruction == NULL)
		return NULL;
	if (state->head == instruction)
		state->head = instruction->previous;
	instruction->previous->next = instruction->next;
	instruction->next->previous = instruction->previous;
	instruction->previous = 0;
	instruction->next = 0;
	return instruction;
}

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param instruction The instruction to add.
 * @return The new head of the linked list.
 */
BrainfuckInstruction * brainfuck_add(BrainfuckState *state, 
		BrainfuckInstruction *instruction) {
	if (state == NULL || instruction == NULL)
		return NULL;
	instruction->previous = state->head;
	if (state->head != NULL) 
		state->head->next = instruction;
	BrainfuckInstruction *iter = instruction;
	while (iter != NULL) {
		if (iter->next == NULL) {
			state->head = iter;
			break;
		}
		iter = iter->next;
	}
	if (state->root == NULL) 
		state->root = instruction;
	return state->head;
}

/**
 * Adds an instruction to the front of the instruction list.
 *
 * @param state The state.
 * @param instruction The instruction to add.
 * @return The new head of the linked list.
 */
BrainfuckInstruction * brainfuck_add_front(BrainfuckState *state, 
		BrainfuckInstruction *instruction) {
	if (state == NULL || instruction == NULL)
		return NULL;
	instruction->previous = 0;
	BrainfuckInstruction *iter = instruction;
	while (iter != NULL) {
		if (iter->next == NULL) {
			state->head = iter;
			break;
		}
		iter = iter->next;
	}
	iter->next = state->root;
	state->root->previous = iter;
	state->root = instruction;
	return state->head;
}

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param before The instruction you want to add another instruction before.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_insert_before(BrainfuckState *state, BrainfuckInstruction *before, 
		BrainfuckInstruction *instruction) {
	if (state == NULL || before == NULL || instruction == NULL)
			return NULL;
	BrainfuckInstruction *previous = before->previous;
	BrainfuckInstruction *iter = instruction;
	while (iter != NULL) {
		if (iter->next == NULL)
			break;
		iter = iter->next;
	}
	before->previous = iter;
	iter->next = iter;

	if (previous != NULL) {
		previous->next = instruction;
		instruction->previous = previous;
	}
	if (state->root == before)
		state->root = instruction;
	return instruction;
}

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param after The instruction you want to add another instruction after.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
BrainfuckInstruction * brainfuck_insert_after(BrainfuckState *state, BrainfuckInstruction *after, 
			BrainfuckInstruction *instruction) {
		if (state == NULL || after == NULL || instruction == NULL)
			return NULL;
		BrainfuckInstruction *next = after->next;
		BrainfuckInstruction *iter = instruction;

		after->next = instruction;
		instruction->previous = after;
		while (iter != NULL) {
			if (iter->next == NULL)
				break;
			iter = iter->next;
		}

		if (next != NULL) {
			next->previous = iter;
			iter->next = next;
		}
		if (state->head == after)
			state->head = iter;
		return instruction;
}

/**
 * Reads a character, converts it to an instruction and repeats until the EOF character
 * 	occurs and will then return a linked list containing all instructions.
 *
 * @param head The head of the linked list that contains the instructions. 
 * @param stream The stream to read from.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_stream(FILE *stream) {
	return brainfuck_parse_stream_until(stream, EOF);
}

/**
 * Reads a character, converts it to an instruction and repeats until the given character
 * 	occurs and will then return a linked list containing all instructions.
 *
 * @param stream The stream to read from.
 * @param until If this character is found in the stream, we will quit reading and return.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_stream_until(FILE *stream, const int until) {
	BrainfuckInstruction *instruction = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
	instruction->next = 0;
	instruction->loop = 0;
	BrainfuckInstruction *root = instruction;
	char ch;
	char temp;
	while ((ch = fgetc(stream)) != until) {
		if (ch == EOF || feof(stream)) { break; }
		instruction->type = ch;
		instruction->difference = 1;
		switch(ch) {
		case BRAINFUCK_TOKEN_PLUS:
		case BRAINFUCK_TOKEN_MINUS:
			while ((temp = fgetc(stream)) != until && (temp == BRAINFUCK_TOKEN_PLUS 
					|| temp == BRAINFUCK_TOKEN_MINUS)) {
				if (temp == ch) {
					instruction->difference++;
				} else {
					instruction->difference--;
				}
			}
			ungetc(temp, stream);
			break;
		case BRAINFUCK_TOKEN_NEXT:
		case BRAINFUCK_TOKEN_PREVIOUS:
			while ((temp = fgetc(stream)) != until && (temp == BRAINFUCK_TOKEN_NEXT 
					|| temp == BRAINFUCK_TOKEN_PREVIOUS)) {
				if (temp == ch) {
					instruction->difference++;
				} else {
					instruction->difference--;
				}
			}
			ungetc(temp, stream);
			break;
		case BRAINFUCK_TOKEN_OUTPUT:
		case BRAINFUCK_TOKEN_INPUT:
			while ((temp = fgetc(stream)) != until && temp == ch)
				instruction->difference++;
			ungetc(temp, stream);
			break;
		case BRAINFUCK_TOKEN_LOOP_START:
			instruction->loop = brainfuck_parse_stream_until(stream, until);
			break;
		case BRAINFUCK_TOKEN_LOOP_END:
			return root;
		case BRAINFUCK_TOKEN_BREAK:
			break;
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

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 *
 * @param str The string to read from.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_string(char *str) {
	return brainfuck_parse_substring(str, 0, -1);
}

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
BrainfuckInstruction * brainfuck_parse_substring(char *str, int begin, int end) {
	int tmp = begin;
	return brainfuck_parse_substring_incremental(str, &tmp, end);
}

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 *
 * @param str The string to read from.
 * @param ptr The pointer to the integer holding the index you want to start parsing at.
 *	Since this will be used as counter, the value of the pointer will be increased.
 * @param end The index you want to stop parsing at.
 *	When <code>-1</code> is given, it will stop at the end of the string.
 * @param The head of the linked list containing the instructions.
 */
BrainfuckInstruction * brainfuck_parse_substring_incremental(char *str, int *ptr, int end) {
	if (str == NULL || ptr == NULL)
		return NULL;
	if (end < 0)
		end = strlen(str);
	BrainfuckInstruction *root = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
	BrainfuckInstruction *instruction = root;
	instruction->next = 0;
	instruction->previous = 0;
	instruction->loop = 0;
	char c, temp_c;
	for (; *ptr < end && (c = str[*ptr]); (*ptr)++) {
			instruction->type = c;
			instruction->difference = 1;
			switch(c) {
			case BRAINFUCK_TOKEN_PLUS:
			case BRAINFUCK_TOKEN_MINUS:
				(*ptr)++;
				for (; *ptr < end && (temp_c = str[*ptr]) && 
						(temp_c == BRAINFUCK_TOKEN_PLUS || temp_c == BRAINFUCK_TOKEN_MINUS); (*ptr)++) {
					if (temp_c == c) {
						instruction->difference++;
					} else {
						instruction->difference--;
					}
				}
				(*ptr)--;
				break;
			case BRAINFUCK_TOKEN_NEXT:
			case BRAINFUCK_TOKEN_PREVIOUS:
				(*ptr)++;
				for (; *ptr < end && (temp_c = str[*ptr]) && 
						(temp_c == BRAINFUCK_TOKEN_NEXT || temp_c == BRAINFUCK_TOKEN_PREVIOUS); (*ptr)++) {
					if (temp_c == c) {
						instruction->difference++;
					} else {
						instruction->difference--;
					}
				}
				(*ptr)--;
				break;
			case BRAINFUCK_TOKEN_OUTPUT:
			case BRAINFUCK_TOKEN_INPUT:
				(*ptr)++;
				for (; *ptr < end && (str[*ptr] == c); (*ptr)++) {
					instruction->difference++;
				}
				(*ptr)--;
				break;
			case BRAINFUCK_TOKEN_LOOP_START:
				(*ptr)++;
				instruction->loop = brainfuck_parse_substring_incremental(str, ptr, end);
				break;
			case BRAINFUCK_TOKEN_LOOP_END:
				return root;
			case BRAINFUCK_TOKEN_BREAK:
				break;
			default:
				continue;
			}
			instruction->next = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
			instruction->next->next = 0;
			instruction->next->loop = 0;
			instruction->next->previous = instruction;
			instruction = instruction->next;
		}
		instruction->type = BRAINFUCK_TOKEN_LOOP_END;
		return root;
}

/**
 * Converts the given character to an instruction.
 *
 * @param c The character to convert.
 * @param The character that's converted into an instruction.
 */
BrainfuckInstruction * brainfuck_parse_character(char c) {
	BrainfuckInstruction *instruction = (BrainfuckInstruction *) malloc(sizeof(BrainfuckInstruction));
	instruction->next = 0;
	instruction->loop = 0;
	instruction->difference = 1;
	switch(c) {
	case BRAINFUCK_TOKEN_PLUS:
	case BRAINFUCK_TOKEN_MINUS:
	case BRAINFUCK_TOKEN_NEXT:
	case BRAINFUCK_TOKEN_PREVIOUS:
	case BRAINFUCK_TOKEN_OUTPUT:
	case BRAINFUCK_TOKEN_INPUT:
	case BRAINFUCK_TOKEN_LOOP_START:
	case BRAINFUCK_TOKEN_LOOP_END:
	case BRAINFUCK_TOKEN_BREAK:
		break;
	default:
		return NULL;
	}
	instruction->type = c;
	return instruction;
}

/**
 * Destroys the given instruction.
 * 
 * @param instruction The instruction to destroy.
 */
void brainfuck_destroy_instruction(BrainfuckInstruction *instruction) {
	if (instruction == NULL)
		return;
	free(instruction);
	instruction = 0;
}

/**
 * Destroys a linked list containing instructions.
 * 
 * @param root The start of the instruction list.
 */
void brainfuck_destroy_instructions(BrainfuckInstruction *root) {
	BrainfuckInstruction *tmp;
	while (root != NULL) {
		tmp = root;
		brainfuck_destroy_instructions(root->loop);
		root = root->next;
		brainfuck_destroy_instruction(tmp);
	}
}

/**
 * Destroys a state.
 * 
 * @param state The state to destroy
 */
void brainfuck_destroy_state(BrainfuckState *state) {
	if (state == NULL)
		return;
	brainfuck_destroy_instructions(state->root);
	state->head = 0;
	state->root = 0;
	free(state);
	state = 0;
}

/**
 * Destroys a context.
 * 
 * @param context The context to destroy
 */
void brainfuck_destroy_context(BrainfuckExecutionContext *context) {
	free(context->tape);
	free(context);
	context = 0;
}

/**
 * Executes the given linked list containing instructions.
 * 
 * @param root The start of the linked list of instructions you want
 * 	to execute.
 * @param context The context of this execution that contains the tape and
 *	other execution related variables.
 */
void brainfuck_execute(BrainfuckInstruction *root, BrainfuckExecutionContext *context) {
	if (root == NULL || context == NULL)
		return;
	BrainfuckInstruction *instruction = root;
	int index;
	while (instruction != NULL && instruction->type != BRAINFUCK_TOKEN_LOOP_END) {
		switch (instruction->type) {
		case BRAINFUCK_TOKEN_PLUS:
			context->tape[context->tape_index] += instruction->difference;
			break;
		case BRAINFUCK_TOKEN_MINUS:
			context->tape[context->tape_index] -= instruction->difference;
			break;
		case BRAINFUCK_TOKEN_NEXT:
			if ((unsigned long) instruction->difference >= INT_MAX - context->tape_size || 
					(unsigned long) context->tape_index + instruction->difference >= context->tape_size) {
				fprintf(stderr, "error: tape memory out of bounds (overrun)\nexceeded the tape size of %zd cells\n", context->tape_size);
				exit(EXIT_FAILURE);
			}
			context->tape_index += instruction->difference;
			break;
		case BRAINFUCK_TOKEN_PREVIOUS:
			if ((unsigned long) instruction->difference >= INT_MAX - context->tape_size || 
					context->tape_index - instruction->difference < 0) {
				fprintf(stderr, "error: tape memory out of bounds (underrun)\nundershot the tape size of %zd cells\n", context->tape_size);
				exit(EXIT_FAILURE);
			}
			context->tape_index -= instruction->difference;
			break;
		case BRAINFUCK_TOKEN_OUTPUT:
			for (index = 0; index < instruction->difference; index++)
				context->output_handler(context->tape[context->tape_index]);
			break;
		case BRAINFUCK_TOKEN_INPUT:
			for (index = 0; index < instruction->difference; index++) {
				char input = context->input_handler();
				if (input == EOF) {
					if (BRAINFUCK_EOF_BEHAVIOR != 1)
						context->tape[context->tape_index] = BRAINFUCK_EOF_BEHAVIOR;
				} else {
					context->tape[context->tape_index] = input;
				}
			}
			break;
		case BRAINFUCK_TOKEN_LOOP_START:
			while(context->tape[context->tape_index])
				brainfuck_execute(instruction->loop, context);
			break;
		case BRAINFUCK_TOKEN_BREAK: {
			int low  = context->tape_index - 10;
			if (low < 0) low = 0;
			int high = low + 21;
			if (high >= (int) context->tape_size) 
				high = context->tape_size-1;
			for (index = low; index < high; index++)
				printf("%i\t", index);
			printf("\n");
			for (index = low; index < high; index++)
				printf("%d\t", context->tape[index]);
			printf("\n");
			for (index = low; index < high; index++)
				if (index == context->tape_index)
					printf("^\t");
			else
				printf(" \t");
			printf("\n");
			break;
		}
		default:
			return;
		}
		instruction = instruction->next;

		if (context->shouldStop == 1) {
			instruction = NULL;
			return;
		}
	}
}

/*
 * Stops the currently running program referenced by the given execution context.
 *
 * @param context The context of this execution that contains the tape and
 *	other execution related variables.
 */
void brainfuck_execution_stop(BrainfuckExecutionContext *context) {
	context->shouldStop = 1;
}

/**
 * Reads exactly one char from stdin.
 * @return The character read from stdin.
 */
char brainfuck_getchar() {
	char ch, t;
	ch = getchar();
	while ((t = getchar()) != '\n' && t != EOF) { }	/* Clear stdin */
	return ch;
}
