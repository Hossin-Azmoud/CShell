#ifndef _COMMAND_H
#define _COMMAND_H

typedef struct {
	char *content;
	size_t cap;
	size_t size;
} Command;

char *command_read(Command *cmd); 
int  command_write(Command *cmd);
int  commands_exec(Command *cmd);

#endif // _COMMAND_H
