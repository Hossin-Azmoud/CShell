#include "command.h"

char *const environ[] = { 
	NULL 
};

Command *alloc_cmd(int cap) {
	Command *cmd = malloc(sizeof(Command));
	
	cmd->args    = (char**) malloc(cap);
	cmd->cap     = cap; 
	cmd->size    = 0;

	return cmd;
}


void parse_cmd(char *buff, Command *cmd) {
	
	char    *Token    = strtok(buff, " ");

	if(!buff) return;

    while(Token != NULL)
    {
    	if(cmd->size == 0) 
    	{
    		cmd->name  = Token;		
    	}
    	
    	cmd->args[cmd->size++] = Token;
        /* Next.. */
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
	pid = fork();

	if(pid == 0) 
	{
		code = execve(cmd->name, cmd->args, environ);
		
		if(code == -1) {
			_puts("An err accured while executing :");
			_puts(cmd->name);
			_putchar('\n');
			exit(code);
		}

		exit(0);
	}
	
	wait(NULL);
	cmd->size = 0;
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


	command->size = token_index;
	
	print_command(command);
	
	return command;
}
#endif