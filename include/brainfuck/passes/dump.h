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
 * A pass that dumps the intermediate representation of the script to the given
 * file stream.
 * The macro BRAINFUCK_PASS_DUMP_AVAILABLE should be defined by implementations
 * supporting this pass.
 */
#ifndef BRAINFUCK_PASS_DUMP_H
#define BRAINFUCK_PASS_DUMP_H

/** @addtogroup pass Pass */
/** @{ */

/**
 * A flag that indicates whether this pass is available.
 */
const int brainfuck_pass_dump_available;

/**
 * This pass dumps the intermediate representation of the given scripts to the
 * given file stream in an assembly-like format.
 *
 * @param[in] script The script to dump.
 * @param[in] file The file to dump the intermediate representation to.
 */
void brainfuck_pass_dump(const struct BrainfuckScript *script, FILE *file);
/** @} */
#endif /* BRAINFUCK_PASS_DUMP_H */