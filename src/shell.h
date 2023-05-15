
#ifndef _COMMAND_H
#define _COMMAND_H

#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "util.h"

#define MAX_BUILT_IN_COUNT 16
#define CLEAR_BYTES        "\033[2J"

typedef struct Command {
	char *name;
	char **argv;
	int   argc;
	int   cap;
} Command;

typedef struct built_in_command {
	char *name;
	void (*func)(char **, int);
} built_in_command;


built_in_command construct_built_in(char *name, void (*func)(char **, int));

void     built_in_exit(char **args, int count);
void     built_in_env(char **args, int count);
void     built_in_cd(char **args, int count);
void     commands_exec(Command *cmd);
void     parse_cmd(char *chunk, Command *cmd);
Command  *alloc_cmd(int cap);
void     print_command(Command *c);
int      find_cmd(Command *c, char **paths, int size);
void     free_command(Command *c);
int      exec_builtin(Command *cmd);
void     reg_built_ins();
int      shell();

#endif /* _COMMAND_H */