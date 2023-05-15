#include "command.h"
#include <assert.h>

void test();
int shell();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {
	shell();
	return 0;
}

void test() 
{
	char *my;
	char *path = getEnv("USER");
	printf("%s\n", path);
	setEnv("USER", "New_User");
	my = getEnv("USER");
	printf("%s\n", my);
}

int shell() {
	
	int size = 0;
	int run = 1;
	int result = 0;
	char     *buff;
	Command  *cmd;
    
    char **ENV_PATHS    = allocate_char_grid(BUFF_MAX, BUFF_MAX);
    int  ENV_PATHS_SIZE = get_tokenized_path(ENV_PATHS);

    reg_built_ins();

	while(run) 
	{
		size = 0;
		cmd  = alloc_cmd(BUFF_MAX);
		buff = malloc(BUFF_MAX);
		prompt();
		size = read_command(buff, BUFF_MAX);
		
		if(size > 0) {
				
			parse_cmd(buff, cmd);
			result = exec_builtin(cmd);
			
			if(!result) 
			{
				
				if(find_cmd(cmd, ENV_PATHS, ENV_PATHS_SIZE) == 0) 
				{

					commands_exec(cmd);
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
		}
	}

	free_char_grid(ENV_PATHS, ENV_PATHS_SIZE);
	free(cmd);
	free(buff);
	
	return 0;
}
