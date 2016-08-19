/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Fabian Mastenbroek
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
#define BRAINFUCK_LOOP_DEPTH 16
#define BRAINFUCK_BUFFER_SIZE 1024
#define BRAINFUCK_ARRAY_SIZE 1024

/* Implementation */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void)
{
	return malloc(sizeof(struct BrainfuckInstruction));
}

void brainfuck_instruction_dealloc(struct BrainfuckInstruction *instruction)
{
	free(instruction);
}

struct BrainfuckScript * brainfuck_script_alloc(void)
{
	return malloc(sizeof(struct BrainfuckScript));
}


void brainfuck_script_dealloc(struct BrainfuckScript *script)
{
	assert(script);
	if (script->next)
		brainfuck_script_dealloc(script->next);
	free(script->array);
	free(script);
}


int brainfuck_script_init(struct BrainfuckScript *script, 
	struct BrainfuckScript *previous, size_t size)
{
	assert(script);
	
	script->previous = previous;
	script->size = size;
	script->index = 0;
	script->array = malloc(sizeof(struct BrainfuckInstruction) * size);
	
	return script->array ? BRAINFUCK_EOK : BRAINFUCK_ENOMEM;
}

void brainfuck_script_dump(const struct BrainfuckScript *script, FILE *file)
{
	register int i;
	register struct BrainfuckInstruction instruction;
	
	assert(script);
	assert(file);
	
	while (script) {
		for (i = 0; i < script->index; i++) {
			instruction = script->array[i];
			switch(instruction.type) {
			case INC:
				fprintf(file, "inc %i; ", instruction.argument.offset);
				break;
			case DEC:
				fprintf(file, "dec %i; ", instruction.argument.offset);
				break;
			case MOVL:
				fprintf(file, "movl %i; ", instruction.argument.offset);
				break;
			case MOVR:
				fprintf(file, "movr %i; ", instruction.argument.offset);
				break;
			case CLR:
				fprintf(file, "clr; ");
				break;
			case IN:
				fprintf(file, "in %i; ", instruction.argument.n);
				break;
			case OUT:
				fprintf(file, "out %i; ", instruction.argument.n);
				break;
			case JZ:
				fprintf(file, "jz %i; ", instruction.argument.offset);
				break;
			case JMP:
				fprintf(file, "jmp %i; ", instruction.argument.offset);
				break;
			case NOP:
				fprintf(file, "nop; ");
				break;
			}
		}
		script = script->next;
	}
	printf("\n");
}

struct BrainfuckParserContext * brainfuck_parser_context_alloc(void)
{
	return malloc(sizeof(struct BrainfuckParserContext));
}

void brainfuck_parser_context_dealloc(struct BrainfuckParserContext *ctx)
{
	assert(ctx);
	free(ctx->offsets.array);
	free(ctx);
}

int brainfuck_parser_context_init(struct BrainfuckParserContext *ctx)
{
	assert(ctx);
	
	ctx->offsets.array = malloc(sizeof(struct BrainfuckInstruction *) 
		* BRAINFUCK_LOOP_DEPTH);
	if (!ctx->offsets.array)
		return BRAINFUCK_ENOMEM;
	ctx->offsets.size = BRAINFUCK_LOOP_DEPTH;
	ctx->offsets.index = 0;
	return BRAINFUCK_EOK;
}


int brainfuck_parser_validate(const struct BrainfuckParserContext *ctx)
{
	return ctx->offsets.index == 0;
}


void brainfuck_parser_parse_string_partial(const char *string, 
	struct BrainfuckParserContext *ctx, int *error)
{
	char ch;
	int error_holder;
	int delta;
	int offset = 0;
	unsigned int n;
	int index;
	size_t size;
	struct BrainfuckInstruction *instruction;
	struct BrainfuckInstruction *tmp;
	
	assert(ctx);
	
	error = error ? error : &error_holder;
	*error = BRAINFUCK_EOK;
	
	/* Initialize the context if it hasn't been initialized yet */
	if (!ctx->offsets.array && brainfuck_parser_context_init(ctx) != BRAINFUCK_EOK)
		goto error_nomem;
	
	/* Initialize script if it hasn't been initialized yet */
	if (!ctx->tail) {
		ctx->tail = brainfuck_script_alloc();
		if (!ctx->tail || brainfuck_script_init(ctx->tail, NULL, BRAINFUCK_ARRAY_SIZE) != BRAINFUCK_EOK)
			goto error_nomem;
		ctx->head = ctx->tail;
	}
	index = ctx->tail->index;
	size = ctx->tail->size;
	instruction = &ctx->tail->array[index];
	offset = ctx->offsets.current;
	while((ch = *string++)) {
		switch(ch) {
		case '+':
		case '-':
			delta = 0;
			string--;
			while ((ch = *string++) == '+' || ch == '-')
					delta += (ch == '+' ? 1 : -1);
			string--;
			if (delta == 0) {
				continue;
			} else if (delta > 0) {
				instruction->type = INC;
				instruction->argument.offset = delta;
			} else {
				instruction->type = DEC;
				instruction->argument.offset = -delta;
			}
			break;
		case '>':
		case '<':
			delta = 0;
			string--;
			while ((ch = *string++) == '>' || ch == '<')
					delta += (ch == '>' ? 1 : -1);
			string--;
			if (delta == 0) {
				continue;
			} else if (delta > 0) {
				instruction->type = MOVR;
				instruction->argument.offset = delta;
			} else {
				instruction->type = MOVL;
				instruction->argument.offset = -delta;
			}
			break;
		case ',':
			n = 0;
			string--;
			while ((ch = *string++) == ',' && n++);
			instruction->type = IN;
			instruction->argument.n = n;
			break;
		case '.':
			n = 0;
			string--;
			while ((ch = *string++) == '.' && n++);
			instruction->type = OUT;
			instruction->argument.n = n;
			break;
		case '[':
			if ((size_t) ctx->offsets.index >= ctx->offsets.size)
				goto error_nomem;
			instruction->type = JZ;
			instruction->argument.offset = offset;
			ctx->offsets.array[ctx->offsets.index++] = instruction;
			offset = 0;
			break;
		case ']':
			if (ctx->offsets.index <= 0)
				goto error_syntax;
			instruction->type = JMP;
			instruction->argument.offset = -offset;
			tmp = ctx->offsets.array[--ctx->offsets.index];
			offset += tmp->argument.offset;
			tmp->argument.offset = -instruction->argument.offset + 1;
			break;
		default:
			continue;
		}
		instruction += (index++, offset++, 1);
		if ((size_t) index < size)
			continue;
		ctx->tail->next = brainfuck_script_alloc();
		if (!ctx->tail->next ||
				brainfuck_script_init(ctx->tail->next, ctx->tail, BRAINFUCK_ARRAY_SIZE) != BRAINFUCK_EOK)
			goto error_nomem;
		ctx->tail->index = index;
		ctx->tail = ctx->tail->next;
		index = ctx->tail->index;
		size = ctx->tail->size;
		instruction = &ctx->tail->array[index];
	}
	ctx->tail->index = index;
	ctx->offsets.current = offset;
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
	struct BrainfuckParserContext ctx = {NULL, NULL, {0, 0, 0, NULL}};
	
	error = error ? error : &error_holder;
	*error = BRAINFUCK_EOK;
	
	brainfuck_parser_parse_string_partial(string, &ctx, error);
	if (*error != BRAINFUCK_EOK) 
		goto error;
	
	if (!brainfuck_parser_validate(&ctx)) {
		*error = BRAINFUCK_ESYNTAX;
		goto error;
	}
	
	free(ctx.offsets.array);
	return ctx.head;
	
	error:
		free(ctx.offsets.array);
		if (ctx.head)
			brainfuck_script_dealloc(ctx.head);
		return NULL;
}

struct BrainfuckScript * brainfuck_parser_parse_file(FILE *file, int *error)
{
	int error_holder = BRAINFUCK_EOK;
	char buffer[BRAINFUCK_BUFFER_SIZE] = {0};
	struct BrainfuckParserContext ctx = {NULL, NULL, {0, 0, 0, NULL}};
	
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
			goto error;
	}

	if (!brainfuck_parser_validate(&ctx)) {		
		*error = BRAINFUCK_ESYNTAX;
		goto error;
	}
	free(ctx.offsets.array);
	return ctx.head;
	
	error:
		free(ctx.offsets.array);
		if (ctx.head)
			brainfuck_script_dealloc(ctx.head);
		return NULL;
}

static struct BrainfuckInstruction * next(const struct BrainfuckScript *script, int index) {
	assert(script);
	
	while (index < 0 || index >= script->index) {
		if (index >= script->index) {
			index -= script->index;
			script = script->next;
			if (!script)
				return NULL;
		} else if (index < 0) {
			script = script->previous;
			if (!script)
				return NULL;
			index += script->index;
		}
	}
	return script->array + index;
}

int brainfuck_pass_clear(const struct BrainfuckScript *script)
{
	struct BrainfuckInstruction *instruction;
	struct BrainfuckInstruction *a;
	struct BrainfuckInstruction *b;
	int i = 0;
	while((instruction = next(script, i++))) {
		if (instruction->type != JZ)
			continue;
		a = next(script, i++);
		if (!a || !(a->type == INC ||a->type == DEC))
			continue;
		b = next(script, i++);
		if (!b || !(b->type == JMP))
			continue;
		instruction->type = CLR;
		a->type = NOP;
		b->type = NOP;
	}
	return BRAINFUCK_EOK;
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
	unsigned int i;
	register int j;
	register unsigned int k;
	int size; 
	struct BrainfuckInstruction *instructions;
	register unsigned char *memory = ctx->memory;
	register int index = 0;
	struct BrainfuckInstruction instruction;
	union BrainfuckArgument argument;
	
	
	assert(script);
	assert(ctx);
	
	while (script) {
		size = script->index;
		instructions = script->array;
		for (i = 0; i < (unsigned int) size; i++) {
			instruction = instructions[i];
			argument = instruction.argument;
			switch(instruction.type) {
			case INC:
				/*printf("inc %p %i => 0x%08x\n", memory, instruction.argument.offset, *memory + instruction.argument.offset);*/
				memory[index] += argument.offset;
				break;
			case DEC:
				/*printf("dec %p %i => 0x%08x\n", memory, instruction.argument.offset, *memory + instruction.argument.offset);*/
				memory[index] -= argument.offset;
				break;
			case MOVL:
				/*printf("movl %p %i => %p\n", memory, instruction.argument.offset, memory + instruction.argument.offset);*/
				index -= argument.offset;
				break;
			case MOVR:
				/*printf("mov %p %i => %p\n", memory, instruction.argument.offset, memory + instruction.argument.offset);*/
				index += argument.offset;
				break;
			case CLR:
				/*printf("clr\n");*/
				memory[index] = 0;
				break;
			case IN:
				for (k = 0; k < argument.n; k++)
					memory[index] = ctx->read();
				/*printf("in %p => 0x%08x\n", memory, *memory);*/
				break;
			case OUT:
				/*printf("out %p => 0x%08x\n", memory, *memory);*/
				for (k = 0; k < argument.n; k++)
					ctx->write(memory[index]);
				break;
			case JZ:
				/*printf("jz %i\n", instruction.argument.offset);*/
				if (memory[index])
					break;
				/*Fallthrough */
			case JMP:
				/*printf("jmp %i\n", instruction.argument.offset);*/
				j = i + argument.offset - 1;
				while (j < 0 || j >= size) {
					if (j < 0) {
						script = script->previous;
						size = script->index;
						j += size;
					} else {
						j -= size;
						script = script->next;
						size = script->index;
					}
					instructions = script->array;
				}
				i = j;
				break;
			case NOP:
				break;
			}
		}
		script = script->next;
	}
	return BRAINFUCK_EOK;
}
