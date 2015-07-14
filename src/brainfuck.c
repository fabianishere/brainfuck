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
/*
 * A reference implementation of the brainfuck.h header written in C89, 
 *	implementing the brainfuck language.
 */

/* Dependencies */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <brainfuck.h>

/* Definitions */
#define BRAINFUCK_LOOP_DEPTH 8
#define BRAINFUCK_BUFFER_SIZE 1024

/* Implementation */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void)
{
	return malloc(sizeof(struct BrainfuckInstruction));
}

void brainfuck_instruction_dealloc(struct BrainfuckInstruction *instruction)
{
	free(instruction);
}

struct BrainfuckParserContext * brainfuck_parser_context_alloc(void)
{
	return malloc(sizeof(struct BrainfuckParserContext));
}

void brainfuck_parser_context_dealloc(struct BrainfuckParserContext *ctx)
{
	assert(ctx);
	free(ctx->loop);
	free(ctx);
}

int brainfuck_parser_context_init(struct BrainfuckParserContext *ctx)
{
	assert(ctx);
	
	ctx->loop = malloc(sizeof(struct BrainfuckInstruction *) 
		* BRAINFUCK_LOOP_DEPTH);
	if (!ctx->loop)
		return BRAINFUCK_ENOMEM;
	ctx->loop_size = BRAINFUCK_LOOP_DEPTH;
	ctx->loop_index = 0;
	return BRAINFUCK_EOK;
}


int brainfuck_parser_validate(struct BrainfuckParserContext *ctx)
{
	return ctx->loop_index == 0;
}


void brainfuck_parser_parse_string_partial(const char *string, 
	struct BrainfuckParserContext *ctx, int *error)
{
	char ch;
	int error_holder;
	int i;
	unsigned int n;
	struct BrainfuckInstruction *instruction;
	
	assert(ctx);
	
	error = error ? error : &error_holder;
	*error = BRAINFUCK_EOK;
	instruction = ctx->tail;
	
	/* Initialise the context if it hasn't been initialised yet */
	if (!ctx->loop && brainfuck_parser_context_init(ctx) != BRAINFUCK_EOK)
		goto error_nomem;

	while((ch = *string++)) {
		switch(ch) {
		case '+':
		case '-':
			i = 0;
			string--;
			while ((ch = *string++) == '+' || ch == '-')
					i += (ch == '+' ? 1 : -1);
			string--;
			if (i == 0)
				break;
			instruction = brainfuck_instruction_alloc();
			instruction->type = INC;
			instruction->argument.delta = i;
			break;
		case '>':
		case '<':
			i = 0;
			string--;
			while ((ch = *string++) == '>' || ch == '<')
					i += (ch == '>' ? 1 : -1);
			string--;
			if (i == 0)
				break;
			instruction = brainfuck_instruction_alloc();
			instruction->type = MOV;
			instruction->argument.delta = i;
			break;
		case ',':
			n = 0;
			string--;
			while ((ch = *string++) == ',' && n++);
			instruction = brainfuck_instruction_alloc();
			instruction->type = IN;
			instruction->argument.n = n;
			break;
		case '.':
			n = 0;
			string--;
			while ((ch = *string++) == '.' && n++);
			instruction = brainfuck_instruction_alloc();
			instruction->type = OUT;
			instruction->argument.n = n;
			break;
		case '[':
			instruction = brainfuck_instruction_alloc();
			instruction->type = JZ;
			instruction->argument.jump = NULL;
			ctx->loop[ctx->loop_index++] = instruction;
			break;
		case ']':
			if (ctx->loop_index <= 0)
				goto error_syntax;
			
			instruction = brainfuck_instruction_alloc();
			instruction->type = JMP;
			instruction->argument.jump = ctx->loop[--ctx->loop_index];
			instruction->argument.jump->argument.jump = instruction;
			break;
		default:
			continue;
		}
		
		if (!ctx->head)
			ctx->head = instruction;
		else
			ctx->tail->next = instruction;
		ctx->tail = instruction;
	}
	return;
	
	error_nomem:
		*error = BRAINFUCK_ENOMEM;
		return;
	error_syntax:
		*error = BRAINFUCK_ESYNTAX;
		return;
}

struct BrainfuckScript * brainfuck_parser_parse_string(const char *string, 
	int *error)
{
	int error_holder;
	struct BrainfuckParserContext ctx = {NULL, NULL, NULL, 0, 0};
	
	error = error ? error : &error_holder;
	*error = BRAINFUCK_EOK;
	
	brainfuck_parser_parse_string_partial(string, &ctx, error);
	if (*error != BRAINFUCK_EOK)
		return NULL;
	
	if (!brainfuck_parser_validate(&ctx)) {
		*error = BRAINFUCK_ESYNTAX;
		return NULL;
	}
	free(ctx.loop);
	return ctx.head;
}

struct BrainfuckScript * brainfuck_parser_parse_file(FILE *file, int *error)
{
	int error_holder = BRAINFUCK_EOK;
	char buffer[BRAINFUCK_BUFFER_SIZE] = {0};
	struct BrainfuckParserContext ctx = {NULL, NULL, NULL, 0, 0};
	
	error = error ? error : &error_holder;
	*error = BRAINFUCK_EOK;
	
	assert(file);
	while (!feof(file)) {
		memset(&buffer, 0, sizeof(buffer));
		fread(&buffer, sizeof(buffer), 1, file);
		if (ferror(file))
			continue;
		brainfuck_parser_parse_string_partial(buffer, &ctx, error);
		if (*error != BRAINFUCK_EOK)
			return NULL;
	}

	if (!brainfuck_parser_validate(&ctx)) {		
		*error = BRAINFUCK_ESYNTAX;
		return NULL;
	}
	free(ctx.loop);
	return ctx.head;
}

struct BrainfuckEngineContext * brainfuck_engine_context_alloc(void)
{
	return malloc(sizeof(struct BrainfuckEngineContext));
}

void brainfuck_engine_context_dealloc(struct BrainfuckEngineContext *ctx)
{
	free(ctx);
}

int brainfuck_engine_run(const struct BrainfuckScript *script, 
	struct BrainfuckEngineContext *ctx)
{
	assert(script);
	assert(ctx);
	
	unsigned int k;
	
	while (script) {
		switch(script->type) {
		case INC:
			*ctx->memory += script->argument.delta;
			break;
		case MOV:
			ctx->memory += script->argument.delta;
			break;
		case CLR:
			*ctx->memory = 0;
			break;
		case IN:
			for (k = 0; k < script->argument.n; k++)
				*ctx->memory = ctx->read();
			break;
		case OUT:
			for (k = 0; k < script->argument.n; k++)
				ctx->write(*ctx->memory);
			break;
		case JMP:
			script = script->argument.jump;
			continue;
		case JZ:
			if (!*ctx->memory)
				script = script->argument.jump;
			break;
		case NOP:
			break;
		}
		script = script->next;
	}
	return BRAINFUCK_EOK;
}