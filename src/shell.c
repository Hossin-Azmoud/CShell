#include "shell.h"

static built_in_command built_ins[MAX_BUILT_IN_COUNT] = { 0 };
static int BUILT_INS_COUNT = 0;
extern char **environ;

Command *alloc_cmd(int cap) 
{
	Command *cmd = malloc(sizeof(Command));	
	
	cmd->name    = (char*) malloc(cap);
	cmd->argv    = (char**) malloc(cap) + 1;
	cmd->cap     = cap; 
	cmd->argc    = 0;	
	
	return cmd;
}
void realloc_cmd(Command *cmd)
{
	Command *tmp = alloc_cmd(cmd->cap + BUFF_MAX);
	
	int i;
	
	tmp->name = cmd->name;
	
	for (i = 0; i < cmd->argc; ++i)
	{
		tmp->argv[tmp->argc++] = cmd->argv[i];
	}

	*cmd = *tmp; /* Move tmp to cmd */
}

void parse_cmd(char *buff, Command *cmd) {
	
	char    *Token    = strtok(buff, " ");
	if(!buff) return;
    
    while(Token != NULL)
    {
    	if(cmd->argc == 0) 
    	{
    		cmd->name = Token;
    		
    	}

    	cmd->argv[cmd->argc++] = Token;
        
        if(cmd->argc == cmd->cap) 
        {
        	realloc_cmd(cmd); /* reallocate another.. */
        }

        Token = strtok(NULL, " ");
    }

    cmd->argv[cmd->argc] = NULL;

}

void print_command(Command *c) {
	
	int i;
	
	_puts("--------PARSED COMMAND---------\n");
	_puts("name: ");
	_puts(c->name);
	_puts("\n");
	
	for(i = 1; i < c->argc; i++) {
		_puts(c->argv[i]);
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
		code = execve(cmd->name, cmd->argv, environ);
		printf("Exex\n");
		if(code == -1) 
		{
			print_command(cmd);
			
			perror("");
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
		copy = malloc(_strlen(paths[i]) + _strlen(c->name) + 1);
		_strcpy(copy, paths[i]);		
		copy = join_path(copy, c->name);		
		res = access(copy, X_OK);
		
		if(res != -1)
		{
			c->name = copy;
			return 0;
		}

		free(copy);
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
	
	for(i = 0; environ[i]; i++) {
		_puts(environ[i]);
		_putchar('\n');	
	}
}

void built_in_clear(char **args, int count) {
	int i;
	
	if(count > 1)
	{
		_puts("clear WAS CALLED WITH: ");
		
		for(i = 0; i < count; i++) {
			_puts(args[i]);
			_puts("\n");
		}
	}

	_puts(CLEAR_BYTES);
	_puts("\033[0;0H\n"); /* Go to the starting col and row of the term. */
}

void built_in_cd(char **args, int count) {
	int res;
	
	if(count == 1) {
		args[1] = "/";
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
			command.func(cmd->argv, cmd->argc);
			return 1;
		}
	}

	return 0;
}

built_in_command construct_built_in(char *name, void (*func)(char **, int)) {
	
	built_in_command *command = malloc(sizeof(built_in_command));
	
	command->name = name;
	command->func = func;
	
	BUILT_INS_COUNT++;
	
	return *command;
}

void reg_built_ins() {
	
	built_ins[0] = construct_built_in("exit", built_in_exit);
	_puts("\n[0][REG] exit\n");
	
	built_ins[1] = construct_built_in("env", built_in_env);
	_puts("[1][REG] env\n");
	
	built_ins[2] = construct_built_in("cd", built_in_cd);
	_puts("[2][REG] cd\n");
	
	built_ins[3] = construct_built_in("clear", built_in_clear);
	_puts("[3][REG] clear\n\n");
	
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

				_fputs("\n", STDERR_FILENO);
				_fputs("ERROR ", STDERR_FILENO);
				_fputs("[ ", STDERR_FILENO);
				_fputs(cmd->name, STDERR_FILENO);
				_fputs(" ]\n", STDERR_FILENO);
				
				perror("");
				
				_fputs("\n", STDERR_FILENO);
			}
		}
	}

	free_char_grid(ENV_PATHS, ENV_PATHS_SIZE);
	free(cmd);
	free(buff);
	
	return 0;
}
