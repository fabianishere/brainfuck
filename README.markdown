brainfuck 3
===============
Brainfuck interpreter written in C89.

## Getting the source
Download the source code by running the following code in your command prompt:
```sh
$ git clone -b develop https://github.com/fabianm/brainfuck.git
```
or simply [grab](https://github.com/fabianm/brainfuck/archive/develop.zip) a copy of the source code as a Zip file.

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
	-h --help			show a help message
	-m --memory	<int>		the size of the memory block to allocate for the program (default: 30000 elements)
	-e --eval	<string>	run code directly
```
An Application Programming Interface is also provided:

```c
#include <brainfuck.h>

int main(void)
{
	int memory[30000] = {0};
	int error = BRAINFUCK_EOK;
	struct BrainfuckContext ctx;
	struct BrainfuckScript *script = brainfuck_parse_string("+++++++++[>+++++++++++<-].", &error);
	brainfuck_execution_context_init(&ctx, getchar, putchar,
		sizeof(int) * 30000, memory, 0);
	brainfuck_execution_interpret(script);
	brainfuck_script_free(script)
}
```

## License
The code is released under the MIT license. See the LICENSE file.

