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
 * This header file defines the engine interface for this library.
 */
#ifndef BRAINFUCK_ENGINE_H
#define BRAINFUCK_ENGINE_H

/* Dependencies */
#include <stdint.h>

/**
 * An interface of an engine that is able to run a {@link BrainfuckScript}.
 *
 * @addtogroup engine Engine
 * @{
 */
/**
 * This structure represents the context in which a script should run.
 */
struct BrainfuckEngineContext {
	/** 
	 * The memory a script should operate on.
	 */
	uint8_t *memory;
	
	/**
	 * Read a character from an input stream.
	 *
	 * @return The integer that was read from the input stream.
	 */
	int (*read)(void);

	/**
	 * Write a character to an output stream.
	 * 
	 * @param[in] c An integer to write to the output stream.
	 * @return On success, the written character is returned. If a writing 
	 * error occurs, EOF is returned.
	 */
	int (*write)(const int c);
};

/**
 * Allocate a {@link BrainfuckEngineContext} on the heap.
 *
 *
 * @return A pointer to the allocated structure or <code>NULL</code> if the 
 *	allocation failed.
 */
struct BrainfuckEngineContext * brainfuck_engine_context_alloc(void);

/**
 * Deallocate an engine context.
 * 
 * The {@link BrainfuckEngineContext#memory} field is not deallocated by
 * this function, and the user should do this instead.
 * 
 * @param[in] ctx The context to deallocate.
 */
void brainfuck_engine_context_dealloc(struct BrainfuckEngineContext *ctx);

/**
 * Run the given script using an interpreter.
 *
 * @param[in] script The script to run.
 * @param[in] ctx The context in which the execution takes place.
 * @return {@link BRAINFUCK_EOK} on success, one of the defined error codes
 * on failure.
 */
int brainfuck_engine_run(const struct BrainfuckScript *script, 
	struct BrainfuckEngineContext *ctx);
/** @} */

#endif /* BRAINFUCK_ENGINE_H */