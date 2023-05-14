#include "command.h"

static built_in_command built_ins[BUILT_INS_COUNT] = { 0 };
extern char **environ;

Command *alloc_cmd(int cap) 
{
	Command *cmd = malloc(sizeof(Command));	
	cmd->name    = (char*) malloc(cap);
	cmd->args    = (char**) malloc(cap);
	cmd->cap     = cap; 
	cmd->size    = 0;	
	return cmd;
}
void realloc_cmd(Command *cmd)
{
	Command *tmp = alloc_cmd(cmd->cap + BUFF_MAX);
	int i;
	tmp->name = cmd->name;
	
	for (i = 0; i < cmd->size; ++i)
	{
		tmp->args[tmp->size++] = cmd->args[i];
	}

	*cmd = *tmp; /* Move tmp to cmd */
}

void parse_cmd(char *buff, Command *cmd) {
	
	char    *Token    = strtok(buff, " ");
	
	if(!buff) return;
    
    while(Token != NULL)
    {
    	if(cmd->size == 0) 
    	{
    		cmd->name = Token;
    	}

    	cmd->args[cmd->size++] = Token;
        
        if(cmd->size == cmd->cap) 
        {
        	realloc_cmd(cmd); /* reallocate another.. */
        }

        Token = strtok(NULL, " ");
    }
}

void print_command(Command *c) {
	
	int i;
	_puts("--------PARSED COMMAND---------");
	_puts("name: ");
	_puts(c->name);
	_puts("\n");
	
	for(i = 0; i < c->size; i++) {
		_puts(c->args[i]);
		_puts(" ");
	}
	
	_puts("\n");

}

void commands_exec(Command *cmd) {

	int pid, code = 0;
	
	/* signal(SIGQUIT, handle_signal); Sync */
	
	pid = fork();
	if(pid == 0)
	{
		/* Child Process. */
		
		_putchar('\n');
		code = execve(cmd->name, cmd->args, environ);
	
		if(code == -1) 
		{
			_puts(cmd->name);
			perror(": ");
			_putchar('\n');
			exit(0);
		}

		_putchar('\n');
		exit(0);
	}
	
	wait(NULL);
}

int find_cmd(Command *c, char **paths, int size) {
	int i;
	int res;
	char *copy;
	

	if(size == 0) return 1;
	
	res = access(c->name, X_OK); 
	
	if(res != -1) {
		return 0;
	}

	for (i = 0; i < size; ++i) 
	{
		copy = malloc(_strlen(paths[i]) + 1);
		_strcpy(copy, paths[i]);
		join_path(copy, c->name);
		
		res = access(copy, X_OK);
		
		if(res != -1)
		{
			_strcpy(c->name, copy);
			return 0;
		}
	}

	return 1; /* Not found */
}


void built_in_exit(char **args, int count) {
	int i;

	if(count > 1)
	{
			
		_puts("EXIT WAS CALLED WITH: ");
		for(i = 0; i < count; i++) {
			_puts(args[i]);
			_puts("\n");
		}
	}

	exit(0);
}

void built_in_env(char **args, int count) {
	int i;
	
	if(count > 1)
	{
		_puts("ENV WAS CALLED WITH: ");
		
		for(i = 0; i < count; i++) {
			_puts(args[i]);
			_puts("\n");
		}
	}

	while(*environ){
		_puts(*environ++);
		_putchar('\n');
	}

}

void built_in_cd(char **args, int count) {
	int res;
	printf("Hello");
	
	if(count > 1)
	{
		_puts("CD WAS CALLED WITH: ");
		_puts(" args\n");
	}

	res = chdir(args[1]);
	
	if (res != 0)
	{
		_puts(args[1]);
		_putchar(' ');
		perror(":");
	}
}

int exec_builtin(Command *cmd) {
	
	int i = 0;	
	
	for(i = 0; i < BUILT_INS_COUNT; ++i)
	{
		built_in_command command = built_ins[i];
		
		if(_strcmp(command.name, cmd->name)) {
			command.func(cmd->args, cmd->size);
			return 1;
		}
	}

	return 0;
}

built_in_command construct_built_in(char *name, void (*func)(char **, int)) {
	
	built_in_command *command = malloc(sizeof(built_in_command));
	
	command->name = name;
	command->func = func;

	return *command;
}

void reg_built_ins() {
	built_ins[0] = construct_built_in("exit", built_in_exit);
	_puts("\n[0][REG] exit\n");
	
	built_ins[1] = construct_built_in("env", built_in_env);
	_puts("[1][REG] env\n");
	
	built_ins[2] = construct_built_in("cd", built_in_cd);
	_puts("[2][REG] cd\n\n");
}