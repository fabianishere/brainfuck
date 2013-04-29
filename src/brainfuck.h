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

#define BRAINFUCK_DEBUG_ENABLED 1
#define BRAINFUCK_DEBUG_DISABLED 0

#define BRAINFUCK_MAX_CELLS 30000

typedef enum BrainfuckTokenType {
   UNKNOWN,
   PLUS,
   MINUS,
   NEXT,
   PREVIOUS,
   INPUT,
   OUTPUT,
   LOOP_START,
   LOOP_END
} BrainfuckTokenType;

/*
 * Represents a Brainfuck token like "+", "-" or ">".
 */
typedef struct BrainfuckToken {
	struct BrainfuckToken *previous;
	struct BrainfuckToken *next;
	char value;
	enum BrainfuckTokenType type;
} BrainfuckToken;


/*
 * Structure that keeps the state of the whole Brainfuck interpeter.
 */
typedef struct BrainfuckState {
	/**
	 * The root token of this state.
	 */
	struct BrainfuckToken *root_token;
	/*
	 * The current token of this state.
	 */
	struct BrainfuckToken *current_token;
	/*
	 * The data pointer.
	 */
	char *pointer;
	/*
	 * The current position of the data pointer used to determine if 
	 * 	the current data pointer is valid.
	 */
	int position;
	/*
	 * When 0 the interpreter does not show debug messages, otherwise
	 *	it does.
	 */
	int debug;
} BrainfuckState;

/*
 * Adds an element to the token array.
 *
 * @param state The Brainfuck state.
 * @param c The character to add.
 */
void brainfuck_put_token(BrainfuckState *, const char);

/*
 * Adds elements to the token array.
 *
 * @param state The Brainfuck state.
 * @param str The characters to add.
 */
void brainfuck_put_token_string(BrainfuckState *, const char *);

/*
 * Returns the last token of the token list.
 *
 * @return The last token of the token list.
 */
BrainfuckToken* brainfuck_get_last_token(BrainfuckState *);

/*
 * Moves to the next cell.
 *
 * @return The next cell.
 */
void brainfuck_next_cell(BrainfuckState *);

/*
 * Moves to the previous element.
 *
 * @return The previous element.
 */
void brainfuck_previous_cell(BrainfuckState *);

/*
 * Increases the value of the current cell.
 */
void brainfuck_increase_cell(BrainfuckState *);

/*
 * Decreases the value of the current cell.
 */
void brainfuck_decrease_cell(BrainfuckState *);

/*
 * Creates a new BrainfuckState.
 *
 * @param size The size of the data pointer.
 * @param debug 0 to disable debug, otherwise debug is enabled.
 */
BrainfuckState * brainfuck_new_state(int, int);

/*
 * Clears the given state.
 * 
 * @param state The BrainfuckState to clear.
 * @param size The size of the new state.
 */
void brainfuck_clear_state(BrainfuckState *, int);

/*
 * End a state and remove it from the memory.
 * 
 * @param state The BrainfuckState to end.
 */
void brainfuck_end_state(BrainfuckState *);

/*
 * Runs the Brainfuck program with the given BrainfuckState.
 *
 * @param state The BrainfuckState to run.
 */
void brainfuck_run(BrainfuckState *);

/*
 * Runs the given token.
 *
 * @param state The BrainfuckState to run.
 * @param token The token use.
 */
void brainfuck_run_token(BrainfuckState *, BrainfuckToken *);
#endif
