#include "_std.h"

int _fputchar(char c, int Stream) {
	
	if(!c) return 0;
	return (write(Stream, &c, 1));
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

char *_realloc(char *ptr, int size) {
	char *new = malloc(size);

	if(new == NULL) {
		return NULL;
	}
	
	while(*ptr)
	{
		*ptr++ = *new++;
	}

	free(ptr);
	return new;
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
	int size, cursor = 0;
	char *c = malloc(1);
	
	while(read(STDIN_FILENO, c, 1))
	{
		if(*c == '\n') 
		{
			break;
		}

		switch((int) *c) {
			case EOF: {
				exit(1);
			} break;

			case End_key: {
				cursor = size;
			
			} break;
			
			case Left_key: {
				if(cursor > 0) {
					cursor--;
				}
				
				lseek(STDIN_FILENO, cursor, SEEK_SET);
			
			} break;
			
			case Right_key: {
				if(cursor < size) {
					cursor++;
				}

				lseek(STDIN_FILENO, cursor, SEEK_SET);
			
			} break;
			
			default: {
				
				if(cap == size + 1) {
					buff = _realloc(buff, size + 1);
					cap += 1;
				}

				buff[size++] = *c;
				cursor++;
				printf("%s\n", buff);
			} break;
		}
	}

	return size;
}