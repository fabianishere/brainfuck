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
 * A brainfuck implementation written in C.
 */
/* Dependencies */
#include <stdlib.h>
#include <string.h>

#include <brainfuck/brainfuck.h>
#include <brainfuck/parser.h>
#include <brainfuck/engine.h>

/* Definitions */
#define BRAINFUCK_ALLOC malloc
#define BRAINFUCK_REALLOC realloc
#define BRAINFUCK_DEALLOC free
#define BRAINFUCK_SCOPE_DEPTH 16
#define BRAINFUCK_BUFFER_SIZE 1024

/* brainfuck/brainfuck.h */
const char brainfuck_misc_version[] = BRAINFUCK_VERSION;

int brainfuck_misc_version_number(void) {
	return BRAINFUCK_VERSION_NUMBER;
}

/* brainfuck/parser.h */
struct BrainfuckInstruction * brainfuck_instruction_alloc(void)
{
	return BRAINFUCK_ALLOC(sizeof(struct BrainfuckInstruction));
}

void brainfuck_instruction_dealloc(struct BrainfuckInstruction *instruction)
{
	BRAINFUCK_DEALLOC(instruction);
}

struct BrainfuckScript * brainfuck_script_alloc(void)
{
	return BRAINFUCK_ALLOC(sizeof(struct BrainfuckScript));
}


void brainfuck_script_dealloc(struct BrainfuckScript *script)
{
	struct BrainfuckInstruction *inst = script->head;
	struct BrainfuckInstruction *tmp;
	while (inst) {
		tmp = inst;
		inst = inst->next;
		BRAINFUCK_DEALLOC(tmp);
	}
	BRAINFUCK_DEALLOC(script);
}

struct BrainfuckParserContext {
	/**
	 * The {@link BrainfuckScript} that is being parsed.
	 */
	struct BrainfuckScript *script;
	
	/**
	 * A stack of instructions which represents the scopes.
	 */
	struct {
		/*
		 * The size of the stack.
		 */
		size_t size;

		/*
		 * The current index in the stack.
		 */
		int index;

		/*
		 * The array of instructions.
		 */
		struct BrainfuckInstruction **array;
	} scopes;
	
	/**
	 * The current memory offset.
	 */
	int32_t offset;
};

struct BrainfuckParserContext * brainfuck_parser_context_alloc(void)
{
	return BRAINFUCK_ALLOC(sizeof(struct BrainfuckParserContext));
}

void brainfuck_parser_context_dealloc(struct BrainfuckParserContext *ctx)
{
	BRAINFUCK_DEALLOC(ctx->scopes.array);
	BRAINFUCK_DEALLOC(ctx);
}

static int parser_context_init(struct BrainfuckParserContext *ctx)
{
	ctx->script = brainfuck_script_alloc();
	ctx->script->head = NULL;
	ctx->script->tail = NULL;
	
	if (!ctx->script)
		return BRAINFUCK_ENOMEM;
	
	ctx->scopes.array = BRAINFUCK_ALLOC(sizeof(struct BrainfuckInstruction *) 
		* BRAINFUCK_SCOPE_DEPTH);
	if (!ctx->scopes.array)
		return BRAINFUCK_ENOMEM;
	ctx->scopes.size = BRAINFUCK_SCOPE_DEPTH;
	ctx->scopes.index = 0;
	ctx->offset = 0;
	return BRAINFUCK_EOK;
}

int brainfuck_parser_validate(const struct BrainfuckParserContext *ctx)
{
	return ctx->scopes.index == 0;
}

void brainfuck_parser_parse_string_segment(const char *string, 
	struct BrainfuckParserContext *ctx, int *error)
{
	char c;
	int reserved;
	int32_t delta = 0;
	int32_t offset = 0;
	struct BrainfuckInstruction *inst;
	struct BrainfuckInstruction *prev;
	struct BrainfuckInstruction *tmp;
	
	error = error ? error : &reserved;
	*error = BRAINFUCK_EOK;
	
	/* Initialize context if it hasn't been initialized yet */
	if (!ctx->script && (*error = parser_context_init(ctx)) != BRAINFUCK_EOK)
		return;
	
	/* Allocate first instruction */
	inst = brainfuck_instruction_alloc();
	inst->opcode = NOP;
	
	/* Append instruction to previous instruction */
	if (!ctx->script->head) {
		ctx->script->head = inst;
		ctx->script->tail = inst;
	} else {
		ctx->script->tail->next = inst;
	}
	
	/* Set current offset */
	offset = ctx->offset;
	
	while((c = *string++)) {
		switch(c) {
		/* ADD and SUB instructions */
		case '+':
		case '-':
			/* Consume until token of different type */
			delta = 0;
			string--;
			while ((c = *string++) == '+' || c == '-')
					delta += (c == '+' ? 1 : -1);
			string--;
			
			if (delta == 0) {
				continue;
			} else if (delta > 0) {
				inst->opcode = ADD;
				inst->x.offset = delta;
				inst->y.offset = offset;
			} else {
				inst->opcode = SUB;
				inst->x.offset = -delta;
				inst->y.offset = offset;
			}
			break;
			
		/* MOVL and MOVR instructions */
		case '>':
		case '<':
			/* Consume until token of different type */
			delta = 0;
			string--;
			while ((c = *string++) == '>' || c == '<')
					delta += (c == '>' ? 1 : -1);
			string--;
			
			/* Increase/decrease current memory offset accordingly */
			offset += delta;
			continue; /* Don't allocate new instruction */
			
		/* I/O instructions */
		case ',':
			inst->opcode = IN;
			inst->x.offset = offset;
			break;
		case '.':
			inst->opcode = OUT;
			inst->x.offset = offset;
			break;
			
		/* Flow control instructions */
		case '[':
			/* Have we reached the scope upper limit */
			if ((size_t) ctx->scopes.index >= ctx->scopes.size)
				goto error_nomem;

			/* Reset offset before jump */
			if (offset > 0) {
				inst->opcode = MOVR;
				inst->x.offset = offset;
				inst->next = brainfuck_instruction_alloc();
				inst = inst->next;
			} else if (offset < 0) {
				inst->opcode = MOVL;
				inst->x.offset = -offset;
				inst->next = brainfuck_instruction_alloc();
				inst = inst->next;
			}

			if (!inst)
				goto error_nomem;

			offset = 0;

			inst->opcode = JMP;
			inst->y.flags |= 0xF; /* Forward jump */
			
			/* Put jump on stack */
			ctx->scopes.array[ctx->scopes.index++] = inst;
			break;
		case ']':
			/* Have we reached the scope lower limit */
			if (ctx->scopes.index <= 0)
				goto error_syntax;
			
			/* Reset offset before jump */
			if (offset > 0) {
				inst->opcode = MOVR;
				inst->x.offset = offset;
				inst->next = brainfuck_instruction_alloc();
				inst = inst->next;
			} else if (offset < 0) {
				inst->opcode = MOVL;
				inst->x.offset = -offset;
				inst->next = brainfuck_instruction_alloc();
				inst = inst->next;
			}

			if (!inst)
				goto error_nomem;

			offset = 0;
			
			inst->opcode = JNZ;
			inst->y.flags |= ~0xF; /* Backward jump */
			
			/* Get and set the jump targets */
			tmp = ctx->scopes.array[--ctx->scopes.index];
			inst->x.target = tmp->next;
			tmp->x.target = inst;
			break;
		default:
			continue;
		}
		
		/* Allocate new instruction */
		inst->next = brainfuck_instruction_alloc();
		prev = inst;
		inst = inst->next;
		inst->next = NULL;
		inst->opcode = NOP;
	}
	
	/* Set offset */
	ctx->offset = offset;
	
	/* Remove last NOP instruction if possible */
	if (inst->opcode == NOP && prev) {
		ctx->script->tail = prev;
		prev->next = NULL;
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
	struct BrainfuckParserContext ctx = {NULL, {0, 0, NULL}, 0};
	int reserved;
	
	error = error ? error : &reserved;
	*error = BRAINFUCK_EOK;
	
	brainfuck_parser_parse_string_segment(string, &ctx, error);
	
	if (*error != BRAINFUCK_EOK) 
		goto error;
	if (!brainfuck_parser_validate(&ctx)) {
		*error = BRAINFUCK_ESYNTAX;
		goto error;
	}
	
	BRAINFUCK_DEALLOC(ctx.scopes.array);
	return ctx.script;
	
	error:
		BRAINFUCK_DEALLOC(ctx.scopes.array);
		if (ctx.script)
			brainfuck_script_dealloc(ctx.script);
		return NULL;
}

struct BrainfuckScript * brainfuck_parser_parse_file(FILE *file, int *error)
{
	char buffer[BRAINFUCK_BUFFER_SIZE] = {0};
	struct BrainfuckParserContext ctx = {NULL, {0, 0, NULL}, 0};
	int reserved = BRAINFUCK_EOK;
	
	error = error ? error : &reserved;
	*error = BRAINFUCK_EOK;
	
	while (!feof(file)) {
		memset(&buffer, 0, sizeof(buffer));
		(void) fread(&buffer, 1, sizeof(buffer), file);
		if (ferror(file)) 
			continue;

		brainfuck_parser_parse_string_segment(buffer, &ctx, error);
		
		if (*error != BRAINFUCK_EOK)
			goto error;
	}

	if (!brainfuck_parser_validate(&ctx)) {
		*error = BRAINFUCK_ESYNTAX;
		goto error;
	}
	
	BRAINFUCK_DEALLOC(ctx.scopes.array);
	return ctx.script;
	
	error:
		BRAINFUCK_DEALLOC(ctx.scopes.array);
		if (ctx.script)
			brainfuck_script_dealloc(ctx.script);
		return NULL;
}

/* brainfuck/engine.h */
struct BrainfuckEngineContext * brainfuck_engine_context_alloc(void)
{
	return BRAINFUCK_ALLOC(sizeof(struct BrainfuckEngineContext));
}

void brainfuck_engine_context_dealloc(struct BrainfuckEngineContext *ctx)
{
	BRAINFUCK_DEALLOC(ctx);
}

int brainfuck_engine_run(const struct BrainfuckScript *script, 
	struct BrainfuckEngineContext *ctx)
{
	register struct BrainfuckInstruction *inst = script->head;
	register uint8_t *mem = ctx->memory;
	register int index = 0;
	
	while (inst) {
		switch(inst->opcode) {
		case ADD:
			mem[index + inst->y.offset] += inst->x.offset;
			break;
		case SUB:
			mem[index + inst->y.offset] -= inst->x.offset;
			break;
		case MOVL:
			index -= inst->x.offset;
			break;
		case MOVR:
			index += inst->x.offset;
			break;
		case CPY:
			mem[index + inst->y.offset] = mem[index + inst->x.offset];
			break;
		case MUL:
			mem[index + inst->y.offset] = mem[index + inst->x.offset] * inst->z.multiplier;
			break;
		case CLR:
			mem[index + inst->x.offset] = 0;
			break;
		case IN:
			mem[index + inst->x.offset] = ctx->read();
			break;
		case OUT:
			ctx->write(mem[index + inst->x.offset]);
			break;
		case JNZ:
			if (!mem[index])
				break;
			/* Fallthrough */
		case JMP:
			inst = inst->x.target;
			continue;
		case NOP:
			break;
		}
		inst = inst->next;
	}
	return BRAINFUCK_EOK;
}

void brainfuck_script_dump(const struct BrainfuckScript *script, FILE *file)
{
	struct BrainfuckInstruction* inst = script->head;
	while (inst) {
		switch(inst->opcode) {
		case ADD:
			fprintf(file, "%p: add $%d, %d\n", inst, inst->x.offset, inst->y.offset);
			break;
		case SUB:
			fprintf(file, "%p: sub $%d, %d\n", inst, inst->x.offset, inst->y.offset);
			break;
		case MOVL:
			fprintf(file, "%p: movl %d\n", inst, inst->x.offset);
			break;
		case MOVR:
			fprintf(file, "%p: movr %d\n", inst, inst->x.offset);
			break;
		case CLR:
			fprintf(file, "%p: clr %d\n", inst, inst->x.offset);
			break;
		case CPY:
			fprintf(file, "%p: cpy %d, %d\n", inst, inst->x.offset, inst->y.offset);
			break;
		case MUL:
			fprintf(file, "%p: mul %d, %d, $%d\n", inst, inst->x.offset, inst->y.offset, inst->z.multiplier);
			break;
		case IN:
			fprintf(file, "%p: in %d\n", inst, inst->x.offset);
			break;
		case OUT:
			fprintf(file, "%p: out %d\n", inst, inst->x.offset);
			break;
		case JNZ:
			fprintf(file, "%p: jnz %p\n", inst, inst->x.target);
			break;
		case JMP:
			fprintf(file, "%p: jmp %p\n", inst, inst->x.target);
			break;
		case NOP:
			fprintf(file, "%p: nop\n", inst);
			break;
		}
		inst = inst->next;
	}
	fprintf(file, "\n");
}