#include "_string.h"

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