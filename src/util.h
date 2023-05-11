#ifndef __UTIL__H
#define __UTIL__H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#define BUFF_MAX 100

int   _putchar(char c);
int   _strlen(char *s);
int   _puts(char *s);
char  *_strcpy(char *dst, char *src);
int   _strcmp(char *s1, char *s2);
int   read_command(char *buff);
void  prompt();

#endif /* __UTIL__H */




