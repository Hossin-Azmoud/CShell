#include "_std.h"

extern char **environ;

int _fputchar(char c, int Stream) {
	
	if(!c) return 0;
	return (write(Stream, &c, 1));
}


char *getEnv(char *key) {
	
	int  i;
	int  size  = 0;
	char *copy;
	char *k = { 0 };
	char *v = { 0 };

	for(i = 0; environ[i]; i++)
	{
		
		size = _strlen(environ[i]);
		
		copy = malloc(size);

		_strcpy(copy, environ[i]);

		k = strtok(copy, "=");
		v = strtok(NULL, "=");
		
		if(_strcmp(k, key)) 
		{
			return v;
		}

		free(copy);
	}

	free(copy);
	return NULL;
}

char *setEnv(char *key, char *value) 
{

	
	char **new_environ;
	char *copy;
	char *new_var;
	int  i;
	int  max_col   = 0;
	int  col_size  = 0;
	char *k        = { 0 };
	char *v        = { 0 };

	for(i = 0; environ[i]; i++)
	{
		col_size = _strlen(environ[i]);
		
		if(col_size > max_col) 
		{
			max_col = col_size;
		}
		
		copy = malloc(col_size);
		_strcpy(copy, environ[i]);

		k = strtok(copy, "=");
		v = strtok(NULL, "=");
		
		if(_strcmp(k, key)) /* found. */
		{
			new_var = join(key, value, "=");
			_strcpy(environ[i], new_var); /* Key = Value */
			return v;
		}
	}

	new_var          =  join(key, value, "=");
	new_environ  =  allocate_char_grid(i, max_col);

	_strcpy(new_environ[i - 1], "");
	i--;
	
	_strcpy(new_environ[i - 1], key); /* Key = Value */
	i--;
	
	while(i-- > 0) {
		_strcpy(new_environ[i - 1], environ[i - 1]);
	}

	environ = new_environ;	

	return "";
}

int _fputs(char *s, int Stream) {
	
	int n = 0;

	if(!s) {
		return n;
	};

	while(s[n]) {
		n += _fputchar(s[n], Stream);
	}

	return n;
}

#if 0
/* TODO:  make converson functions. */
int __itoa(int n, char *buff, int size) {
	
	
}

int __atoi(char *buff)
{
    int n = 0;
    size_t len = strlen(buff);
    int digit_ = 0;

    while(len-- > 0) {

        if(len - 1 == 0) {
            if(buff[0] == '-') {
                n *= -1;
                break;
            }
        }

        n += (buff[len - 1] - '0' * digit_);
        digit_ *= 10;
    }

    return n;
}

#endif

int _putchar(char c) {
	return _fputchar(c, STDOUT_FILENO);
}

int _puts(char *s) {
	return _fputs(s, STDOUT_FILENO);
}

char *_realloc(char *ptr, int size) {
	
	char *new       = malloc(size);
	char *new_start = new;
	
	if(new == NULL) {
		return NULL;
	}
	
	while(*ptr != '\0')
	{
		*new++ = *ptr++;
	}
	
	return new_start;
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

int read_command(char *buff, int cap)
{
	int  size = 0;
	int  cursor = 0;
	int  c = 0;

	char Seq[3];
	
	while(1)
	{
		read(STDIN_FILENO, &c, 1);
		
		if(c == SEQ_START_BYTE)
		{
			read(STDIN_FILENO, &Seq[0], 1);
			read(STDIN_FILENO, &Seq[1], 1);
			
			if (Seq[0] == SEQ_SEC_BYTE)
			{
				switch(Seq[1]) {
					case UP_KEY: {
						_puts("U Clicked `Up_key`!");
					} break;
					case DOWN_KEY: {
						_puts("U Clicked `Down_key`!");
					} break;
					case RIGHT_KEY: {
						if(cursor < size) {
							cursor++;
						}
						
						fseek(stdin, cursor, SEEK_SET);
					
					} break;
					case LEFT_KEY: {
						if(cursor > 0) {
							cursor--;
						}

						fseek(stdin, cursor, SEEK_SET);
					} break;
					case END_KEY: {
						cursor = size - 1;
						_puts("U Clicked `End_key`!");
					} break;
					case HOME_KEY: {
						cursor = 0;
						_puts("U Clicked `Home_key`!");
					} break;
					default: {
						_puts("U Clicked `Unkown key.`!");
						printf("%i\n", Seq[1]);
					}
				}
				
				continue;
			}

			continue;
		}
		
		if(c == EOF || c == 0) {
			exit(1);
		}

		if(c == '\n') {
			break;
		}

		if(cap == size + 1) {
			buff = _realloc(buff, size + 1);
			cap += 1;
		}

		buff[cursor++] = (char) c;
		size++;
	}

	buff[size] = '\0';
	return size;
}