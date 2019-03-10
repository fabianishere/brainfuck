/*
 * Copyright (c) 2019 Fabian Mastenbroek
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
 * This header file defines some general information about this library and
 * introduces constants used by the library.
 */
#ifndef BRAINFUCK_H
#define BRAINFUCK_H

/**
 * The macro definitions defined by this library.
 *
 * @addtogroup definitions Definitions
 * @{
 */
/**
 * Compile-time version information of this library.
 *
 * @see version_runtime
 * @addtogroup version_compiletime Compile-time version information
 * @{
 */
/**
 * A macro that evaluates to a string version of the library with which the
 * header file is associated.
 */
#define BRAINFUCK_VERSION        "3.0.0"

/**
 * A macro that resolves to an integer <code>(X*1000000 + Y*1000 + Z)</code>
 * where X, Y, and Z are the major version, minor version, and release number.
 */
#define BRAINFUCK_VERSION_NUMBER  3000000
/** @} */

/**
 * Error codes returned by routines of this library.
 *
 * API routines of this library return one of the set shown below, in order to
 * indicate success or failure.
 *
 * @addtogroup error Error codes
 * @{
 */
#define BRAINFUCK_EOK          0  /**< Successful result */
#define BRAINFUCK_EINTERNAL    1  /**< Internal logic error in implementation */
#define BRAINFUCK_EUNAVAILABLE 2  /**< Feature not implemented or unavailable */
#define BRAINFUCK_ENOMEM       3  /**< A malloc() failure */
#define BRAINFUCK_ESYNTAX      4  /**< Syntax error in input */
#define BRAINFUCK_EBOUNDS      5  /**< Input caused index to go out of bounds */
#define BRAINFUCK_EIO          6  /**< I/O error while reading or writing */
/** @} */
/** @} */

/**
 * Miscellaneous library routines.
 *
 * @addtogroup misc Miscellaneous
 * @{
 */
/**
 * Runtime version information of this library.
 *
 * @see version_compiletime
 * @addtogroup version_runtime Runtime version information
 * @{
 */
/**
 * A constant string identical to the contents of the {@link BRAINFUCK_VERSION}
 * macro of the header the library is compiled against.
 */
extern const char brainfuck_version[];

/**
 * An integer equal to {@link BRAINFUCK_VERSION_NUMBER} of the header
 * the library is compiled against.
 */
extern const int brainfuck_version_number;
/** @} */
/** @} */
#endif /* BRAINFUCK_H */
