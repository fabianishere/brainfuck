brainfuck 3
===============
Brainfuck interpreter written in C. Currently slower than the latest version of
	the master branch.

## Getting the source
Download the source code by running the following code in your command prompt:
```sh
$ git clone https://github.com/FabianM/brainfuck.git
```
or simply [grab](https://github.com/FabianM/brainfuck/archive/master.zip) a copy of the source code as a Zip file.

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
See LICENSE file.

## Contributors
    Fabian M. https://github.com/fabianm mail.fabianm@gmail.com
    aliclubb https://github.com/aliclubb
    diekmann https://github.com/diekmann
    SevenBits https://github.com/SevenBits
    Alex Burka https://github.com/durka durka42@gmail.com
