/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Fabian M.
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
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <brainfuck.h>

/**
 * A {@link BrainfuckInstruction} represents an instruction that can be 
 * 	executed and is part of a linked list of instructions created by a 
 *	parser.
 */
struct BrainfuckInstruction {
	
	/**
	 * The type of this instruction.
	 */
	enum BrainfuckType type;

	/**
	 * The attributes of this instruction.
	 */
	union attributes {
		
		/**
		 * The change in value.
		 */
		int delta;
		
		/**
		 * The amount of times the instruction should be executed.
		 */
		unsigned int k;
		
		/**
		 * Attributes for a PROCEDURE or LOOP instruction.
		 */
		struct procedure {
			
			/**
			 * The head of a linked list containing the instructions of this 
			 *	procedure or loop.
			 */
			struct BrainfuckInstruction *head;
			
			/**
			 * The tail of a linked list containing the instructions of this 
			 *	procedure or loop.
			 */
			struct BrainfuckInstruction *tail;
			
		} procedure;

	} attributes;
	
	/**
	 * The next instruction in the linked list of instructions.
	 */
	struct BrainfuckInstruction *next;
	
} BrainfuckInstruction;

/**
 * Allocate a new {@link BrainfuckInstruction} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void)
{
	return malloc(sizeof(struct BrainfuckInstruction));
}

/**
 * Free the given {@link BrainfuckInstruction} from the memory.
 * 
 * @param instruction The instruction to free from the memory.
 */
void brainfuck_instruction_free(struct BrainfuckInstruction *instruction)
{
	free(instruction);
}

/**
 * Initialise the given instruction as MUTATE instruction.
 *
 * @param instruction The instruction to initialise.
 * @param delta The change in cell value.
 */
void brainfuck_instruction_mutate(
	struct BrainfuckInstruction *instruction, const int delta)
{
	assert(instruction);
	instruction->type = MUTATE;
	instruction->attributes.delta = delta;
	instruction->next = NULL;
}

/**
 * Initialise the given instruction as CLEAR instruction.
 *
 * @param instruction The instruction to initialise.
 */
void brainfuck_instruction_clear(struct BrainfuckInstruction *instruction)
{
	assert(instruction);
	instruction->type = CLEAR;
	instruction->next = NULL;
}

/**
 * Initialise the given instruction as MOVE instruction.
 *
 * @param instruction The instruction to initialise.
 * @param delta The change in index.
 */
void brainfuck_instruction_move(
	struct BrainfuckInstruction *instruction, const int delta)
{
	assert(instruction);
	instruction->type = MOVE;
	instruction->attributes.delta = delta;
	instruction->next = NULL;
}

/**
 * Initialise the given instruction as READ instruction.
 *
 * @param instruction The instruction to initialise.
 * @param k The amount of times this instruction will be executed.
 * @return The READ instruction that has been created or <code>NULL</code>
 *	if the creation failed.
 */
void brainfuck_instruction_read(
	struct BrainfuckInstruction *instruction, const unsigned int k)
{
	assert(instruction);
	instruction->type = READ;
	instruction->attributes.k = k;
	instruction->next = NULL;
}

/**
 * Initialise the given instruction as WRITE instruction.
 *
 * @param instruction The instruction to initialise.
 * @param k The amount of times this instruction will be executed.
 * @return The READ instruction that has been created or <code>NULL</code>
 *	if the creation failed.
 */
void brainfuck_instruction_write(
	struct BrainfuckInstruction *instruction, const unsigned int k)
{
	assert(instruction);
	instruction->type = WRITE;
	instruction->attributes.k = k;
	instruction->next = NULL;
}


/**
 * Initialise the given instruction as PROCEDURE instruction.
 *
 * @param instruction The instruction to initialise.
 */
void brainfuck_instruction_procedure(struct BrainfuckInstruction *instruction)
{
	assert(instruction);
	instruction->type = PROC;
	instruction->attributes.procedure.head = NULL;
	instruction->attributes.procedure.tail = NULL;
	instruction->next = NULL;
}

/**
 * Add the given instruction to the given procedure.
 * 
 * @param procedure The procedure to add the instruction to.
 * @param instruction The instruction to add to the procedure.
 */
void brainfuck_instruction_procedure_add(struct BrainfuckInstruction *procedure,
	struct BrainfuckInstruction *instruction)
{
	assert(procedure && instruction);
	if (!procedure->attributes.procedure.head)
		procedure->attributes.procedure.head = instruction;
	if (procedure->attributes.procedure.tail)
		procedure->attributes.procedure.tail->next = instruction;
	procedure->attributes.procedure.tail = instruction;
}

/**
 * Free a procedure (or procedure-based) instruction from the heap.
 * 
 * @param instruction The instruction to free from the heap.
 */
void brainfuck_instruction_procedure_free(
	struct BrainfuckInstruction *instruction)
{
	assert(instruction);
	brainfuck_instruction_procedure_free_tail(instruction);
	brainfuck_instruction_free(instruction);
}

/**
 * Free the tail of the given {@link BrainfuckInstruction}.
 * 
 * @param instruction The instruction to free the tail from the heap.
 */
void brainfuck_instruction_procedure_free_tail(
	struct BrainfuckInstruction *instruction)
{
	assert(instruction);
	struct BrainfuckInstruction *ptr = instruction->attributes.procedure.head;
	struct BrainfuckInstruction *tmp = NULL;
	while (ptr) {
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
}

/**
 * Initialise the given instruction as LOOP instruction.
 *
 * @param instruction The instruction to initialise.
 */
void brainfuck_instruction_loop(struct BrainfuckInstruction *instruction)
{
	assert(instruction);
	instruction->type = LOOP;
	instruction->attributes.procedure.head = NULL;
	instruction->attributes.procedure.tail = NULL;
	instruction->next = NULL;
}

/**
 * The {@link BrainfuckParserState} struct holds the state of the parser.
 */
struct BrainfuckParserState {

	/**
	 * The {@link BrainfuckScript} of the program currently being parsed.
	 */
	struct BrainfuckScript *script;

	/**
	 * The scope the parser currently is in.
	 */
	struct scope {
		
		/**
		 * The depth of the scope.
		 */
		unsigned int depth;
		
		/**
		 * The maximum depth of the scope. If the depth exceeds this value,
		 *	then the size of the stack containing the scope will be expanded.
		 */
		unsigned int max_depth;
		
		/**
		 * The array with all the scopes.
		 */
		struct BrainfuckInstruction **scopes;
		
	} scope;

} BrainfuckParserState;

/**
 * Allocate a new {@link BrainfuckParserState} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckParserState * brainfuck_parser_state_alloc(void)
{
	return malloc(sizeof(struct BrainfuckParserState));
}

/**
 * Initialise a {@link BrainfuckParserState} structure.
 *
 * @param state The {@link BrainfuckParserState} structure to initialise.
 * @return <code>BRAINFUCK_EOK</code> on success, a value lower than zero
 *	on failure.
 */
int brainfuck_parser_state_init(struct BrainfuckParserState *state)
{
	state->script = brainfuck_instruction_alloc();
	if (!state->script)
		return BRAINFUCK_ENOMEM;
	brainfuck_instruction_procedure(state->script);
	
	state->scope.depth = 0;
	state->scope.max_depth = BRAINFUCK_DINITIALDEPTH;
	state->scope.scopes = malloc(state->scope.max_depth * 
		sizeof(struct BrainfuckInstruction *));
	if (!state->scope.scopes) {
		brainfuck_script_free(state->script);
		return BRAINFUCK_ENOMEM;
	}
	state->scope.scopes[0] = state->script;
	return BRAINFUCK_EOK;
}

/**
 * Free the given {@link BrainfuckParserState} from the heap.
 * 
 * @param state The state to free from the heapy.
 */
void brainfuck_parser_state_free(struct BrainfuckParserState *state)
{
	assert(state);
	free(state->scope.scopes);
	state->scope.scopes = NULL;
	state->scope.max_depth = 0;
	state->scope.depth = 0;
	free(state);
}

/**
 * Parse the given string with the given state.
 * Use this method to parse multiple parts of the same script.
 *
 * @param string The string to read the script from.
 * @param state The state of the parser.
 * @param error A pointer to an integer that will be set to either a success
 *	or an error code.
 */
void brainfuck_parse_string_state(
		const char *string, struct BrainfuckParserState *state, int *error)
{
	int error_holder = BRAINFUCK_EOK;
	int delta = 0;
	unsigned int k = 0;
	char character; 
	struct BrainfuckInstruction *instruction = NULL;
	struct BrainfuckInstruction **tmp = NULL;
	
	assert(state);
	error = error ?: &error_holder;
	
	/* Initialise the state if it hasn't been initialised yet */
	if (!state->script)
		brainfuck_parser_state_init(state);
	
	/* TODO: find alternative way to do this */
	goto alloc;
	
	while((character = *string++)) {
		delta = k = 1;
		switch(character) {
		case '-':
			delta = -1;
		case '+':
			while ((character = *string++) == '+' || character == '-')
				delta += (character == '+' ? 1 : -1);
			string--;
			brainfuck_instruction_mutate(instruction, delta);
			break;
		case '<':
			delta = -1;
		case '>':
			while ((character = *string++) == '>' || character == '<')
				delta += (character == '>' ? 1 : -1);
			brainfuck_instruction_move(instruction, delta);
			string--;
			break;
		case ',':
			while ((character = *string++) == ',' && k++);
			string--;
			brainfuck_instruction_read(instruction, k);
			break;
		case '.':
			while ((character = *string++) == '.' && k++);
			string--;
			brainfuck_instruction_write(instruction, k);
			break;
		case '[':
			/* Detect CLEAR instructions */
			while ((character = *string++) == '+' || character == '-')
				k++;
			if (character == ']') {
				brainfuck_instruction_clear(instruction);
				break;
			}
			/* Restore index */
			string -= k;

			/* Handle as regular loop otherwise */
			if (state->scope.depth + 2 > state->scope.max_depth) {
				/* Prevent allocating too much memory */
				if (state->scope.max_depth > BRAINFUCK_DMAXDEPTH)
					goto error_nomem;
				tmp = realloc(state->scope.scopes, 
					2 * state->scope.max_depth *
					sizeof(struct BrainfuckInstruction *));
				/* Check if the reallocation is successful */
				if  (tmp) {
					state->scope.scopes = tmp;
					state->scope.max_depth *= 2;
					tmp = NULL;
				}
			}

			brainfuck_instruction_loop(instruction);
			brainfuck_instruction_procedure_add(
				state->scope.scopes[state->scope.depth], instruction);
			state->scope.scopes[++state->scope.depth] = instruction;

			/* TODO: find alternative way to do this */
			goto alloc;
		case ']':
			if (state->scope.depth <= 0)
				goto error_syntax;
			state->scope.depth--;
		default:
			continue;
		}
		brainfuck_instruction_procedure_add(
			state->scope.scopes[state->scope.depth], instruction);
		
		/* alloc: allocate a new instruction */
		alloc:
			instruction = brainfuck_instruction_alloc();
			if (!instruction)
				goto error_nomem;
	}
	return;
	
	error_nomem:
		*error = BRAINFUCK_ENOMEM;
		brainfuck_script_free(state->script);
		return;
	error_syntax:
		*error = BRAINFUCK_ESYNTAX;
		brainfuck_script_free(state->script);
		return;
}

/**
 * Parse the given string.
 *
 * @param string The string to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_string(const char *string, int *error)
{
	int error_holder = BRAINFUCK_EOK;
	struct BrainfuckParserState state = {NULL, {0, 0, NULL}};
	
	error = error ? error : &error_holder;
	brainfuck_parse_string_state(string, &state, error);

	if (*error != BRAINFUCK_EOK)
		return NULL;
	if (state.scope.depth != 0) {
		*error = BRAINFUCK_ESYNTAX;
		return NULL;
	}
	free(state.scope.scopes);
	state.scope.scopes = NULL;
	return state.script;
}

/**
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
	char buffer[BRAINFUCK_DFILEBUFSIZE] = {0};
	struct BrainfuckParserState state = {NULL, {0, 0, NULL}};
	
	error = error ? error : &error_holder;
	assert(file);

	while (!feof(file)) {
		memset(&buffer, 0, sizeof(buffer));
		fread(&buffer, sizeof(buffer), 1, file);
		if (ferror(file))
			continue;
		brainfuck_parse_string_state(buffer, &state, error);
		if (*error != BRAINFUCK_EOK)
			return NULL;
	}
	if (state.scope.depth != 0) {
		*error = BRAINFUCK_ESYNTAX;
		return NULL;
	}
	free(state.scope.scopes);
	state.scope.scopes = NULL;
	return state.script;
}

/**
 * Allocate a new {@link BrainfuckExecutionContext} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckExecutionContext * brainfuck_execution_context_alloc(void)
{
	return malloc(sizeof(struct BrainfuckExecutionContext));
}

/**
 * Free the given {@link BrainfuckExecutionContext} from the heap.
 * 
 * @param ctx The context to free from the heap.
 */
void brainfuck_execution_context_free(struct BrainfuckExecutionContext *ctx)
{
	free(ctx);
}

/**
 * Initialise the given {@link BrainfuckExecutionContext}.
 * 
 * @param ctx The execution context to initialise.
 * @param read The read function.
 * @param write The write function.
 * @param mem_size The size of the memory block.
 * @param memory The memory block the program can work with.
 * @param index The initial index.
 */
void brainfuck_execution_context_init(
	struct BrainfuckExecutionContext *ctx, int (*read)(void), 
	int (*write)(const int character), const size_t mem_size, 
	unsigned int *memory, const unsigned long index)
{
	assert(ctx);
	ctx->read = read;
	ctx->write = write;
	ctx->mem_size = mem_size;
	ctx->memory = memory;
	ctx->index = index;
}

/**
 * Interpret the instructions of the given {@link BrainfuckScript} with the
 *	given {@link BrainfuckExecutionContext}.
 *
 * @param script The script to interpret.
 * @param ctx The execution context that will provide environment.
 * @return an integer with a value of zero or higher if the script executed 
 *	successfully, a value lower than zero otherwise.
 */
int brainfuck_execution_interpret(
	const struct BrainfuckScript *script, struct BrainfuckExecutionContext *ctx)
{
	assert(script && ctx);
	unsigned int index = 0;
	struct BrainfuckInstruction *instruction = script->attributes.procedure
		.head;
	while (instruction) {
		switch(instruction->type) {
		case MUTATE:
			ctx->memory[ctx->index] += instruction->attributes.delta;
			break;
		case CLEAR:
			ctx->memory[ctx->index] = 0;
			break;
		case MOVE:
			ctx->index += instruction->attributes.delta;
			break;
		case READ:
			for (index = 0; index < instruction->attributes.k; index++)
				ctx->memory[ctx->index] = ctx->read();
			break;
		case WRITE:
			for (index = 0; index < instruction->attributes.k; index++)
				ctx->write(ctx->memory[ctx->index]);
			break;
		case PROC:
			brainfuck_execution_interpret(instruction, ctx);
			break;
		case LOOP:
			while(ctx->memory[ctx->index])
				brainfuck_execution_interpret(instruction, ctx);
			break;
		}
		instruction = instruction->next;
	}
	return BRAINFUCK_EOK;
}