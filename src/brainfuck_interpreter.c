#include <stdio.h>
#include <stdarg.h>
#if defined __WIN32__ || defined _WIN32_ || defined _WIN32
#include <time.h>
#include <windows.h>
#endif
#if defined __APPLE__ || defined __unix__ || defined unix || defined _unix
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "brainfuck_interpreter.h"

#define MAX_CELLS 65536

void brainfuck_interpreter() {
		printf("Entering interactive mode...\n");
		#if defined __APPLE__ || defined __unix__ || defined unix || defined _unix
			fflush(stdout);
		#endif
		#if defined __WIN32__ || defined _WIN32_ || defined _WIN32
			Sleep(1);
		#endif
		#if !defined __WIN32__ || !defined _WIN32_ || !defined _WIN32
			sleep(1);
		#endif
		printf("Welcome to the Brainfuck Interpreter!");
		#if defined __APPLE__ || defined __unix__ || defined unix || defined _unix
		fflush(stdout);
		#endif
		for(;;)
		{
			printf("\nbrainfuck> ");
			char c;
			int pointer = 0;
			/* Put every character in character array */
			int size = 1048576;
			char chars[size];
			c = ' ';
			chars[pointer++] = (char) c;
			while ((c = getchar()) != '\n')
				chars[pointer++] = (char) c;
			/* Run the code */
			brainfuck_eval(chars);
		}
	}