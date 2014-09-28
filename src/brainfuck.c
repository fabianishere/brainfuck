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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <brainfuck.h>

/*
 * Allocate a new {@link BrainfuckInstruction} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void)
{
	return malloc(sizeof(struct BrainfuckInstruction));
}

/* 
 * Free the given {@link BrainfuckInstruction} from the memory.
 * 
 * @param instruction The instruction to free from the memory.
 */
void brainfuck_instruction_free(struct BrainfuckInstruction *instruction)
{
	free(instruction);
	instruction = 0;
}

/*
 * Create a cell value mutation instruction and returns it.
 *
 * @param delta The difference between the current cell value and the value
 * 	that is going to be set.
 * @return The cell value mutation instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_value_mutation(
	int delta)
{
	struct BrainfuckInstruction *instruction = 
			brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->type = VALUE;
	instruction->attributes.delta = delta;
	instruction->next = NULL;
	return instruction;
}

/*
 * Create an index mutation instruction and returns it.
 *
 * @param delta The difference between the current index and the index
 * 	that is going to be set.
 * @return The index mutation instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_index_mutation(
	int delta)
{
	struct BrainfuckInstruction *instruction = 
			brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->type = INDEX;
	instruction->attributes.delta = delta;
	instruction->next = NULL;
	return instruction;
}

/*
 * Create an output instruction and returns it.
 *
 * @param times The amount of times this instruction will be executed.
 * @return The output instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_output(int times)
{
	struct BrainfuckInstruction *instruction = 
			brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->type = OUTPUT;
	instruction->attributes.delta = times;
	instruction->next = NULL;
	return instruction;
}

/*
 * Create an input instruction and returns it.
 *
 * @param times The amount of times this instruction will be executed.
 * @return The input instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_input(int times)
{
	struct BrainfuckInstruction *instruction = 
			brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->type = INPUT;
	instruction->attributes.delta = times;
	instruction->next = NULL;
	return instruction;
}

/*
 * Create a loop instruction and return it.
 *
 * @return The break instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_loop()
{
	struct BrainfuckInstruction *instruction = 
			brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->type = LOOP;
	instruction->attributes.jump = NULL;
	instruction->next = NULL;
	return instruction;
}

/*
 * Create a break instruction and return it.
 *
 * @param jump The jump of this break instruction.
 * @return The break instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_break(
	struct BrainfuckInstruction *jump)
{
	struct BrainfuckInstruction *instruction = 
			brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->type = BREAK;
	instruction->attributes.jump = jump;
	instruction->next = NULL;
	return instruction;
}

/*
 * Allocate a new {@link BrainfuckContext} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckContext * brainfuck_context_alloc(void)
{
	return malloc(sizeof(struct BrainfuckContext));
}

/* 
 * Free the given {@link BrainfuckContext} from the memory.
 * 
 * @param ctx The context to free from the memory.
 */
void brainfuck_context_free(struct BrainfuckContext *ctx)
{
	if (!ctx)
		return;
	free(ctx->memory);
	free(ctx);
	ctx = 0;
}

/*
 * Return a new {@link BrainfuckContext} initialised with the default
 * 	values.
 * 
 * @return The default {@link BrainfuckContext}.
 */
struct BrainfuckContext * brainfuck_context_default(void)
{
	struct BrainfuckContext *ctx = brainfuck_context_alloc();
	
	if (!ctx)
		return NULL;
	
	ctx->read = &getchar;
	ctx->write = &putchar;
	ctx->mem_size = BRAINFUCK_DMEMSIZE;
	ctx->memory = calloc(ctx->mem_size, sizeof(int));
	
	if (!ctx->memory)
		return NULL;
	
	ctx->index = 0;
	ctx->running = 0;
	return ctx;
}

/*
 * Allocate a new {@link BrainfuckState} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckState * brainfuck_state_alloc(void)
{
	return malloc(sizeof(struct BrainfuckState));
}

/* 
 * Free the given {@link BrainfuckContext} from the memory.
 * 
 * @param state The state to free from the memory.
 */
void brainfuck_state_free(struct BrainfuckState *state)
{
	if (!state)
		return;
	
	free(state->jumps.array);
	free(state);
	state = 0;
}

/*
 * Parse the given string with the given state.
 *
 * @param string The string to read the script from.
 * @param jumps Pointer to an array of pointers to {@link BrainfuckInstruction}s.
 * @param jumps_size Pointer to the size of the jumps array.
 * @param jumps_top Pointer to the current index of the jumps array.
 * @param error A pointer to an integer that will be set to either a success
 *	or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code>
 *	if the parsing failed.
 * @notice Currently only available internally.
 */
struct BrainfuckScript * brainfuck_parse_string_state(
		char *string, struct BrainfuckState *state, int *error)
{
	struct BrainfuckInstruction *script = NULL;
	struct BrainfuckInstruction *top = NULL;
	struct BrainfuckInstruction *instruction = NULL;
	struct BrainfuckInstruction *jump = NULL;
	char character;
	int delta;
	int error_holder = BRAINFUCK_EOK;

	error = error ? error : &error_holder;

	if (!string || !state) {
		*error = BRAINFUCK_EPARAM;
		return NULL;
	}	
	
	while((character = *string++)) {
		delta = 1;
		switch(character) {
		case '-':
			delta = -1;
		case '+':
			while ((character = *string++) == '+' 
					|| character == '-')
				delta += (character == '+' ? 1 : -1);
			string--;
			instruction = brainfuck_instruction_create_value_mutation(delta);
			break;
		case '<':
			delta = -1;
		case '>':
			while ((character = *string++) == '>' 
					|| character == '<')
				delta += (character == '>' ? 1 : -1);
			string--;
			instruction = brainfuck_instruction_create_index_mutation(delta);
			break;
		case '.':
			while ((character = *string++) == '.')
				delta++;
			string--;
			instruction = brainfuck_instruction_create_output(delta);
			break;
		case ',':
			while ((character = *string++) == ',')
				delta++;
			string--;
			instruction = brainfuck_instruction_create_input(delta);
			break;
		case '[':
			if (!state->jumps.array || state->jumps.size <= 0) {
				state->jumps.size = BRAINFUCK_DINITIALDEPTH;
				state->jumps.array = malloc(state->jumps.size * sizeof(struct BrainfuckInstruction *));
			} else if (state->jumps.top + 1 >= state->jumps.size) {
				state->jumps.size *= 2;
				state->jumps.array = realloc(state->jumps.array, state->jumps.size * sizeof(struct BrainfuckInstruction *));
			} else if (4 * state->jumps.top + 4 <= state->jumps.size 
					&& state->jumps.size > BRAINFUCK_DINITIALDEPTH) {
				state->jumps.size /= 2;
				state->jumps.array = realloc(state->jumps.array, state->jumps.size * sizeof(struct BrainfuckInstruction *));
			}
			instruction = brainfuck_instruction_create_loop();
			state->jumps.array[++state->jumps.top] = instruction;
			break;
		case ']':
			if (state->jumps.top < 0) {
				*error = BRAINFUCK_ESYNTAX;
				return NULL;
			}
			jump = state->jumps.array[state->jumps.top--];
			if (!jump) {
				*error = BRAINFUCK_EINTERNAL;
				return NULL;
			}
			instruction = brainfuck_instruction_create_break(jump);
			jump->attributes.jump = instruction;
			break;
		default:
			continue;
		}
		if (!instruction) {
			*error = BRAINFUCK_ENOMEM;
			return NULL;
		}
		if (!top && !script) {
			top = script = instruction;
		} else {
			top->next = instruction;
			top = instruction;
		}
	}	
	return script;
}

/* 
 * Parse the given string.
 *
 * @param string The string to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_string(char *string, int *error)
{
	int error_holder = BRAINFUCK_EOK;
	struct BrainfuckState *state = NULL;
	struct BrainfuckScript *script = NULL;
	
	error = error ? error :  &error_holder;
	state = brainfuck_state_alloc();

	if (!state) {
		*error = BRAINFUCK_ENOMEM;
		return NULL;
	}

	state->jumps.array = NULL;
	state->jumps.size = 0;
	state->jumps.top = -1;

	script = brainfuck_parse_string_state(string, state, error);


	if (*error != BRAINFUCK_EOK) {
		goto error;
	} else if (state->jumps.top > 0) {
		*error = BRAINFUCK_ESYNTAX;
		goto error;
	}
	
	brainfuck_state_free(state);
	return script;

	/* goto this label on error */
	error:
		brainfuck_state_free(state);
		return NULL;
}

/*
 * Parse the given file.
 *
 * @param file The file to read the script from.
 * @param error A pointer to an integer that will be set to either a success
 *	or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code>
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_file(FILE *file, int *error)
{
	int error_holder = BRAINFUCK_EOK;
	char buffer[1024];
	struct BrainfuckScript *script = NULL;
	struct BrainfuckInstruction *top = NULL;
	struct BrainfuckInstruction *instruction = NULL;
	struct BrainfuckState *state = NULL;

	error = error ? error : &error_holder;

	if (!file) {
		*error = BRAINFUCK_EPARAM;
		return NULL;
	}

	state = brainfuck_state_alloc();

	if (!state) {
		*error = BRAINFUCK_ENOMEM;
		return NULL;
	}

	state->jumps.array = NULL;
	state->jumps.size = 0;
	state->jumps.top = -1;

	while (!feof(file)) {
		memset(&buffer, 0, sizeof(buffer));
		fread(&buffer, sizeof(buffer), 1, file);
		if (ferror(file))
			continue;
		instruction = brainfuck_parse_string_state(buffer, state,
 			error);
		if (*error != BRAINFUCK_EOK)
			goto error;
		script = script ? script : instruction;
		if (top)
			top->next = instruction;
		while (instruction->next)
			instruction = instruction->next;
		top = instruction;
	}

	if (state->jumps.top > 0) {
		*error = BRAINFUCK_ESYNTAX;
		goto error;
	}
	
	brainfuck_state_free(state);
	return script;	

	/* goto this label on error */
	error:
		brainfuck_state_free(state);
		return NULL;
}

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
	struct BrainfuckContext *ctx)
{
	if (!script || !ctx)
		return BRAINFUCK_EPARAM;
	ctx->instruction = script;
	ctx->running = 1;
	int index = 0;

	while (ctx->instruction && ctx->running) {
		switch(ctx->instruction->type) {
		case VALUE:
			ctx->memory[ctx->index] += ctx->instruction->attributes.delta;
			break;
		case INDEX:
			ctx->index += ctx->instruction->attributes.delta;
			break;
		case INPUT:
			for (index = 0; index < ctx->instruction->attributes.delta; index++)
				ctx->memory[ctx->index] = ctx->read();
			break;
		case OUTPUT:
			for (index = 0; index < ctx->instruction->attributes.delta; index++)
				ctx->write(ctx->memory[ctx->index]);
			break;
		case LOOP:
			if (ctx->memory[ctx->index] || 
					!ctx->instruction->attributes.jump)
				break;
			ctx->instruction = ctx->instruction->attributes.jump;
			continue;
		case BREAK:
			if (!ctx->memory[ctx->index] || 
					!ctx->instruction->attributes.jump)
				break;
			ctx->instruction = ctx->instruction->attributes.jump;
			continue;
		}
		ctx->instruction = ctx->instruction->next;
	}
	return BRAINFUCK_EOK;
}
