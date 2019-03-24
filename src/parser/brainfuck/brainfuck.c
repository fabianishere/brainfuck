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
#include <brainiac/parser.h>
#include <brainiac/ir.h>

#include "brainfuck.h"
#include "../../brainiac.h"

struct BrainfuckParserContext {
    struct BrainiacParserContext base;

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
        struct BrainiacInstruction **array;
    } scopes;

    /**
     * The current memory offset.
     */
    int32_t offset;
};

static struct BrainiacParserContext * alloc(struct BrainiacParser *parser,
                                             struct BrainiacProgram *program)
{
    struct BrainfuckParserContext *ctx = BRAINIAC_ALLOC(sizeof(struct BrainfuckParserContext));

    if (ctx) {
        ctx->base.parser = parser;
        ctx->base.program = program;
        ctx->scopes.size = BRAINIAC_SCOPE_DEPTH;
        ctx->scopes.index = 0;
        ctx->scopes.array = BRAINIAC_ALLOC(
            sizeof(struct BrainiacInstruction *) * BRAINIAC_SCOPE_DEPTH);
        ctx->offset = 0;

        if (!ctx->scopes.array) {
            BRAINIAC_DEALLOC(ctx);
            return NULL;
        }
    }

    return (struct BrainiacParserContext *) ctx;
}

static void dealloc(struct BrainiacParserContext *ctx)
{
    BRAINIAC_DEALLOC(((struct BrainfuckParserContext *) ctx)->scopes.array);
    BRAINIAC_DEALLOC(ctx);
}

static void start(struct BrainiacParserContext *ctx)
{
    (void) ctx;
}

static int consume(struct BrainiacParserContext *ctx, const char *input)
{
    char c;
    int32_t delta = 0;
    int32_t offset = 0;

    struct BrainfuckParserContext *bctx = (void *) ctx;
    struct BrainiacInstruction *inst;
    struct BrainiacInstruction *tmp;

    /* Set current offset */
    offset = bctx->offset;

    while((c = *input++)) {
        switch(c) {
            /* ADD instructions */
            case '+':
            case '-':
                /* Consume until token of different type */
                delta = 0;
                do {
                    delta += (c == '+' ? 1 : -1); /* TODO Handle overflow */
                } while (((c = *input) == '+' || c == '-') && input++);

                if (delta == 0) {
                    continue;
                } else if ((inst = brainiac_ir_alloc(ADD)) == NULL) {
                    return BRAINIAC_ENOMEM;
                }
                inst->operands[0].i32 = offset;
                inst->operands[1].i32 = delta;
                brainiac_ir_append(ctx->program, inst);
                break;

                /* MOV instructions */
            case '>':
            case '<':
                /* Consume until token of different type */
                delta = 0;
                do {
                    delta += (c == '>' ? 1 : -1); /* TODO Handle overflow */
                } while (((c = *input) == '>' || c == '<') && input++);

                /* Increase/decrease current memory offset accordingly */
                offset += delta;
                break; /* Don't allocate new instruction */
                /* I/O instructions */
            case ',':
                if ((inst = brainiac_ir_alloc(IN)) == NULL) {
                    return BRAINIAC_ENOMEM;
                }
                inst->operands[0].i32 = offset;
                brainiac_ir_append(ctx->program, inst);
                break;
            case '.':
                if ((inst = brainiac_ir_alloc(OUT)) == NULL) {
                    return BRAINIAC_ENOMEM;
                }
                inst->operands[0].i32 = offset;
                brainiac_ir_append(ctx->program, inst);
                break;

                /* Flow control instructions */
            case '[':
                /* Have we reached the scope upper limit */
                if ((size_t) bctx->scopes.index >= bctx->scopes.size) {
                    return BRAINIAC_ENOMEM;
                }

                /* Reset offset before jump */
                if (offset != 0) {
                    if ((inst = brainiac_ir_alloc(MOV)) == NULL) {
                        return BRAINIAC_ENOMEM;
                    }
                    inst->operands[0].i32 = offset;
                    brainiac_ir_append(ctx->program, inst);
                    offset = 0;
                }

                /* Add JMP instruction */
                if ((inst = brainiac_ir_alloc(JMP)) == NULL) {
                    return BRAINIAC_ENOMEM;
                }
                inst->operands[0].ref = NULL;
                brainiac_ir_append(ctx->program, inst);

                /* Put jump on stack */
                bctx->scopes.array[bctx->scopes.index++] = inst;
                break;
            case ']':
                /* Have we reached the scope lower limit */
                if (bctx->scopes.index <= 0) {
                    return BRAINIAC_ESYNTAX;
                }

                /* Reset offset before jump */
                if (offset != 0) {
                    if ((inst = brainiac_ir_alloc(MOV)) == NULL) {
                        return BRAINIAC_ENOMEM;
                    }
                    inst->operands[0].i32 = offset;
                    brainiac_ir_append(ctx->program, inst);
                    offset = 0;
                }

                /* Add JNZ instruction */
                if ((inst = brainiac_ir_alloc(JNZ)) == NULL) {
                    return BRAINIAC_ENOMEM;
                }
                inst->operands[0].ref = NULL;
                brainiac_ir_append(ctx->program, inst);

                /* Get and set the jump targets */
                tmp = bctx->scopes.array[--bctx->scopes.index];
                inst->operands[0].ref = tmp->next;
                tmp->operands[0].ref = inst;
                break;
            default:
                /* Ignore any unknown character */
                continue;
        }
    }

    /* Set offset */
    bctx->offset = offset;
    return BRAINIAC_EOK;
}

int end(struct BrainiacParserContext *ctx)
{
    if (((struct BrainfuckParserContext *) ctx)->scopes.index != 0) {
        return BRAINIAC_ESYNTAX;
    }

    return BRAINIAC_EOK;
}

struct BrainiacParser brainiac_parser_brainfuck = {
    .name = "brainfuck",
    .version = "1.0.0",
    .extensions = (const char *[]) { "b", "bf", "brainfuck", NULL },
    .alloc = &alloc,
    .dealloc = &dealloc,
    .start = &start,
    .consume = &consume,
    .end = &end
};
