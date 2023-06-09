#include "shell.h"

static built_in_command built_ins[MAX_BUILT_IN_COUNT] = { 0 };
static int BUILT_INS_COUNT                            = 0;
static int COMMAND_MAX                                = 20;
const  char ctx_array[CONTEXT_COUNT] = { ';', '&', '|' };
extern char **environ;
static char **ENV_PATHS    = { 0 };
static int  ENV_PATHS_SIZE = 0;
static int  RUN_FLAG       = 1;


Command *alloc_cmd(int cap)
{
	int i;
	Command *cmd = malloc(sizeof(Command));
	
	cmd->name    = (char*) malloc(cap);
	cmd->argv = (char**) malloc((MAX_ARGS + 1) * sizeof(char*));
	
	for(i = 0; i < MAX_ARGS; i++ ) 
	{
		cmd->argv[i] = (char*)malloc(cap + 1);
	}

	cmd->cap     = cap;
	cmd->argc    = 0;
	
	return cmd;
}

void free_cmd(Command *cmd)
{
	int i;

	for(i = 0; i < MAX_ARGS; i++ )
	{
	  free(cmd->argv[i]);
	}

	free(cmd->argv);
	free(cmd->name);
	
	free(cmd);
}

Command  **alloc_cmds_buffer(int cap) 
{
	
	int  it, i;
	Command **grid = (Command**) malloc(COMMAND_MAX * sizeof(Command *));

	for(it = 0; it < COMMAND_MAX; ++it)
	{
		grid[it]          = malloc(sizeof(Command));
		grid[it]->name    = (char*) malloc(cap);
		
		grid[it]->argv = (char**) malloc((MAX_ARGS + 1) * sizeof(char*));

		for( i=0; i < MAX_ARGS; i++ ) {
		  grid[it]->argv[i] = (char*)malloc(cap + 1);
		}

		grid[it]->cap     = cap; 
		grid[it]->argc    = 0;		
	}

	return grid;
}

void free_cmd_grid(Command **grid)
{
	
	int it;
	
	while(grid[it] != NULL)
	{
		free_cmd(grid[it]);
		it++;
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

int expect_chr(char found, char expected) /* 1: On sucess, 0: Failed*/
{	
	if(found == expected)
	{
		return 1;
	}

	_fputs("[ ERROR ]: \n", STDERR_FILENO);
	_fputs("Syntax error, Expected ", STDERR_FILENO);
	_fputchar(expected, STDERR_FILENO);
	_fputs(" but found ", STDERR_FILENO);
	_fputchar(found, STDERR_FILENO);
	_fputs("\n", STDERR_FILENO);
	
	return 0;
}

int parse_commands(char *buff, EContext *ctx, Command **cmd)
{
	char     *command_buff;
	
	int cset        = 0;
	int cmd_buff_sz = 0;
	int size        = 0;
	int it          = 0;
	int j           = 0;
	
	Command  *command_cp;
	char **tokens;
	tokens       = allocate_char_grid(COMMAND_MAX, BUFF_MAX);
	command_buff = malloc(BUFF_MAX);

	ltrim(buff);
	
	while((*buff != '\0') && (*buff != '\n'))
	{
		switch(*buff)
		{
			case JOIN_SYM: {
				
				if(cset && (*ctx != JOIN))
				{
					_fputs("[ ERROR ]: \n", STDERR_FILENO);
					_fputs("Syntax error, Expected ; but found ", STDERR_FILENO);
					_fputchar(ctx_array[*ctx], STDERR_FILENO);
					_fputs("\n", STDERR_FILENO);
					
					free(command_buff);
					free_char_grid(tokens, COMMAND_MAX);
					free_cmd_grid(cmd);

					return 0;
				}
				
				buff++;
				cset = 1;
				*ctx  = JOIN;

			} break;

			case AND_SYM: {
				
				if(cset && (*ctx != AND))
				{

					_fputs("[ ERROR ]: \n", STDERR_FILENO);
					_fputs("Syntax error, Expected && but found ", STDERR_FILENO);
					_fputchar(ctx_array[*ctx], STDERR_FILENO);
					_fputs("\n", STDERR_FILENO);
					
					free(command_buff);
					free_char_grid(tokens, COMMAND_MAX);
					free_cmd_grid(cmd);

					return 0;
				}

				buff++;

				if(expect_chr(*buff, ctx_array[AND]) == 0)
				{
					free(command_buff);
					free_char_grid(tokens, COMMAND_MAX);
					free_cmd_grid(cmd);
					return 0;
				};

				buff++;
				cset  = 1;
				*ctx  = AND;

			} break;

			case OR_SYM: {
				
				if(cset && (*ctx != OR))
				{

					_fputs("[ ERROR ]: \n", STDERR_FILENO);
					_fputs("Syntax error, Expected || but found ", STDERR_FILENO);
					_fputchar(ctx_array[*ctx], STDERR_FILENO);
					_fputs("\n", STDERR_FILENO);
					
					free(command_buff);
					free_char_grid(tokens, COMMAND_MAX);
					free_cmd_grid(cmd);
					return 0;
				}
				
				buff++;

				if(expect_chr(*buff, ctx_array[OR]) == 0) 
				{
					free(command_buff);
					free_char_grid(tokens, COMMAND_MAX);
					free_cmd_grid(cmd);
					return 0;
				};
				
				buff++;
				cset  = 1;
				*ctx  = OR;
				continue;

			} break;
			
			default: {
				while(*buff && *buff != '&' && *buff != '\0' && *buff != '|' && *buff != ';' && *buff != '\n')
				{
					/*TODO: CHeck if the command buffer is enough to hold a new character.?*/
					if(cmd_buff_sz + 1 == BUFF_MAX)
					{
						command_buff = _realloc(command_buff, (cmd_buff_sz * 2));
					}
					
					command_buff[cmd_buff_sz++] = (char) *buff;
					buff++;
				}

				command_buff[cmd_buff_sz] = '\0';

				if(size + 1 == COMMAND_MAX)
				{
					_fputs("[MEMORY ERROR] Command size reached the limit.\n", STDERR_FILENO);
					_fputs("[SIZE] ",                                          STDERR_FILENO);
					_fputi(size,                                               STDERR_FILENO);
					_fputs("\n",                                               STDERR_FILENO);
					free(command_buff);
					free_cmd_grid(cmd);
					free_char_grid(tokens, COMMAND_MAX);
					
					return 0;
				}
				
				_strcpy(tokens[size], command_buff);
				
				cmd_buff_sz = 0;
				size++;
				
			};
		}	 	
	}

	tokens[size] = NULL;

	for(it = 0; it < size; it++) 
	{
		command_cp = alloc_cmd(BUFF_MAX);

		if(parse_command(tokens[it], command_cp) == 0) 
		{
			cmd[it]->name = _strcpy(cmd[it]->name, command_cp->name);
			
			for(j = 0; j < command_cp->argc; ++j)
			{
				cmd[it]->argv[j] = _strcpy(cmd[it]->argv[j], command_cp->argv[j]);
			}
			
			cmd[it]->argv[j] = NULL;
			cmd[it]->argc    = j;
			cmd[it]->cap     = BUFF_MAX;

			free_cmd(command_cp);
			continue;
		}

		free_cmd(command_cp);
		free_cmd_grid(cmd);
		_fputs("[PARSE ERROR] a command could not be parsed.\n", STDERR_FILENO);
		return 0;
	}
	
	cmd[it] = NULL;
	free_char_grid(tokens, size);

	return it;
}

 

int parse_command(char *buff, Command *cmd) 
{
	char *cp = malloc(BUFF_MAX);
	char *Token;
	
	_strcpy(cp, buff);
	Token = strtok(cp, " ");
	
	if(!buff) return 0;

    while(Token != NULL)
    {
    	if(cmd->argc == 0) {
    		_strcpy(cmd->name, Token);
    	}

    	_strcpy(cmd->argv[cmd->argc++], Token);
        
        if(cmd->argc == 30)
        {
        	printf("Realloc..\n");
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

	_puts("Name: |");
	_puts(c->name);
	_puts("|");
	_puts("\n");
	_puts("Argv: ");
	
	for(i = 0; i < c->argc; i++) 
	{
		_puts(c->argv[i]);
		_puts(" ");
	}
	
	_puts("\n");
}

int _exec(Command *cmd) 
{

	int pid, code, stat = 0;
	pid = fork();
	
	if(pid == 0)
	{
		/* Child Process. */
		code = execve(cmd->name, cmd->argv, environ);

		if(code == -1) 
		{
			perror("[ERROR]");
			_putchar('\n');
			exit(1);
		}

		exit(0);
	}
	
    wait(&stat);
    
    if(WIFEXITED(stat)) 
    {
        code = WEXITSTATUS(stat);
        return code;
    }

    return -2;
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
			_strcpy(c->name   , copy);
			/* _strcpy(c->argv[0], copy); */
			return 0;
		}

		free(copy);
	}

	return 1; /* Not found */
}


void built_in_exit(char **args, int count) 
{
	if(count > 1)
	{
		RUN_FLAG = __atoi(args[1]);
		return;
	}
	
	RUN_FLAG = 0;

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
	int size = 12;
	
	char *dir_post_buffer;
	char *pwd              = getEnv("PWD");
	char *prev             = getEnv("OLDPWD");

	if(pwd == NULL)
	{
		_fputs("Could not get pwd env variable.\n", STDERR_FILENO);
		_fputs("Please check if it is set by the env command. if not then set it.\n", STDERR_FILENO);
		return;
	}
	
	if(prev == NULL) 
	{
		prev = pwd;
	}

	if(count == 1) 
	{
		res = chdir(ROOT);
		
		
		if(res != 0)
		{
			perror("[ERROR (CD)]");
			return;
		}

		setEnv("PWD",    ROOT);
		setEnv("OLDPWD", pwd);
		return;
	}
	
	if(_strcmp(args[1], "-")) 
	{
		
		_puts(prev);
		_puts("\n");
		
		res = chdir(prev);
		
		if (res != 0)
		{
			_puts(args[1]);
			_putchar(' ');
			perror(":");
		}

		setEnv("PWD",    prev);
		setEnv("OLDPWD", pwd);
		return;
	}

	setEnv("OLDPWD", pwd);
	res = chdir(args[1]);
	
	if(res != 0)
	{
		_puts(args[1]);
		_putchar(' ');
		perror(":");
		return;
	}

	while(getcwd(dir_post_buffer, size) == NULL) {
		size += 1;
		dir_post_buffer = malloc(size);
	}

	setEnv("PWD", dir_post_buffer);
	
	if(dir_post_buffer != NULL) {
		free(dir_post_buffer);	
	}
}

int exec_builtin(Command *cmd) 
{
	built_in_command command;

	int i = 0;
	
	for(i = 0; i < BUILT_INS_COUNT; ++i)
	{
		
		command = built_ins[i];
		
		if(_strcmp(command.name, cmd->name)) {
			command.func(cmd->argv, cmd->argc);
			return 1;
		}
	}

	return 0;
}

void construct_built_in(char *name, void (*func)(char **, int), built_in_command *ptr) 
{
	_strcpy((*ptr).name, name);
	
	(*ptr).func = func;
	
	BUILT_INS_COUNT++;
}

void reg_built_ins() {
	
	construct_built_in("exit", built_in_exit, built_ins + 0);
	_puts("\n[0][REG] exit\n");
	
	construct_built_in("env", built_in_env, built_ins + 1);
	_puts("[1][REG] env\n");
	
	construct_built_in("cd", built_in_cd, built_ins + 2);
	_puts("[2][REG] cd\n");
	
	construct_built_in("clear", built_in_clear, built_ins + 3);
	_puts("[3][REG] clear\n\n");
}

/*

void unreg_built_ins()
{
	int i;
	
	for (i = 0; i < BUILT_INS_COUNT; ++i)
	{
		free(built_ins + i);
	}
}

*/

void init() 
{
	ENV_PATHS      = allocate_char_grid(BUFF_MAX, BUFF_MAX);
	ENV_PATHS_SIZE = get_tokenized_path(ENV_PATHS);
	reg_built_ins();
}

void deinit() 
{
	free_char_grid(ENV_PATHS, BUFF_MAX);
}

void execute_joined(Command **command_array, int size)
{
	int i;
	
	for (i = 0; (i < size && command_array[i]); ++i)
	{
		execute_command(command_array[i]);
	}
}

void execute_and(Command **command_array, int size) {
	int i, prev_code;
	
	for (i = 0; (i < size && command_array[i]); ++i)
	{
		prev_code = execute_command(command_array[i]);
		
		if(prev_code != 0)
		{
			return;
		}
	}
}

void execute_or(Command **command_array, int size) {
	int i, prev_code;
	
	for (i = 0; (i < size && command_array[i]); ++i)
	{
		prev_code = execute_command(command_array[i]);
		
		if(prev_code == 0)
		{
			return;
		}
	}

}

int execute_command(Command *cmd)
{
	int result = exec_builtin(cmd);
	
	if(!result) 
	{
		if(find_cmd(cmd, ENV_PATHS, ENV_PATHS_SIZE) == 0)
		{
			return _exec(cmd);
		}

		_fputs("\n", STDERR_FILENO);
		_fputs("ERROR ", STDERR_FILENO);
		_fputs("[ ", STDERR_FILENO);
		_fputs(cmd->name, STDERR_FILENO);
		_fputs(" ]\n", STDERR_FILENO);
		
		perror("");
		
		_fputs("\n", STDERR_FILENO);
	}

	return -1; /* Command Not found. */
}


int shell() {
	int  size = 0;
	
	int  buff_size = BUFF_MAX;
	char *buff = malloc(buff_size);
	
	
	EContext ctx = NONE;
	Command  **command_array = alloc_cmds_buffer(COMMAND_MAX);

	init();
	
	while(RUN_FLAG == 1)
	{
		ctx = NONE;
		size = 0;
		prompt();
		
		size = _getline(buff, &buff_size, stdin);
		
		if(size == 0)
		{
			continue;
		}
		
		size = parse_commands(buff, &ctx, command_array);
		
		if(size == 0)
		{
			continue;	
		}
		
		switch(ctx) 
		{
			case JOIN: {
				/* Execute the sequence No matter what happens to next and prev commands? */
				execute_joined(command_array, size);
			} break;

			case AND: {
				/* Commands were joined with &&, thus we need to execute sequence in this manner, if the nth - 1 command did not fail we execute nth.*/
				execute_and(command_array, size);
			} break;

			case OR: {
				/* Commands were joined with &&, thus we need to execute sequence in this manner, if the nth - 1 command failed we execute nth.*/				
				execute_or(command_array, size);
			} break;

			case NONE: {
				/* No command was Joined so there is only atleast one command to be exectuted. */
				execute_command(command_array[0]);
			} break;
			
			default:
				_fputs("Enable to find the parsed context..\n", STDERR_FILENO);
				break;
		}
		
		
	}
	
	free_cmd_grid(command_array);
	free(buff);
	deinit();
	exit(RUN_FLAG);
}