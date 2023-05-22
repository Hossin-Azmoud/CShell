#ifndef __STRINGH__H
#define __STRINGH__H

#include "_std.h"
#include <ctype.h>

int   _strlen(char *s);
char  *_strcpy(char *dst, char *src);
char  *_strcat(char *dst, char *src);
int   _strcmp(char *s1, char *s2);
char *join(char *dst, char *src, char *delim);

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);

#endif /* __STRINGH__H */