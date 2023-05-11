
#ifndef _COMMAND_H
#define _COMMAND_H

#include <string.h>
#include <sys/wait.h>
#include "util.h"



typedef struct {
	char *name;
	char **args;
	int   cap;
	int   size;
} Command;

char      *command_read(Command *cmd);
int       command_write(Command *cmd);

void      commands_exec(Command *cmd);

void      parse_cmd(char *chunk, Command *cmd);

Command   *alloc_cmd(int cap);
void      print_command(Command *c);

#endif /* _COMMAND_H */