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

#include "brainfuck.h"

/*
 * Adds an element to the token list.
 *
 * @param state The Brainfuck state.
 * @param c The character to add.
 */
void brainfuck_put_token(BrainfuckState *state, const char c) {
	BrainfuckToken *token = (BrainfuckToken*) malloc(sizeof(BrainfuckToken));
	token->value = c;
	switch(c) {
		case '+': {
			token->type = PLUS;
			break;
		}
		case '-': {
			token->type = MINUS;
			break;	
		}
		case '>': {
			token->type = NEXT;
			break;		
		} 
		case '<': {
			token->type = PREVIOUS;
			break;
		} 
		case '.': {
			token->type = OUTPUT;
			break;
		} 
		case ',': {
			token->type = INPUT;
			break;
		} 
		case '[': {
			token->type = LOOP_START;
			break;
		} 
		case ']': {
			token->type = LOOP_END;
			break;
		}
		default: {
			token->type = UNKNOWN;
			break;
		}
	}
	if (state->root_token) {
		BrainfuckToken *last = brainfuck_get_last_token(state);
		last->next = token;
		token->previous = last;
	} else {
		state->root_token = token;
		state->current_token = token;
	}
}

/*
 * Adds elements to the token list.
 *
 * @param state The Brainfuck state.
 * @param str The characters to add.
 */
void brainfuck_put_token_string(BrainfuckState *state, const char* str) {
	while(*str) {
		brainfuck_put_token(state, *str);
		str++;
	}
}

/*
 * Returns the last token of the token list.
 *
 * @return The last token of the token list.
 */
BrainfuckToken* brainfuck_get_last_token(BrainfuckState *state) {
	BrainfuckToken *token = state->current_token;
	while(token->next) {
		token = token->next;
	}
	return token;
}

/*
 * Moves to the next cell and initializes a new one if there isn't a next
 * 	cell.
 *
 * @return The next cell.
 */
void brainfuck_next_cell(BrainfuckState *state) {
	++state->position;
	++state->pointer;
	if (state->debug != 0)
		printf("pos is now %i\n", state->position);
}

/*
 * Moves to the previous cell.
 *
 * @return The previous cell.
 */
void brainfuck_previous_cell(BrainfuckState *state) {
	if (state->position <= 0) {
		if (state->debug != 0)
			printf("data pointer out of range\n");
		return;
	}
	--state->position;
	--state->pointer;
	if (state->debug != 0)
		printf("pos is now %i\n", state->position);
}

/*
 * Increases the value of the current cell.
 */
void brainfuck_increase_cell(BrainfuckState *state) {
	++*state->pointer;
	if (state->debug != 0)
		printf("value of %i is %c (%i)\n", state->position, 
			*state->pointer, *state->pointer);
}

/*
 * Decreases the value of the current cell.
 */
void brainfuck_decrease_cell(BrainfuckState *state) {
	--*state->pointer;
	if (state->debug != 0)
		printf("value of %i is %c (%i)\n", state->position, 
			*state->pointer, *state->pointer);
}

/*
 * Creates a new BrainfuckState.
 *
 * @param size The size of the data pointer.
 * @param debug 0 to disable debug, otherwise debug is enabled.
 */
BrainfuckState* brainfuck_new_state(int size, int debug) {
	BrainfuckState *state = (BrainfuckState *) malloc(sizeof(BrainfuckState));
	state->pointer = malloc(size * sizeof(char));
	state->position = 0;
	state->debug = debug;
	return state;
}

/*
 * Clears the given state.
 * 
 * @param state The BrainfuckState to clear.
 * @param size The size of the data pointer.
 */
void brainfuck_clear_state(BrainfuckState *state, int size) {
	BrainfuckToken *token = state->root_token;
	BrainfuckToken *tmp;
	while(token) {
		tmp = token;
		token = token->next;
		free(tmp);
		tmp = 0;
	}
	state->pointer = malloc(size * sizeof(char));
	state->position = 0;
	state->debug = 0;
}

/*
 * End a state and remove it from the memory.
 * 
 * @param state The BrainfuckState to end.
 */
void brainfuck_end_state(BrainfuckState *state) {
	brainfuck_clear_state(state, 0);
	free(state->pointer);
	state->pointer = 0;
	free(state);
	state = 0;
}

/*
 * Runs the Brainfuck program with the given BrainfuckState.
 *
 * @param state The BrainfuckState to run.
 */
void brainfuck_run(BrainfuckState *state) {
	while(state->current_token) {
		brainfuck_run_token(state, state->current_token);
		state->current_token = state->current_token->next;
	}
}

/*
 * Runs the given token.
 *
 * @param state The BrainfuckState to run.
 * @param token The token use.
 */
void brainfuck_run_token(BrainfuckState *state, BrainfuckToken *token) {
	switch(token->type) {
		case PLUS:
			brainfuck_increase_cell(state);
		break;
		case MINUS:
			brainfuck_decrease_cell(state);
		break;
		case NEXT:
			brainfuck_next_cell(state);
		break;
		case PREVIOUS:
			brainfuck_previous_cell(state);
		break;
		case OUTPUT:
			putchar(*state->pointer);
			fflush(stdout);
		break;
		case INPUT:
			*state->pointer = getchar();
		break;
		case LOOP_START:
			if (*(state->pointer) == 0) {
				int depth = 1;
				while (depth > 0) {
					state->current_token = state->current_token->next;
					if (state->current_token) {
						if (state->current_token->type == LOOP_START) {
							depth++;
						} else if (state->current_token->type == LOOP_END) {
							depth--;
						}
					}
				}
			}
		break;
		case LOOP_END:
			if (*(state->pointer) != 0) {
				int depth = 1;
				while (depth > 0) {
					state->current_token = state->current_token->previous;
					if (state->current_token) {
						if (state->current_token->type == LOOP_START) {
							depth--;
						} else if (state->current_token->type == LOOP_END) {
							depth++;
						}
					}
				}
			}
		break;
		case UNKNOWN:
		break;
	}
}
