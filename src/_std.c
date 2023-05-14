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
	int size = 0;
	char Seq[3];
	int c = 0;

	while(1)
	{
		read(STDIN_FILENO, &c, 1);
		
		if(c == SEQ_START_BYTE) 
		{
			read(STDIN_FILENO, &Seq[0], 1);
			read(STDIN_FILENO, &Seq[1], 1);
			if (Seq[0] == '[')
			{
				/*
				Up_key = 65,
				Down_key,
				Right_key,
				Left_key */
				switch(Seq[1]) {
					case Up_key: {
						_puts("U Clicked `Up_key`!");
					} break;
					case Down_key: {
						_puts("U Clicked `Down_key`!");
					} break;
					case Right_key: {
						_puts("U Clicked `Right_key`!");
					} break;
					case Left_key: {
						_puts("U Clicked `Left_key`!");
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

		buff[size++] = (char) c;
	}

	return size;
}