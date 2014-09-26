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
struct BrainfuckInstruction * brainfuck_instruction_create_cell_mutation(
	int delta)
{
	struct BrainfuckInstruction *instruction = brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->id = BRAINFUCK_ICELL;
	instruction->attributes.delta = delta;
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
	struct BrainfuckInstruction *instruction = brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->id = BRAINFUCK_IINDEX;
	instruction->attributes.delta = delta;
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
	struct BrainfuckInstruction *instruction = brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->id = BRAINFUCK_IOUTPUT;
	instruction->attributes.delta = times;
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
	struct BrainfuckInstruction *instruction = brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->id = BRAINFUCK_IINPUT;
	instruction->attributes.delta = times;
	return instruction;
}

/*
 * Create a loop instruction and return it.
 *
 * @return The break instruction.
 */
struct BrainfuckInstruction * brainfuck_instruction_create_loop()
{
	struct BrainfuckInstruction *instruction = brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->id = BRAINFUCK_ILOOP;
	instruction->attributes.delta = 1;
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
	struct BrainfuckInstruction *instruction = brainfuck_instruction_alloc();
	if (!instruction)
		return NULL;
	instruction->id = BRAINFUCK_IBREAK;
	instruction->attributes.jump = jump;
	return instruction;
}

/*
 * Allocate a new {@link BrainfuckFileStream} from the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckFileStream * brainfuck_stream_file_alloc(void)
{
	return malloc(sizeof(struct BrainfuckFileStream));
}

/* 
 * Free the given {@link BrainfuckStringStream} from the memory.
 * 
 * @param stream The stream to free from the memory.
 */
void brainfuck_stream_file_free(struct BrainfuckFileStream *stream)
{
	free(stream);
	stream = 0;
}

/*
 * Read a character from the stream and return it.
 * 
 * @param stream The stream to unget the character from.
 * @return The character read from the stream.
 */
int brainfuck_stream_file_get(struct BrainfuckStream *stream)
{
	struct BrainfuckFileStream *file_stream = 
		brainfuck_stream_file_cast(stream);
	return fgetc(file_stream->file);
}

/*
 * Unget a character from the stream.
 *
 * @param stream The stream to unget the character from.
 * @param character The character to unget.
 */
void brainfuck_stream_file_unget(struct BrainfuckStream *stream, int character)
{
	struct BrainfuckFileStream *file_stream = 
		brainfuck_stream_file_cast(stream);
	ungetc(character, file_stream->file);
}

/*
 * Initialise the given {@link BrainfuckFileStream} with
 * 	the given {@link FILE}.
 *
 * @param stream The {@ink BrainfuckFileStream} to initialise.
 * @param file The {@link FILE} of the file stream.
 */
void brainfuck_stream_file_init(struct BrainfuckFileStream *stream, FILE *file)
{
	if (!stream)
		return;
	stream->file = file;
	stream->base.get = &brainfuck_stream_file_get;
	stream->base.unget = &brainfuck_stream_file_unget;
}

/*
 * Create a new {@link BrainfuckFileStream} with the given file.
 *
 * @param file The file to create the new file stream with.
 * @return The file stream.
 */
struct BrainfuckFileStream * brainfuck_stream_file(FILE *file)
{
	struct BrainfuckFileStream *stream = brainfuck_stream_file_alloc();
	brainfuck_stream_file_init(stream, file);
	return stream;
}

/*
 * Allocate a new {@link BrainfuckBufferStream} from the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckBufferStream * brainfuck_stream_buffer_alloc()
{
	return malloc(sizeof(struct BrainfuckBufferStream));
}

/* 
 * Free the given {@link BrainfuckBufferStream} from the memory.
 * 
 * @param stream The stream to free from the memory.
 */
void brainfuck_stream_buffer_free(struct BrainfuckBufferStream *stream)
{
	free(stream);
	stream = 0;
}

/*
 * Read a character from the stream and return it.
 * 
 * @param stream The stream to unget the character from.
 * @return The character read from the stream.
 */
int brainfuck_stream_buffer_get(struct BrainfuckStream *stream)
{
	struct BrainfuckBufferStream *buf_stream = 
		brainfuck_stream_buffer_cast(stream);
	if (!stream || buf_stream->index >= buf_stream->length)
		return BRAINFUCK_EOF;
	return buf_stream->buffer[buf_stream->index++];
}

/*
 * Unget a character from the stream.
 *
 * @param stream The stream to unget the character from.
 * @param character The character to unget.
 */
void brainfuck_stream_buffer_unget(struct BrainfuckStream *stream, 
		int character)
{
	struct BrainfuckBufferStream *buf_stream = 
		brainfuck_stream_buffer_cast(stream);
	if (!stream || character < 0)
		return;
	buf_stream->buffer[--buf_stream->index] = character;
}

/*
 * Initialise the given {@link BrainfuckBufferStream} with
 * 	the given buffer, index and length.
 *
 * @param stream The {@ink BrainfuckBufferStream} to initialise.
 * @param buffer The memory buffer of this {@link BrainfuckBufferStream}.
 * @param index The index in memory buffer of this 
 *	{@link BrainfuckBufferStream}.
 * @param length The length of the memory buffer of this 
 *	{@link BrainfuckBufferStream}.
 */
void brainfuck_stream_buffer_init(struct BrainfuckBufferStream *stream, 
		char *buffer, int index, int length)
{
	if (!stream)
		return;
	stream->buffer = buffer;
	stream->index = index;
	stream->length = length;
	stream->base.get = &brainfuck_stream_buffer_get;
	stream->base.unget = &brainfuck_stream_buffer_unget;
}

/*
 * Create a new {@link BrainfuckBufferStream} with the given string.
 *
 * @param string The string to create the new {@link BrainfuckBufferStream} 
 * 	with.
 * @return The created {@link BrainfuckBufferStream}.
 */
struct BrainfuckBufferStream * brainfuck_stream_string(char *string)
{
	struct BrainfuckBufferStream *stream = brainfuck_stream_buffer_alloc();
	char *copy = strdup(string);
	if (!copy)
		return NULL;
	brainfuck_stream_buffer_init(stream, copy, 0, strlen(string));
	return stream;
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
	ctx->mem_size = BRAINFUCK_DEFAULT_MEMSIZE;
	ctx->memory = malloc(sizeof(int) * ctx->mem_size);
	
	if (!ctx->memory)
		return NULL;
	
	ctx->index = 0;
	ctx->running = 0;
	return ctx;
}

/*
 * The {@link BrainfuckJump} structure is a internal strucutre that is used
 *	as a stack that contains loop {@link BrainfuckInstruction}s for 
 *	break {@link BrainfuckInstruction}s to jump back to the loop instruction.
 */
struct BrainfuckJump {
	
	/*
	 * The previous {@link BrainfuckJump} in the list.
	 */
	struct BrainfuckJump *previous;
	
	/*
	 * The pointer to the loop {@link BrainfuckInstruction}.
	 */
	struct BrainfuckInstruction *instruction;
	
} BrainfuckJump;

/*
 * Allocate a new {@link BrainfuckJump} to the heap.
 *
 * @return A pointer to the memory allocated to the heap or 
 *	<code>NULL</code> if there is no memory available.
 */
struct BrainfuckJump * brainfuck_jump_alloc(void)
{
	return malloc(sizeof(struct BrainfuckJump));
}

/* 
 * Free the given {@link BrainfuckJump} from the memory.
 * 
 * @param jump The {@link BrainfuckJump} to free from the memory.
 */
void brainfuck_jump_free(struct BrainfuckJump *jump)
{
	free(jump);
	jump = 0;
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
	if (!string) {
		if (error)
			*error = BRAINFUCK_EPARAM;
		return NULL;
	}
	struct BrainfuckBufferStream *stream = brainfuck_stream_string(string);
	struct BrainfuckScript *script = brainfuck_parse(
		brainfuck_stream_cast(stream), error);
	brainfuck_stream_buffer_free(stream);
	return script;
}

/* 
 * Parse the given {@link FILE}.
 *
 * @param string The string to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse_file(FILE *file, int *error)
{
	if (!file) {
		if (error)
			*error = BRAINFUCK_EPARAM;
		return NULL;
	}
	struct BrainfuckFileStream *stream = brainfuck_stream_file(file);
	struct BrainfuckScript *script = brainfuck_parse(
		brainfuck_stream_cast(stream), error);
	brainfuck_stream_file_free(stream);
	return script;
}

/* 
 * Parse the given {@link BrainfuckStream}.
 *
 * @param stream The stream to read the script from.
 * @param error A pointer to an integer that will be set to either a 
 * 	success or an error code.
 * @return A pointer to a {@link BrainfuckScript} instance or <code>NULL</code> 
 *	if the parsing failed.
 */
struct BrainfuckScript * brainfuck_parse(struct BrainfuckStream *stream, 
	int *error)
{
	struct BrainfuckInstruction *script = NULL;
	struct BrainfuckInstruction *top = NULL;
	struct BrainfuckInstruction *instruction = NULL;
	struct BrainfuckJump *jump_list = NULL;
	struct BrainfuckJump *jump;
	char character;
	int delta;
	int error_holder = BRAINFUCK_OK;
	
	if (!error)
		error = &error_holder;
	if (!stream) {
		*error = BRAINFUCK_EPARAM;
		return NULL;
	}
	
	while((character = stream->get(stream)) != EOF) {
		delta = 1;
		switch(character) {
		case '-':
			delta = -1;
		case '+':
			while ((character = stream->get(stream)) == '+' 
					|| character == '-')
				delta += (character == '+' ? 1 : -1);
			stream->unget(stream, character);
			instruction = brainfuck_instruction_create_cell_mutation(delta);
			break;
		case '<':
			delta = -1;
		case '>':
			while ((character = stream->get(stream)) == '>' 
					|| character == '<')
				delta += (character == '>' ? 1 : -1);
			stream->unget(stream, character);
			instruction = brainfuck_instruction_create_index_mutation(delta);
			break;
		case '.':
			while ((character = stream->get(stream)) == '.')
				delta++;
			stream->unget(stream, character);
			instruction = brainfuck_instruction_create_output(delta);
			break;
		case ',':
			while ((character = stream->get(stream)) == ',')
				delta++;
			stream->unget(stream, character);
			instruction = brainfuck_instruction_create_input(delta);
			break;
		case '[':
			instruction = brainfuck_instruction_create_loop();
			jump = brainfuck_jump_alloc();
			if (!jump) {
				*error = BRAINFUCK_ENOMEM;
				return NULL;
			}
			jump->instruction = instruction;
			jump->previous = jump_list;
			jump_list = jump;
			break;
		case ']':
			jump = jump_list;
			if (!jump) {
				*error = BRAINFUCK_ESYNTAX;
				return NULL;
			}
			jump_list = jump_list->previous;
			instruction = brainfuck_instruction_create_break(jump->instruction);
			jump->instruction->attributes.jump = instruction;
			brainfuck_jump_free(jump);
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
	while (ctx->instruction && ctx->running) {
		switch(ctx->instruction->id) {
		case BRAINFUCK_ICELL:
			ctx->memory[ctx->index] += ctx->instruction->attributes.delta;
			break;
		case BRAINFUCK_IINDEX:
			ctx->index += ctx->instruction->attributes.delta;
			break;
		case BRAINFUCK_IINPUT:
			ctx->memory[ctx->index] = ctx->read();
			break;
		case BRAINFUCK_IOUTPUT:
			ctx->write(ctx->memory[ctx->index]);
			break;
		case BRAINFUCK_ILOOP:
			if (ctx->memory[ctx->index])
				break;
			ctx->instruction = ctx->instruction->attributes.jump;
			continue;
		case BRAINFUCK_IBREAK:
			if (!ctx->memory[ctx->index])
				break;
			ctx->instruction = ctx->instruction->attributes.jump;
			continue;
		}
		ctx->instruction = ctx->instruction->next;
	}
	return BRAINFUCK_OK;
}