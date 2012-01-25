# Brainfuck 

------------

Lightweight, open-source Brainfuck interpreter written in C.

Notice
----------------
Written on a machine running Ubuntu (Linux). Not tested on other platforms yet.

Brainfuck
---------
Urban Müller created brainfuck in 1993 with the intention of designing a language which could be implemented with the smallest possible compiler.

### Commands
The eight language commands, each consisting of a single character:

| Character | Meaning                                                                                                                                                                           |
|:----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|     >     | increment the data pointer (to point to the next cell to the right).                                                                                                              |
|     <     | decrement the data pointer (to point to the next cell to the left).                                                                                                               |
|     +     | increment (increase by one) the byte at the data pointer.                                                                                                                         |
|     -     | decrement (decrease by one) the byte at the data pointer.                                                                                                                         |
|     .     | output a character, the ASCII value of which being the byte at the data pointer.                                                                                                  |
|     ,     | accept one byte of input, storing its value in the byte at the data pointer.                                                                                                      |
|     [     | if the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command*.|
|     ]     | if the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command*.|
