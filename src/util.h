#ifndef __UTIL__H
#define __UTIL__H

#include "_string.h"

#define BUG(ID) printf("BUG %s\n", ID);

void prompt();
char *getpath();
int  get_tokenized_path(char **paths);
char *join_path(char *dst, char *src);

#endif /* __UTIL__H */




