
#ifndef _COMMAND_H
#define _COMMAND_H

#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "util.h"

#define MAX_BUILT_IN_COUNT 16
#define MAX_ARGS           20
#define MAX_PATH           512

#define CLEAR_BYTES "\033[2J"

#define SEMI_COLON  ";"
#define ROOT        "/"
#define AND_SYM     '&'
#define OR_SYM      '|'
#define JOIN_SYM    ';'

typedef enum EContext {
	JOIN, /* ; */	
	AND, /* && */	
	OR, /* || */	
	NONE,
	CONTEXT_COUNT
} EContext;

typedef struct Command {
	char *name;
	char **argv;
	int   argc;
	int   cap;
} Command;

typedef struct built_in_command {
	char  name[16];
	void (*func)(char **, int);
} built_in_command;

void construct_built_in(char *name, void (*func)(char **, int), built_in_command *ptr);
void built_in_exit(char **args, int count);
void built_in_env(char **args, int count);
void built_in_cd(char **args, int count);
int  _exec(Command *cmd);

int  parse_commands(char *buff, EContext *ctx, Command **cmd);
int  parse_command(char *buff, Command *cmd);


Command  *alloc_cmd(int cap);
void     free_cmd(Command *cmd);
Command  **alloc_cmds_buffer(int cap);
void     print_command(Command *c);
int      find_cmd(Command *c, char **paths, int size);
void     free_command(Command *c);
void     free_cmd_grid(Command **grid);
int      exec_builtin(Command *cmd);
void     reg_built_ins();
void     execute_joined(Command **command_array, int size);
void     execute_or(Command **command_array, int size);
void     execute_and(Command **command_array, int size);
int      execute_command(Command *command_array);
void     init();
void     deinit();
void     unreg_built_ins();
int      shell();

#endif /* _COMMAND_H */