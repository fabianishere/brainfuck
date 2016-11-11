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
 * Optimization and debugging passes for the brainfuck Intermediate Representation (IR).
 */
/* Dependencies */
#include <stdio.h>

#include <brainfuck/brainfuck.h>
#include <brainfuck/parser.h>
#include <brainfuck/passes/clear.h>
#include <brainfuck/passes/dump.h>

#define BRAINFUCK_PASS_DUMP_AVAILABLE 1
#define BRAINFUCK_PASS_CLEAR_AVAILABLE 1

const int brainfuck_pass_clear_available = 1;
const int brainfuck_pass_dump_available = 1;

void brainfuck_pass_dump(const struct BrainfuckScript *script, FILE *file)
{
	struct BrainfuckInstruction* inst = script->head;
	while (inst) {
		switch(inst->opcode) {
		case ADD:
			fprintf(file, "%p: add $%d, %d\n", inst, inst->x.offset, inst->y.offset);
			break;
		case SUB:
			fprintf(file, "%p: sub $%d, %d\n", inst, inst->x.offset, inst->y.offset);
			break;
		case MOVL:
			fprintf(file, "%p: movl %d\n", inst, inst->x.offset);
			break;
		case MOVR:
			fprintf(file, "%p: movr %d\n", inst, inst->x.offset);
			break;
		case CLR:
			fprintf(file, "%p: clr %d\n", inst, inst->x.offset);
			break;
		case CPY:
			fprintf(file, "%p: cpy %d, %d\n", inst, inst->x.offset, inst->y.offset);
			break;
		case MUL:
			fprintf(file, "%p: mul %d, %d, $%d\n", inst, inst->x.offset, inst->y.offset, inst->z.multiplier);
			break;
		case IN:
			fprintf(file, "%p: in %d\n", inst, inst->x.offset);
			break;
		case OUT:
			fprintf(file, "%p: out %d\n", inst, inst->x.offset);
			break;
		case JNZ:
			fprintf(file, "%p: jnz %p\n", inst, inst->x.target);
			break;
		case JMP:
			fprintf(file, "%p: jmp %p\n", inst, inst->x.target);
			break;
		case NOP:
			fprintf(file, "%p: nop\n", inst);
			break;
		}
		inst = inst->next;
	}
	fprintf(file, "\n");
}

int brainfuck_pass_clear(const struct BrainfuckScript *script)
{
	struct BrainfuckInstruction *inst = script->head;
	struct BrainfuckInstruction *a;
	struct BrainfuckInstruction *b;
	
	while(inst) {
		if (inst->opcode != JMP)
			goto next;
		a = inst->next;
		if (!a || !(a->opcode == ADD || a->opcode == SUB))
			goto next;
		b = a->next;
		if (!b || !(b->opcode == JNZ))
			goto next;
		inst->opcode = CLR;
		inst->x.offset = 0;
		inst->next = b->next;
		brainfuck_instruction_dealloc(a);
		brainfuck_instruction_dealloc(b);
		next:
			inst = inst->next;
	}
	return BRAINFUCK_EOK;
}