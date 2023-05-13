
#ifndef _COMMAND_H
#define _COMMAND_H

#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "util.h"
#define BUILT_INS_COUNT 3

typedef struct Command {
	char *name;
	char **args;
	int   cap;
	int   size;
} Command;

typedef struct built_in_command {
	char *name;
	void (*func)(Command *);
} built_in_command;

void    built_in_exit(Command *cmd);
void    built_in_env(Command *cmd);
void    built_in_cd(Command *cmd);
void    commands_exec(Command *cmd);
void    parse_cmd(char *chunk, Command *cmd);
Command *alloc_cmd(int cap);
void    print_command(Command *c);
int     find_cmd(Command *c, char **paths, int size);
void    free_command(Command *c);
int     exec_builtin(Command *cmd);

#endif /* _COMMAND_H */