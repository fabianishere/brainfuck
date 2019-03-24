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
 * This header file defines the virtual machine interface that runs a
 * brainfuck-based instruction set.
 */
#ifndef BRAINIAC_VM_H
#define BRAINIAC_VM_H

#include <brainiac/ir.h>

/**
 * An interface of an engine that is able to run a {@link BrainiacProgram}.
 *
 * @addtogroup vm Virtual Machine
 * @{
 */
/**
 * This structure represents the virtual machine context in which a program
 * runs.
 */
struct BrainiacVmContext {
    /**
     * The {@link BrainiacVm} associated with this context.
     */
    const struct BrainiacVm *vm;

    /**
     * The size of the memory to operate on.
     */
    size_t memory_size;

    /**
     * The memory a program should operate on.
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
     * @param[in] c The integer to write to the output stream.
     * @return On success, the written character is returned. If a writing
     * error occurs, EOF is returned.
     */
    int (*write)(const int c);
};

/**
 * This structure represents some virtual machine that provides an execution
 * engine for the brainfuck-based instruction set defined in @ref ir.
 */
struct BrainiacVm {
    /**
     * A unique, full name of the virtual machine implementation.
     */
    const char *name;

    /**
     * The version of the virtual machine implementation.
     */
    const char *version;

    /**
     * Allocate a {@link BrainiacVmContext} in which a brainfuck program runs.
     *
     * @param[in] vm The {@link BrainiacVm} to allocate a context for.
     * @return The allocated context or <code>NULL</code> on allocation failure.
     */
    struct BrainiacVmContext * (*alloc)(struct BrainiacVm *vm);

    /**
     * Deallocate the specified {@link BrainiacVmContext}.
     *
     * @param[in] ctx The virtual machine context to deallocate.
     */
    void (*dealloc)(struct BrainiacVmContext *ctx);

    /**
     * Run the given program in the specified {@link BrainiacVmContext}.
     *
     * @param[in] ctx The virtual machine context to run the program in.
     * @param[in] program The program to run.
     * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
     * on failure.
     */
    int (*run)(struct BrainiacVmContext *ctx,
               const struct BrainiacProgram *program);
};

/**
 * Allocate a {@link BrainiacVmContext} in which a brainfuck program runs.
 *
 * @param[in] vm The {@link BrainiacVm} to allocate a context for.
 * @return The allocated {@link BrainiacVmContext} or <code>NULL</code> on
 * allocation failure.
 */
struct BrainiacVmContext * brainiac_vm_alloc(struct BrainiacVm *vm);

/**
 * Deallocate the specified {@link BrainiacVmContext}.
 *
 * @param[in] ctx The virtual machine context to deallocate.
 */
void brainiac_vm_dealloc(struct BrainiacVmContext *ctx);

/**
 * Run the given program in the specified {@link BrainiacVmContext}.
 *
 * @param[in] ctx The virtual machine context to run the program in.
 * @param[in] program The program to run.
 * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
 * on failure.
 */
int brainiac_vm_run(struct BrainiacVmContext *ctx,
                    const struct BrainiacProgram *program);

/**
 * Retrieve an array of available virtual machines on this platform.
 *
 * @return A <code>NULL</code>-terminated array of available virtual machines.
 */
struct BrainiacVm ** brainiac_vm_list(void);

/**
 * Find the first virtual machine available.
 *
 * @param[in] name The name of the virtual machine to find. If <code>NULL</code>
 * is given, then the first vm that is found will be returned.
 * @return The virtual machine that was found or <code>NULL</code> if none are
 * available.
 */
struct BrainiacVm * brainiac_vm_find(const char *name);
/** @} */

#endif /* BRAINIAC_VM_H */
