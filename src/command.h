
#ifndef _COMMAND_H
#define _COMMAND_H

#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "util.h"

typedef struct {
	char *name;
	char **args;
	int   cap;
	int   size;
} Command;

void    commands_exec(Command *cmd);
void    parse_cmd(char *chunk, Command *cmd);
Command *alloc_cmd(int cap);
void    print_command(Command *c);
int     find_cmd(Command *c, char **paths, int size);
void    free_command(Command *c);
#endif /* _COMMAND_H */