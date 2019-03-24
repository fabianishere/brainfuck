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

#include <brainiac/brainiac.h>
#include <brainiac/vm.h>
#include <brainiac/ir.h>

#include <lightning.h>

#include "lightning.h"
#include "../../brainiac.h"

struct BrainiacVmContextLightning {
    struct BrainiacVmContext base;
    jit_state_t *jit;
};

static struct BrainiacVmContext * alloc(struct BrainiacVm *vm)
{
    struct BrainiacVmContextLightning *ctx = BRAINIAC_ALLOC(sizeof(struct BrainiacVmContextLightning));

    if (ctx) {
        ctx->base.vm = vm;
        ctx->base.memory = NULL;
        ctx->base.memory_size = 0;
        ctx->base.read = NULL;
        ctx->base.write = NULL;

        init_jit(NULL);
        ctx->jit = jit_new_state();
    }

    return (void *) ctx;
}

static void dealloc(struct BrainiacVmContext *ctx)
{
    finish_jit();
    BRAINIAC_DEALLOC(ctx);
}

static struct BrainiacInstruction * emit(struct BrainiacVmContext *ctx,
                                          const struct BrainiacInstruction *inst,
                                          jit_node_t *target_label, jit_node_t *target_jmp)
{
    jit_state_t *_jit = ((struct BrainiacVmContextLightning *) ctx)->jit;
    int word_size = 4;

    while (inst) {
        switch(inst->opcode) {
            case ADD:
                jit_ldxi_i(JIT_R0, JIT_V0, inst->operands[0].i32 * word_size);
                jit_addi(JIT_R0, JIT_R0, inst->operands[1].i32);
                jit_stxi_i(inst->operands[0].i32 * word_size, JIT_V0, JIT_R0);
                break;
            case MOV:
                jit_addi(JIT_V0, JIT_V0, inst->operands[0].i32 * word_size);
                break;
            case CPY:
                jit_ldxi_i(JIT_R0, JIT_V0, inst->operands[0].i32 * word_size);
                jit_stxi_i(inst->operands[1].i32 * word_size, JIT_V0, JIT_R0);
                break;
            case MUL:
                jit_ldxi(JIT_R0, JIT_V0, inst->operands[0].i32 * word_size);
                jit_muli(JIT_R0, JIT_R0, inst->operands[2].i8);
                jit_stxi_i(inst->operands[1].i32 * word_size, JIT_V0, JIT_R0);
                break;
            case CLR:
                jit_movi(JIT_R0, 0);
                jit_stxi_i(inst->operands[0].i32 * word_size, JIT_V0, JIT_R0);
                break;
            case IN:
                jit_prepare();
                jit_finishi(ctx->read);
                jit_retval(JIT_R0);
                jit_stxi_i(inst->operands[0].i32 * word_size, JIT_V0, JIT_R0);
                break;
            case OUT:
                jit_ldxi_i(JIT_R0, JIT_V0, inst->operands[0].i32 * word_size);
                jit_prepare();
                jit_pushargr(JIT_R0);
                jit_finishi(ctx->write);
                break;
            case JNZ: {
                jit_patch(target_jmp);
                jit_ldr_i(JIT_R0, JIT_V0);
                jit_node_t *jmp = jit_bnei(JIT_R0, 0);
                jit_patch_at(jmp, target_label);
                return inst->next;
            }
            case JMP: {
                jit_node_t *jmp = jit_jmpi();
                jit_node_t *label = jit_label();
                inst = emit(ctx, inst->next, label, jmp);
                continue;
            }
            case NOP:
                break;
        }
        inst = inst->next;
    }

    return NULL;
}

static int run(struct BrainiacVmContext *ctx,
               const struct BrainiacProgram *program)
{
    jit_state_t *_jit = ((struct BrainiacVmContextLightning *) ctx)->jit;
    uint8_t *mem = ctx->memory;
    struct BrainiacInstruction *inst = program->head;
    jit_prolog();
    jit_movi(JIT_V0, (jit_word_t) mem);
    emit(ctx, inst, NULL, NULL);
    jit_ret();
    jit_epilog();
    void (*run)(void) = jit_emit();
    run();
    jit_clear_state();
    return BRAINIAC_EOK;
}

struct BrainiacVm brainiac_vm_lightning = {
    .name = "lightning",
    .version = "1.0.0",
    .alloc = &alloc,
    .dealloc = &dealloc,
    .run = &run,
};
