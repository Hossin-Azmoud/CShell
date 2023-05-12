#include "./util.h"

int _putchar(char c) {
	
	if(!c) 
		return 0;	
	return (write(STDOUT_FILENO, &c, 1));
}

int _strlen(char *s) {
	
	int len = 0;
	
	if(s == NULL) {
		return 0;
	}
	
	while(*s++) {
		len++;
	}
	
	return len;
}

int _puts(char *s) {
	
	int n = 0;

	if(!s) {
		return n;
	};

	while(s[n]) {
		n += _putchar(s[n]);	
	} 
	
	return n;
}


char *_strcpy(char *dst, char *src) {
	
	char *dst_start = dst;

	if(!dst) { 
		return NULL;
	}

	while(*src)
	{
		*dst++ = *src++;
	}
	
	*dst = '\0';
	
	return dst_start;
}

char *_strcat(char *dst, char *src) {
	/* mem alloc */
	char *tmp = malloc( _strlen(dst) + _strlen(src) + 1 );
	char *dst_end;

	_strcpy(tmp, dst);
	
	*dst = *tmp;
	dst_end = dst + _strlen(dst);
	
	while(*src) 
	{
		*dst_end++ = *src++;
	}

	*dst_end = '\0';
	
	return dst;
}

/* returns 1 if they r the same, else return 0 */
int  _strcmp(char *s1, char *s2) {
	
	if(_strlen(s1) == _strlen(s2)) {
		int it = 0;
		while(s1[it])
		{
			if (s1[it] != s2[it]) 
			{
				return 0;
			}

			it++;	
		}

		return 1;
	}
	
	return 0;
}

int read_command(char *buff) {
	int c, size = 0;

	while((c = getc(stdin)) != '\n')
	{
		
		if(BUFF_MAX == size + 1) {
			/* TODO: Realloc. */
			exit(1);
		}

		if(c == EOF) {
			exit(1);
		};

		buff[size++] = (char) c;
	}

	return size;
}


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

char **allocate_char_grid(int row, int col) 
{
	int i;
	char **grid = (char**) malloc(row * sizeof(char *));
    
    for (i = 0; i < row; i++) 
    {
    	grid[i] = malloc(col);
    }

	return grid;
}

void free_char_grid(char **grid, int row)
{
	int i;
    
    for (i = 0; i < row; i++) 
    {
    	free(grid[i]);
    }

    free(grid);
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