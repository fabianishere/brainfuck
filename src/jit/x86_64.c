/*
 * Copyright 2016 Fabian Mastenbroek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include <brainfuck.h>
#include <jit.h>

/**
 * This structure describes the state of the Just-in-Time compiler.
 */
struct BrainfuckJitState {
    /**
     * The memory containing the (partially) compiled program.
     */
    unsigned char *memory;

    /**
     * The size of the memory block.
     */
    size_t size;

    /**
     * The index in the memory.
     */
    int index;
};

/**
 * Emit the given program to memory.
 *
 * @param state The state of the JIT compiler.
 * @param root The first in the list of instructions.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_program(struct BrainfuckJitState *state,
                                       BrainfuckInstruction *root,
                                       BrainfuckExecutionContext *context);

/**
 * Allocate the given amount of bytes from the heap. This allocated memory can
 * be used to write the emitted code to which can be marked executable.
 *
 * @param size The amount of bytes to allocate.
 * @return The pointer to the memory block that has been allocated or
 * <code>null</code> on failure.
 */
static void* brainfuck_jit_alloc(size_t size)
{
    void* mem = mmap(0, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return mem;
}

/**
 * Mark the given block in memory as executable, but not writable.
 *
 * @param state The state of the compiler.
 * @return <code>0</code> on success, <code>-1</code> otherwise.
 */
static int brainfuck_jit_mark_executable(struct BrainfuckJitState *state)
{
    if (mprotect(state->memory, state->size, PROT_READ | PROT_EXEC) != 0) {
        perror("mprotect");
        return -1;
    }
    return 0;
}

/**
 * Write the given bytes to memory.
 *
 * @param state The state of the compiler.
 * @param memory The memory to write.
 * @param size The size of the memory block to write.
 */
static void brainfuck_jit_emit(struct BrainfuckJitState *state,
                               const unsigned char *memory,
                               size_t size) {
    memcpy(state->memory + state->index, memory, size);
    state->index += size;
}

/**
 * Write the prologue of the program to the memory.
 *
 * @param state The state of the compiler.
 */
static void brainfuck_jit_emit_prologue(struct BrainfuckJitState *state)
{
    static const unsigned char prologue[] = {
            0x55,                   /* push %rbp */
            0x48, 0x89, 0xE5,       /* mov %rsp, %rbp */
            0x48, 0x83, 0xEC, 0x20  /* sub $20, %rsp */
    };
    brainfuck_jit_emit(state, prologue, sizeof(prologue));
}

/**
 * Write the epilogue of the program to the memory.
 *
 * @param state The state of the compiler.
 */
static void brainfuck_jit_emit_epilogue(struct BrainfuckJitState *state)
{
    static const unsigned char epilogue[] = {
            0x48, 0x89, 0xEC,   /* mov %rbp, %rsp */
            0x5D,               /* pop %rbp */
            0xC3                /* ret */
    };
    brainfuck_jit_emit(state, epilogue, sizeof(epilogue));
}

/**
 * Create a dump of the memory at the given address with the given length.
 *
 * @param state The state of the compiler.
 */
static void brainfuck_jit_dump(const struct BrainfuckJitState *state)
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = state->memory;
    size_t len = state->size;

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %lu\n", len);
        return;
    }


    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf("  %s\n", buff);
            printf("  %04x ", i);
        }
        printf(" %02x", pc[i]);
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }
    printf("  %s\n", buff);
}

/**
 * Emit an ADD instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_add(struct BrainfuckJitState *state,
                                   BrainfuckInstruction *instruction,
                                   BrainfuckExecutionContext *context)
{
    const unsigned char code[] = {};
    brainfuck_jit_emit(state, code, sizeof(code));
}

/**
 * Emit a SUB instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_sub(struct BrainfuckJitState *state,
                                   BrainfuckInstruction *instruction,
                                   BrainfuckExecutionContext *context)
{
    const unsigned char code[] = {};
    brainfuck_jit_emit(state, code, sizeof(code));
}

/**
 * Emit an MOVL instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_movl(struct BrainfuckJitState *state,
                                    BrainfuckInstruction *instruction,
                                    BrainfuckExecutionContext *context)
{
    const unsigned char code[] = {};
    brainfuck_jit_emit(state, code, sizeof(code));
}

/**
 * Emit a MOVR instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_movr(struct BrainfuckJitState *state,
                                    BrainfuckInstruction *instruction,
                                    BrainfuckExecutionContext *context)
{
    const unsigned char code[] = {};
    brainfuck_jit_emit(state, code, sizeof(code));
}

/**
 * Emit an INPUT instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_input(struct BrainfuckJitState *state,
                                     BrainfuckInstruction *instruction,
                                     BrainfuckExecutionContext *context)
{
    const unsigned char code[] = {};
    brainfuck_jit_emit(state, code, sizeof(code));
}

/**
 * Emit an OUTPUT instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_output(struct BrainfuckJitState *state,
                                      BrainfuckInstruction *instruction,
                                      BrainfuckExecutionContext *context)
{
    const unsigned char code[] = {};
    brainfuck_jit_emit(state, code, sizeof(code));
}

/**
 * Emit a single instruction to memory.
 *
 * @param state The state of the JIT compiler.
 * @param instruction The instruction to emit.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_instruction(struct BrainfuckJitState *state,
                                           BrainfuckInstruction *instruction,
                                           BrainfuckExecutionContext *context)
{
    switch (instruction->type) {
        case BRAINFUCK_TOKEN_PLUS:
            brainfuck_jit_emit_add(state, instruction, context);
            break;
        case BRAINFUCK_TOKEN_MINUS:
            brainfuck_jit_emit_sub(state, instruction, context);
            break;
        case BRAINFUCK_TOKEN_PREVIOUS:
            brainfuck_jit_emit_movl(state, instruction, context);
            break;
        case BRAINFUCK_TOKEN_NEXT:
            brainfuck_jit_emit_movr(state, instruction, context);
            break;
        case BRAINFUCK_TOKEN_INPUT:
            brainfuck_jit_emit_input(state, instruction, context);
            break;
        case BRAINFUCK_TOKEN_OUTPUT:
            brainfuck_jit_emit_output(state, instruction, context);
            break;
        case BRAINFUCK_TOKEN_LOOP_START:
            brainfuck_jit_emit_program(state, instruction, context);
            break;
        default:
            break;
    }
}

/**
 * Emit the given program to memory.
 *
 * @param state The state of the JIT compiler.
 * @param root The first in the list of instructions.
 * @param context The context within the execution takes place.
 */
static void brainfuck_jit_emit_program(struct BrainfuckJitState *state,
                                       BrainfuckInstruction *root,
                                       BrainfuckExecutionContext *context)
{
    while (root != NULL) {
        brainfuck_jit_emit_instruction(state, root, context);
        root = root->next;
    }
}

/**
 * Execute the given list of instructions using a Just-in-Time compiler.
 *
 * @param root The first in the list of instructions.
 * @param context The context within the execution takes place.
 * @return A negative non-zero integer to indicate a failure, zero on success.
 */
int brainfuck_execute_jit(BrainfuckInstruction *root,
                          BrainfuckExecutionContext *context)
{
    struct BrainfuckJitState state;
    size_t size = 1000;

    /* Allocate writable memory */
    state.memory = brainfuck_jit_alloc(size);
    state.size = size;
    state.index = 0;

    brainfuck_jit_emit_prologue(&state);
    brainfuck_jit_emit_program(&state, root, context);
    brainfuck_jit_emit_epilogue(&state);
    brainfuck_jit_mark_executable(&state);
    brainfuck_jit_dump(&state);
    void (*program)() = (void *) state.memory;
    program();
    return 0;
}
