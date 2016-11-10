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
 * This header file defines the Just in Time (JIT) compiler interface for this library.
 * The macro BRAINFUCK_JIT_AVAILABLE should be defined on platforms supporting a
 * Just in Time compiler implementation.
 */
#ifndef BRAINFUCK_JIT_H
#define BRAINFUCK_JIT_H

/* Dependencies */
#include <brainfuck/engine.h>

/**
 * A {@link BrainfuckCompiledScript} is a function that can be called given
 * a {@link BrainfuckEngineContext} which runs the script that is compiled.
 */
typedef int (*BrainfuckCompiledScript)(struct BrainfuckEngineContext *ctx);

/**
 * Deallocate the given {@link BrainfuckCompilerScript}.
 *
 * @param script The compiled script to deallocate.
 */
void brainfuck_jit_dealloc(BrainfuckCompiledScript script); 

/**
 * Compile the given {@link BrainfuckScript} into machine code.
 *
 * @param[in] script The script to compile.
 * @param[out] error A pointer to an integer that will be set to either a 
 * success code or an error code.
 * @return A pointer to the compiled script or <code>NULL</code> if the compilation failed.
 */
BrainfuckCompiledScript brainfuck_jit_compile(struct BrainfuckScript *script, int *error);

#endif /* BRANFUCK_JIT_H */