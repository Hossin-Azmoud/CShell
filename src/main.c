#include "command.h"

void test();
int shell();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[], char *envp[]) {
	printf("Started\n");
	shell(envp);
	return 0;
}

void test() {
	int i;
	char *tmp;
	char **xs = allocate_char_grid(BUFF_MAX, BUFF_MAX);
	int  size = get_tokenized_path(xs);

	for(i = 0; i < size; ++i)
	{
		tmp = xs[i];
		
		printf("before: %s => %i\n", tmp, _strlen(tmp));	
		
		join_path(tmp, "hello");
		
		printf("after: %s => %i\n", xs[i], _strlen(xs[i]));
	}

	free_char_grid(xs, BUFF_MAX);
}

int shell(char *envp[]) {
	
	int size = 0;
	int run = 1;
	char     *buff;
	Command  *cmd;
	
    char **ENV_PATHS    = allocate_char_grid(BUFF_MAX, BUFF_MAX);
    int  ENV_PATHS_SIZE = get_tokenized_path(ENV_PATHS);

	while(run) 
	{
		cmd  = alloc_cmd(BUFF_MAX);
		buff = malloc(BUFF_MAX);
		
		prompt();

		size = read_command(buff, stdin, BUFF_MAX);
		
		if(size > 1) {
				
			parse_cmd(buff, cmd);
			
			if(_strcmp(cmd->name, "exit")) {
				_puts("exiting..\n");
				run  = 0;
				continue;
			}

			if(_strcmp(cmd->name, "env"))
			{
				print_env(envp);
				continue;
			}

			if(find_cmd(cmd, ENV_PATHS, ENV_PATHS_SIZE) == 0) 
			{
				commands_exec(cmd, envp);
				_puts("\n");
				continue;
			}
			
			_puts("\n");
			_puts("ERROR ");
			_puts("[ ");
			_puts(cmd->name);
			_puts(" ]\n");
			perror("");
			_puts("\n");
		}

		size = 0;
	}

	free_char_grid(ENV_PATHS, ENV_PATHS_SIZE);
	free(cmd);
	free(buff);
	
	return 0;
}
