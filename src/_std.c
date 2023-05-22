#include "_std.h"

extern char **environ;

int _fputchar(char c, int Stream)
{
	if(!c) return 0;
	return (write(Stream, &c, 1));
}

int _fputi(int i, int Stream) {
	
	
	int  n;
	char *buff  = malloc(MAX_DIGIT);
	buff[MAX_DIGIT] = '\0';
	buff = __itoa(i, &buff);
	n = _fputs(buff, Stream);
	
	return n;
}

char *getEnv(char *key) {
	
	int  i;
	int  size  = 0;
	char *k    = { 0 };
	char *v    = { 0 };
	char *copy;
	
	for(i = 0; environ[i] != NULL; i++) 
	{
		
		size = _strlen(environ[i]);
		copy = malloc(size + 1);

		_strcpy(copy, environ[i]);

		k = strtok(copy, "=");
		v = strtok(NULL, "=");
		
		if(_strcmp(k, key)) 
		{
			return v;
		}
		if(copy != NULL) {
			free(copy);
		}
	}

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

	for(i = 0; environ[i]; ++i)
	{

		col_size = _strlen(environ[i]);
		
		
		if(col_size > max_col) 
		{
			max_col = col_size;
		}
		
		/* printf("[%i] %i\n", i, col_size); */


		copy = malloc(col_size);
		
		_strcpy(copy, environ[i]);

		k = strtok(copy, "=");
		v = strtok(NULL, "=");
		
		if(_strcmp(k, key)) /* found. */
		{
			
			
			new_var = join(k, value, "=");
			_strcpy(environ[i], new_var);
			free(copy);	
			return v;
		}

		free(copy);
	}
	
	new_var      =  join(key, value, "=");
	new_environ  =  allocate_char_grid(i, max_col);

	if(new_environ == NULL)
	{
		_fputs("Could not set env: \n", STDERR_FILENO);
		_fputs(new_var, STDERR_FILENO);
		_fputs("\nmalloc failed to allocate.\n", STDERR_FILENO);
		free_char_grid(new_environ, i);
		return NULL;
	}

	new_environ[i - 1] = NULL;
	i--;

	_strcpy(new_environ[i - 1], new_var); /* Key = Value */
	i--;
	

	while(i > 0)
	{
		_strcpy(new_environ[i - 1], environ[i - 1]);
		i--;
	}
	
	environ = new_environ;

	return "";
}

void unsetEnv(char *key)
{
	int i;
	int  skipped   = 0;
	char *k        = { 0 };
	
	
	char **new_environ = allocate_char_grid(BUFF_MAX, BUFF_MAX);
	char *copy;

	for(i = 0; environ[i]; i++)
	{
		
		copy = malloc(_strlen(environ[i]));
		_strcpy(copy, environ[i]);
		
		k = strtok(copy, "=");
		strtok(NULL, "=");
		
		if(_strcmp(k, key))
		{
			i++;
			skipped = 1;
			break;
		}

		new_environ[i] = environ[i];
	}

	if(!skipped)
	{
		_fputs("[ERROR] Could not unset env: \n", STDERR_FILENO);
		_fputs(key, STDERR_FILENO);
		_fputs("\nvariable Was not found. \n", STDERR_FILENO);
		free_char_grid(new_environ, BUFF_MAX);
		
		return;
	}
	
	
	/* Move the rest. */
	while(environ[i]) {
		new_environ[i] = environ[i];
		i++;
	}

	new_environ[i] = NULL;
	environ        = new_environ;
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

int _putchar(char c) {
	return _fputchar(c, STDOUT_FILENO);
}

int _puts(char *s) {
	return _fputs(s, STDOUT_FILENO);
}

char *_realloc(char *ptr, int size) 
{
	
	char *new;
	char *new_start;
	new = malloc(size);
	
	
	new_start = new;
	
	if(new == NULL) {
		return NULL;
	}
	
	while(*ptr != '\0')
	{
		*new++ = *ptr++;
	}
	
	*new = '\0';

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
	char *ptr;
    
    for (i = 0; i < row; ++i) 
    {
    	ptr = grid[i]; 
    	if(ptr) { free(ptr); }
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
				switch(Seq[1]) 
				{
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
		
		if(c == '\n') 
		{
			if(size > 0)
			{
				buff[size] = '\0';
			}

			return size;
			
		}

		if(c == EOF || c == 0) {
			exit(1);
		}
		
		if(cap == size + 1) {
			buff = _realloc(buff, size + 1);
			cap += 1;
		}

		buff[cursor++] = (char) c;
		size++;
	}
}


int _cputs(char *s, char *color_seq) {
	int n = 0;

	n += _puts(color_seq);
	n += _puts(s);
	n += reset_term_color();

	return n;
}

char *__itoa(int n, char **buff)
{
	int  size       = _strlen(*buff);
	char *buff_ptr  = (*buff + size - 1);
	
	int is_signed  = 0;
	
	
	
	if(n < 0) {
		is_signed = (int)(n < 0);
		n = -n;
	}
	
	if(n == 0) {
		
		*buff_ptr-- = '0';
		*buff_ptr--   = '\0';
		*buff = buff_ptr + 1;
		
		return "0\0";
	}

	*buff_ptr-- = '\0';

	for(; 1;(n = (n / 10)), *buff_ptr--)  {
		
		*buff_ptr  = (n % 10 + '0');
		
		if(n == 0) {
			
			if (is_signed)
		    {
		    	*buff_ptr-- = '-';
		    }
			
			*buff = buff_ptr + 1;
		    
		    return  *buff;
		}
	}
}


int __atoi(char *buff) {
    
    int n = 0;
    size_t len = strlen(buff);
    int digit_ = 1;
	
	for(; len > 0; len--)
	{
		
		if(len - 1 == 0) {
			
			if(buff[len - 1] == '-') {
                n = -n;
                break;
            }

            n += (buff[len - 1] - '0') * digit_;
            break;
		}

		n += (buff[len - 1] - '0') * digit_;
		digit_ *= 10;
	}

	return n;
}
