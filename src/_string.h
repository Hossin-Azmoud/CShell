#ifndef __STRINGH__H
#define __STRINGH__H

#include "_std.h"

int   _strlen(char *s);
char  *_strcpy(char *dst, char *src);
char  *_strcat(char *dst, char *src);
int   _strcmp(char *s1, char *s2);
char *join(char *dst, char *src, char *delim);

#endif /* __STRINGH__H */