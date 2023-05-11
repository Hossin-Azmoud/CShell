#include "./util.h"

int _putchar(char c) {
	
	if(!c) 
		return 0;	
	return (write(STDOUT_FILENO, &c, 1));
}

int _strlen(char *s) {
	
	int len = 0;
	
	if(!s) {
		return 0;
	}
	
	while(*s++) {
		len++;
	}
	
	return len;
}

int _puts(char *s) {
	
	int n = 0;

	if(!s) return 0;

	while(s[n]) _putchar(s[n++]);
	
	return n;
}


char *_strcpy(char *dst, char *src) {
	int Tail = _strlen(dst) - 1;
	
	while(*src)
	{
		dst[Tail++] = *src++;
	}

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
	_puts("$$$ ");
	
	while((c = getchar()) != '\n' && c != EOF)
	{
		
		if(BUFF_MAX == size + 1) {
			/* TODO: Realloc. */
			_puts("LIMIT BUFFER");
			exit(-1);
		}

		buff[size++] = (char) c;
	}

	return size;
}


void prompt() {
	_puts("[C-SHEL] > ");
}