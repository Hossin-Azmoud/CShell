#include "command.h"

extern char **environ;


Command *alloc_cmd(int cap) {
	
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

        if(cmd->size == cmd->cap){
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
	
	if(size == 0) return 1;
	
	if(access(c->name, X_OK) != -1) {
		return 0;
	}

	for (i = 0; i < size; ++i) 
	{
		char *copy = malloc(_strlen(paths[i]) + 1);
		
		_strcpy(copy, paths[i]);
		join_path(copy, c->name);
		
		if(access(copy , X_OK) != -1) {
			c->name = copy;
			return 0;
		}
	}

	return 1; /* Not found */
}

#if 0

Command *read_command() {
	
	int c, size, token_index;
	char buff[BUFF_MAX];

	Command *command = alloc_cmd(32);
	
	while((c = getchar()) != '\n' && c != EOF)
	{
		assert(BUFF_MAX > size && "BUFF LIMIT SURPASSED.");
		
		if(c == ' ') {
			/* collect the component of the command. */
			
			if(size > 0) {
				buff[size++] = '\0';
				_strcpy(command->args[token_index], buff);
				
				if(token_index == 0) {
					_strcpy(command->name, buff);
				}

				token_index++;
				size = 0;
			}
		}

		buff[size++] = (char) c;
		/* TODO: Check if the buff is full and reallocate mem */
	}

	command->size = token_index;
	print_command(command);
	return command;
}

Command *read_command() {
	
	int c, size, token_index = 0;
	char buff[BUFF_MAX];

	Command *command = alloc_cmd(32);
	_puts("test-shell$: ");
	
	while((c = getchar()) != '\n' && c != EOF)
	{
		assert(BUFF_MAX > size && "BUFF LIMIT SURPASSED.");
		buff[size++] = (char) c;
		/* TODO: Check if the buff is full and reallocate mem */
	}

	if(c == ' ')
	{
		/* collect the component of the command. */
		if(size > 0) {
			
			buff[size++] = '\0';

			_strcpy(command->args[token_index], buff);
			
			if(token_index == 0) {
				_strcpy(command->name, buff);
			}

			token_index++;
			size = 0;
		}
	}


	command->size = token_index;
	
	print_command(command);
	
	return command;
}

static int recv_eof = 0;
static int finished = 0;
static void handle_signal(int sig){
	
	if(sig == SIGQUIT)
	{
		recv_eof = 1;
		finished = 1;
		exit(0);
	}
}
0616981090
#endif
