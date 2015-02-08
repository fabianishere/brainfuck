brainfuck 3
===============
Brainfuck interpreter written in C89.

## Getting the source
Download the source code by running the following code in your command prompt:
```sh
$ git clone -b develop https://github.com/FabianM/brainfuck.git
```
or simply [grab](https://github.com/FabianM/brainfuck/archive/develop.zip) a copy of the source code as a Zip file.

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

## License
The code is released under the MIT license. See the LICENSE file.

