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
#ifndef BRAINIAC_H
#define BRAINIAC_H

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
#define BRAINIAC_VERSION        "3.0.0"

/**
 * A macro that resolves to an integer <code>(X*1000000 + Y*1000 + Z)</code>
 * where X, Y, and Z are the major version, minor version, and release number.
 */
#define BRAINIAC_VERSION_NUMBER  3000000
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
#define BRAINIAC_EOK          0  /**< Successful result */
#define BRAINIAC_EINTERNAL    1  /**< Internal logic error in implementation */
#define BRAINIAC_EUNAVAILABLE 2  /**< Feature not implemented or unavailable */
#define BRAINIAC_ENOMEM       3  /**< A malloc() failure */
#define BRAINIAC_ESYNTAX      4  /**< Syntax error in input */
#define BRAINIAC_EBOUNDS      5  /**< Input caused index to go out of bounds */
#define BRAINIAC_EIO          6  /**< I/O error while reading or writing */
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
 * A constant string identical to the contents of the {@link BRAINIAC_VERSION}
 * macro of the header the library is compiled against.
 */
extern const char brainiac_version[];

/**
 * An integer equal to {@link BRAINIAC_VERSION_NUMBER} of the header
 * the library is compiled against.
 */
extern const int brainiac_version_number;
/** @} */
/** @} */
#endif /* BRAINIAC_H */
