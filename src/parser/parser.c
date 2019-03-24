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

#include <string.h>

#include <brainiac/brainiac.h>
#include <brainiac/parser.h>

#include "brainiac.h"

#ifdef BRAINIAC_PARSER_BRAINFUCK_ENABLED
    #include "./brainfuck/brainfuck.h"
#endif

/**
 * Internal array containing the available parser implementations.
 */
static struct BrainiacParser *parsers[] = {
#ifdef BRAINIAC_PARSER_BRAINFUCK_ENABLED
    &brainiac_parser_brainfuck,
#endif
    NULL
};

struct BrainiacParserContext * brainiac_parser_alloc(struct BrainiacParser *parser,
                                                     struct BrainiacProgram *program)
{
    return parser->alloc(parser, program);
}

void brainiac_parser_dealloc(struct BrainiacParserContext *ctx)
{
    ctx->parser->dealloc(ctx);
}

void brainiac_parser_start(struct BrainiacParserContext *ctx)
{
    ctx->program->head = NULL;
    ctx->program->tail = NULL;
    ctx->parser->start(ctx);
}

int brainiac_parser_consume_string(struct BrainiacParserContext *ctx,
                                   const char *input)
{
    return ctx->parser->consume(ctx, input);
}

int brainiac_parser_consume_file(struct BrainiacParserContext *ctx,
                                 FILE *file)
{
    char buffer[BRAINIAC_BUFFER_SIZE] = {0};
    int err;

    while (!feof(file)) {
        size_t count = sizeof(buffer) - 1, read;
        if ((read = fread(&buffer, sizeof(char), count, file)) != count) {
            if (ferror(file))
                return BRAINIAC_EIO;
        }
        buffer[read] = '\0'; /* Null-terminate buffer */

        if ((err = brainiac_parser_consume_string(ctx, buffer)) != BRAINIAC_EOK) {
            return err;
        }
    }

    return BRAINIAC_EOK;
}

int brainiac_parser_end(struct BrainiacParserContext *ctx)
{
    return ctx->parser->end(ctx);
}

struct BrainiacParser ** brainiac_parser_list(void)
{
    return parsers;
}

struct BrainiacParser * brainiac_parser_find(const char *name)
{
    struct BrainiacParser **parser;

    for (parser = parsers; *parser; parser++) {
        if (!name || !strcmp((*parser)->name, name)) {
            return *parser;
        }
    }

    return NULL;
}


struct BrainiacParser * brainiac_parser_find_by_extension(const char *ext)
{
    struct BrainiacParser **parser;
    const char **exts;

    for (parser = parsers; parser; parser++) {
        for (exts = (*parser)->extensions; *exts; exts++) {
            if (!strcmp(*exts, ext)) {
                return *parser;
            }
        }
    }

    return NULL;
}
