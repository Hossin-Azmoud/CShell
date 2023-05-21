#include "./util.h"


#define STD_NRM      "\x1B[0m"
#define STD_RED      "\x1B[1;31m"
#define STD_GREEN    "\x1B[1;32m"
#define STD_YELLOW   "\x1B[1;33m"
#define STD_BLUE     "\x1B[1;34m"
#define STD_MAGINTA  "\x1B[1;35m"
#define STD_CYAN     "\x1B[1;36m"
#define STD_WHITE    "\x1B[1;37m"


void prompt() {
	char *user_name = getEnv("USER");
	char *pwd       = getEnv("PWD");
	
	if(user_name && pwd)
	{
		_putchar('[');
		_cputs(user_name, STD_YELLOW);
		_putchar(']');
		_putchar('[');
		_cputs(pwd, STD_MAGINTA);
		_putchar(']');
		_cputs(" >>> ", STD_CYAN);
		return;
	}

	_fputs("Could not find USER env variable. please check if it is set.\n", STDERR_FILENO);
	exit(1);
	
}

char *getpath() {
	
	char *envvar = "PATH";
    char *path   = getEnv(envvar);

    if(!path) {
        _fputs("PATH env was not found!.\n", STDERR_FILENO);
        exit(1);
    }

    return path;
}

char *join_path(char *dst, char *src)
{	
	
	int dst_last = _strlen(dst) - 1;	
	
	if(dst[dst_last] != '/')
	{
		dst = join(dst, src, "/");
		return dst;
	}

	dst = _strcat(dst, src);
	
	return dst;
}

int get_tokenized_path(char **paths)
{
	
	int    token_len;
	char   delim[]   = ":";
	char   *path     = getpath();
	size_t token_idx = 0;

	/* Tokenize */
	char    *Token    = strtok(path, delim);
	
	if(!path) return token_idx;
    
    while(Token != NULL)
    {
    	token_len          = _strlen(Token);
		Token[token_len]   = '\0';
		_strcpy(paths[token_idx++], Token);
		Token = strtok(NULL, delim);
	}

	/* terminate paths */
	paths[token_idx] = NULL;
	
	return token_idx;
}

