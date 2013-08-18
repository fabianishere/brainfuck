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
	BrainfuckState *state = brainfuck_new_state(DEBUG); // 0 is debug disabled, anything else enables debug.
	brainfuck_execute(state, brainfuck_read_stream(fopen("examples/hello_world.bf", "r")));
	brainfuck_end_state(state);
	return EXIT_SUCCESS;
}
```
## License
See LICENSE file.

## Authors
    Fabian M. https://www.github.com/FabianM  mail.fabianm@gmail.com
    aliclubb https://www.github.com/aliclubb
