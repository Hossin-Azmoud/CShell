#ifndef __STD__H
#define __STD__H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include "util.h"

#define BUFF_MAX 128

enum Key {
	End_key = 23,
	Left_key = 37,
	Right_key = 39,
	Backspace = 8
};

int  _fputchar(char c, int Stream);
int  _fputs(char *s, int Stream);
int  _puts(char *s);
int  _putchar(char c);
char **allocate_char_grid(int row, int col);
void free_char_grid(char **grid, int row);
char *_realloc(char *ptr, int size);
int  read_command(char *buff, int cap);

#endif /* __STD__H */
