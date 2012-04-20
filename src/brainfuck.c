#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CELLS 65536

void brainfuck_eval(char chars[]) {
	char c;
	int pointer = 0;
	int char_pointer = 0;
	int loop = 0;
	int tape[MAX_CELLS];
	/* Loop through all character in the character array */
	while (char_pointer++ < strlen(chars)) {
		switch (chars[char_pointer]) {
		case '>':
			pointer++;
			break;
		case '<':
			pointer--;
			break;
		case '+':
			tape[pointer]++;
			break;
		case '-':
			tape[pointer]--;
			break;
		case '.':
			putchar(tape[pointer]);
			break;
		case ',':
			tape[pointer] = (int) getchar();
			break;
		case '[':
			if (tape[pointer] == 0) {
				loop = 1;
				while (loop > 0) {
					c = chars[++char_pointer];
					if (c == '[')
						loop++;
					else if (c == ']')
						loop--;
				}
			}
			break;
		case ']':
			loop = 1;
			while (loop > 0) {
				c = chars[--char_pointer];
				if (c == '[')
					loop--;
				else if (c == ']')
					loop++;
			}
			char_pointer--;
			break;
		}
	}
}
