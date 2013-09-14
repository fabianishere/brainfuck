brainfuck
===========
Brainfuck interpreter written in C.

## Usage
    brainfuck [-ch] <filenames>
	-e  run code directly
	-h  show a help message.

The interactive console can be accessed by passing no arguments.    

We also provide a C api:

``` c
#include <stdio.h>
#include <brainfuck.h>
    
int main() {
	BrainfuckState *state = brainfuck_state();
	BrainfuckExecutionContext *context = brainfuck_context(BRAINFUCK_TAPE_SIZE);
	BrainfuckInstruction *instruction = brainfuck_parse_string("+++++.");
 	brainfuck_add(state, instruction);
 	brainfuck_execute(state->root, context);
	brainfuck_destroy_context(context);
 	brainfuck_destroy_state(state);
	return EXIT_SUCCESS;
}
```
## License
See LICENSE file.

## Contributors
    Fabian M. https://www.github.com/FabianM  mail.fabianm@gmail.com
    aliclubb https://www.github.com/aliclubb
