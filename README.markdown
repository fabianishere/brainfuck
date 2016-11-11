brainfuck 3
===============
Brainfuck interpreter written in C89.

## Getting the source
Download the source code by running the following code in your command prompt:
```sh
$ git clone -b develop https://github.com/fabianishere/brainfuck.git
```
or simply [grab](https://github.com/fabianishere/brainfuck/archive/develop.zip) a copy of the source code as a Zip file.

## Building
Create the build directory.
```sh
$ mkdir build
$ cd build
```
Brainfuck requires CMake and a C compiler (e.g. Clang or GCC) in order to run.
Then, simply create the Makefiles:
```sh
$ cmake ..
```
and finally, build it using the building system you chose (e.g. Make):
```sh
$ make
```

## Usage
```sh
usage: brainfuck [-vhme] file...
	-v --version			show the version information
	-h --help				show a help message
	-m --memory	<int>		the size of the memory block to allocate for the program (default: 30000 elements)
	-e --eval	<string>	run code directly
```
An Application Programming Interface is also provided:

```c
#include <stdio.h>
#include <brainfuck/brainfuck.h>
#include <brainfuck/parser.h>
#include <brainfuck/engine.h>

int main(void)
{
	/* Declaration */
	struct BrainfuckContext ctx;
	struct BrainfuckScript *script;
	
	/* Initialization */
	ctx.read = &getchar;
	ctx.write = &putchar;
	ctx.mem_size = sizeof(int) * 30000;
	ctx.memory = malloc(sizeof(int) * 30000);
	
	/* Parsing */
	script = brainfuck_parser_parse_string("+++++++++[>+++++++++++<-].", NULL);
	
	/* Execution */
	brainfuck_engine_free(script);
	
	/* Cleaning */
	free(ctx.memory);
	brainfuck_script_free(script);
}
```

## License
The code is released under the MIT license. See the LICENSE file.

