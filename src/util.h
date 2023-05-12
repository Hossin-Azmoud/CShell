#ifndef __UTIL__H
#define __UTIL__H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#define BUFF_MAX 128

int   _putchar(char c);
int   _strlen(char *s);
int   _puts(char *s);
char  *_strcpy(char *dst, char *src);
char  *_strcat(char *dst, char *src);
int   _strcmp(char *s1, char *s2);
int   read_command(char *buff, FILE *Stream, int cap);
void  prompt();
char  *getpath();
int   get_tokenized_path(char **paths);
char  *join_path(char *dst, char *src);
char  **allocate_char_grid(int row, int col);
void  free_char_grid(char **grid, int row);
void  print_env(char *envp[]);

#endif /* __UTIL__H */




