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

## Building from source
`gcc` and `make` are required to build this brainfuck interpreter. Build on Windows using MinGW.  

Get the source code: 

    git clone https://github.com/FabianM/brainfuck.git
    
Go into the new directory:

    cd brainfuck
    
Build the brainfuck interpreter:

    make
