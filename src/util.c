#include "./util.h"

void prompt() {
	_puts("[C-SHEL] > ");
}

char *getpath() {
	
	char *envvar = "PATH";
    char *path   = getenv(envvar);

    if(!path) {
        fprintf(stderr, "The environment variable %s was not found.\n", envvar);
        exit(1);
    }

    return path;
}

char *join_path(char *dst, char *src)
{

	int dst_last = _strlen(dst) - 1;
	
	if(dst[dst_last] != '/')
	{
		_strcat(dst, "/");
	}

	_strcat(dst, src);
	
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

void print_env(char *envp[]) {
	
	while(*envp++){
		_puts(*envp);
		_putchar('\n');
	}
}