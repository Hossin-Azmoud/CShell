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
#define SEQ_START_BYTE '\x1b'
#define SEQ_SEC_BYTE '['
#define UP_KEY 65
#define DOWN_KEY 66
#define RIGHT_KEY 67
#define LEFT_KEY 68
#define END_KEY 70
#define HOME_KEY 72

typedef struct Position
{	
	int x;
	int y;
};

/* printf("\033[6;3HHello\n"); */

int   _fputchar(char c, int Stream);
int   _fputs(char *s, int Stream);
int   _puts(char *s);
int   _putchar(char c);
char  **allocate_char_grid(int row, int col);
void  free_char_grid(char **grid, int row);
char  *_realloc(char *ptr, int size);
int   read_command(char *buff, int cap);
char  *_iota(int i);
int   _atoi(char *buff);

int  move_cursor();

#endif /* __STD__H */
