#include "command.h"

void test();
int shell();

int main() {
	shell();

	return 0;
}

void test() {
	char buff[BUFF_MAX];
	Command *cmd = alloc_cmd(BUFF_MAX);
	read_command(buff);
	printf("read command: %s\n", buff);
	parse_cmd(buff, cmd);
	print_command(cmd);
}

int shell() {
	int size = 0;
	int run = 1;
	char     *buff  = malloc(BUFF_MAX);
	Command  *cmd   = alloc_cmd(BUFF_MAX);
	
	while(run) 
	{
		/*
			TODO: make Reading func that returns a command. with its args.
			TODO: make built-in executation command.
			TODO: make exec command.
		*/
		prompt();
		size = read_command(buff);
		
		if(size > 1) {
			
			parse_cmd(buff, cmd);
			
			if(_strcmp(cmd->name, "exit"))
			{
				_puts("exiting..\n");
				run  = 0;
				continue;
			}
			
			commands_exec(cmd);
			
			size = 0;
			continue;
		}

		size = 0;
	}

	return 0;
}
