#include <stdio.h>
#include <stdlib.h>
#include <brainfuck.h>

/**
 * Smoke test running a simple "Hello World" brainfuck program.
 */
int main() {
    BrainfuckState *state = brainfuck_state();
    BrainfuckExecutionContext *context = brainfuck_context(BRAINFUCK_TAPE_SIZE);
    BrainfuckInstruction *instruction = brainfuck_parse_string("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.++"
                                                               "+.------.--------.>>+.>++.");
    brainfuck_add(state, instruction);
    brainfuck_execute(state->root, context);
    brainfuck_destroy_context(context);
    brainfuck_destroy_state(state);
    return EXIT_SUCCESS;
}