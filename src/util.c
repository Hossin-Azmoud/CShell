#include "./util.h"

int _putchar(char c) {
	
	if(!c) 
		return 0;	
	return (write(STDOUT_FILENO, &c, 1));
}

int _strlen(char *s) {
	
	if(!s) 
		return 0;
	
	int len;
	
	for(len = 0; *s++;len++);
	
	return len;
}

int _puts(char *s) {
	if(!s) return 0;
	
	int n = 0;
	
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






