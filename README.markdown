brainfuck
===========
Brainfuck interpreter written in C.

## Usage
    brainfuck [-fcih] <filename>
	-f  run the given file
	-e  run code directly
	-i  open the interactive console.
	-h  show a help message.
	
or from C:

    #include <brainfuck.h>
    
    main() {
    	BrainfuckState *state = brainfuck_new_state(SIZE, DEBUG); // 0 is debug disabled
    	brainfuck_put_token_string(state, "+++.");
    	brainfuck_run(state);
    	brainfuck_end_state(state);
    }


## License
See LICENSE file.

## Authors
    Fabian M. https://www.github.com/FabianM  mail.fabianm@gmail.com
    aliclubb https://www.github.com/aliclubb
    
## Tested
Currently tested on:

* Ubuntu 12.04

