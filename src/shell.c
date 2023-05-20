#include "shell.h"

static built_in_command built_ins[MAX_BUILT_IN_COUNT] = { 0 };
static int BUILT_INS_COUNT = 0;
static int COMMAND_MAX     = 10;
extern char **environ;

Command *alloc_cmd(int cap) 
{
	Command *cmd = malloc(sizeof(Command));	
	
	cmd->name    = (char*) malloc(cap);
	cmd->argv    = (char**) malloc(cap + 1);
	cmd->cap     = cap; 
	cmd->argc    = 0;	
	
	return cmd;
}

void free_cmd(Command *cmd) 
{
	free(cmd->argv);
	free(cmd->name);
	free(cmd);
}

Command **alloc_cmds_buffer(int cap) 
{
	
	int  it;
	Command **grid = (Command**) malloc(COMMAND_MAX * sizeof(Command *));

	for(it = 0; it < COMMAND_MAX; ++it)
	{
		grid[it]->name    = (char*) malloc(cap);
		grid[it]->argv    = (char**) malloc(cap + 1);
		grid[it]->cap     = cap; 
		grid[it]->argc    = 0;		
	}

	return grid;

}

void free_cmd_grid(Command **grid)
{
	
	int it;

	for(it = 0;grid[it] != NULL; ++it)
	{
		free_cmd(grid[it]);
	}

    free(grid);
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

Command **parse_commands(char *buff)
{
	char     *command_buff;
	int      size = 0;
	int      it = 0;
	int      j  = 0;
	Command  *command_cp;
	Command  **cmd    = alloc_cmds_buffer(COMMAND_MAX);
	char     **tokens = allocate_char_grid(COMMAND_MAX, BUFF_MAX);
	command_buff = strtok(buff, ";");
	
	while(command_buff != NULL) 
	{
		
		if(size + 1 == COMMAND_MAX) 
		{
			
			_fputs("[MEMORY ERROR] Command size reached the limit.\n", STDERR_FILENO);
			_fputs("[SIZE] ",                                          STDERR_FILENO);
			_fputi(size,                                               STDERR_FILENO);
			_fputs("\n",                                               STDERR_FILENO);

			return NULL;
		}
		

		_strcpy(tokens[size], command_buff);
		printf("Toked: %s\n", tokens[size]);
		size++;
		command_buff = strtok(NULL, ";");
	}

	tokens[size] = NULL;

	for(it = 0; it < size; it++) 
	{
		printf("Parsing..: %s\n", tokens[it]);
		
		command_cp = alloc_cmd(BUFF_MAX);
		
		if(parse_command(tokens[it], command_cp) == 0) 
		{
			_strcpy(cmd[it]->name, command_cp->name);
			
			for(j = 0; j < command_cp->argc; ++j)
			{
				_strcpy(cmd[it]->argv[j], command_cp->argv[j]);
			}
			
			cmd[it]->argc = j;
			printf("Parsed: %s\n", cmd[it]->name);
			free(command_cp);
			continue;
		}

		free(command_cp);
		_fputs("[PARSE ERROR] a command could not be parsed.\n", STDERR_FILENO);
		return NULL;
	}

	cmd[it] = NULL;
	free_char_grid(tokens, size);
	return cmd;
}

 

int parse_command(char *buff, Command *cmd) 
{
	char *cp = malloc(BUFF_MAX);
	char *Token;	
	int  len;
	_strcpy(cp, buff);
	Token = strtok(cp, " ");
	
	if(!buff) return 0;

    while(Token != NULL)
    {
    	len = _strlen(Token);
    	
    	Token[len] = '\0';
    	
    	if(cmd->argc == 0) {
    		_strcpy(cmd->name, Token);
    	}

    	_strcpy(cmd->argv[cmd->argc++], Token);
        
        if(cmd->argc == cmd->cap) 
        {
        	realloc_cmd(cmd); /* reallocate another.. */
        }

        Token = strtok(NULL, " ");
    }

    cmd->argv[cmd->argc] = NULL;
    
    free(cp);
    return 0;
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

	for(i = 0; i < size; ++i) 
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
	int code = 0;

	if(count > 1)
	{
		code = __atoi(args[1]);
		printf("Exiting with %s, %i\n", args[1], code);
	}

	exit(code);
}

void built_in_env(char **args, int count) {
	int i;
	
	if(count > 1)
	{
		_puts("env was called with: ");
		
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
	char *pwd  = getEnv("PWD");
	
	if(pwd == NULL) {
		_fputs("Could not get pwd env variable.\n", STDERR_FILENO);
		_fputs("Please check if it is set by the env command. if not then set it.\n", STDERR_FILENO);
		return;
	}

	char *prev = getEnv("OLDPWD");
	
	if(prev == NULL) {
		prev = pwd;
	}

	if(count == 1) {
		args[1] = "/";
	}
	
	if(_strcmp(args[1], "-")) {
		
		_puts(prev);
		_puts("\n");
		res = chdir(prev);
		
		if (res != 0)
		{
			_puts(args[1]);
			_putchar(' ');
			perror(":");
		}

		setEnv("PWD", prev);
		setEnv("OLDPWD", pwd);
		return;
	}

	setEnv("OLDPWD", pwd);
	
	res = chdir(args[1]);
	
	if (res != 0)
	{
		_puts(args[1]);
		_putchar(' ');
		perror(":");
		return;
	}

	setEnv("PWD", args[1]);
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
	int  i    = 0;
	int  size = 0;
	int  run  = 1;
	int  result = 0;
	char *buff;
	
	Command  **command_array;

    char **ENV_PATHS    = allocate_char_grid(BUFF_MAX, BUFF_MAX);
    int  ENV_PATHS_SIZE = get_tokenized_path(ENV_PATHS);

    reg_built_ins();

	while(run)  
	{
		size = 0;
		buff           = malloc(BUFF_MAX);
		prompt();

		size = read_command(buff, BUFF_MAX);
		
		if(size > 0) 
		{
				
			command_array = parse_commands(buff);
			
			if(command_array == NULL) {
				continue;
			}

			for (i = 0; i < size; ++i)
			{
				result = exec_builtin(command_array[i]);
				
				if(!result) 
				{
					
					if(find_cmd(command_array[i], ENV_PATHS, ENV_PATHS_SIZE) == 0)
					{
						commands_exec(command_array[i]);
						_puts("\n");
						continue;
					}

					_fputs("\n", STDERR_FILENO);
					_fputs("ERROR ", STDERR_FILENO);
					_fputs("[ ", STDERR_FILENO);
					_fputs(command_array[i]->name, STDERR_FILENO);
					_fputs(" ]\n", STDERR_FILENO);
					
					perror("");
					
					_fputs("\n", STDERR_FILENO);
				}	
			}
		}

		free_cmd_grid(command_array);
	}

	free_char_grid(ENV_PATHS, ENV_PATHS_SIZE);
	free_cmd_grid(command_array);
	free(buff);	
	return 0;
}