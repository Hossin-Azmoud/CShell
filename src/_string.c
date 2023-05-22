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

char *_strcpy(char *dst, char *src) 
{
	
	int  it = 0;
	char *dst_start = dst;
	
	
	if(!dst) {
		return NULL;
	}

	while(src[it] != '\0')
	{
		*dst++ = src[it++];
	}
	
	*dst = '\0';
	return dst_start;
}

/*
char *_strcat(char *dst, char *src) {

	
	char *dst_end;

	int sz  = _strlen(dst) + _strlen(src) + 1; 
	dst     = _realloc(dst, sz);

	dst_end = dst + _strlen(dst);
	
	while(*src)
	{
		*dst_end++ = *src++;
	}

	*dst_end = '\0';
	return dst;
}
*/

char *_strcat(char *dst, char *src) 
{
	int sz  = _strlen(dst) + _strlen(src) + 1; 
	int end = _strlen(dst);
	
	dst = _realloc(dst, sz + 1);
	
	while(*src)
	{
		dst[end++] = *src;
		src++;
	}

	dst[end] = '\0';

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


char *join(char *dst, char *src, char *delim)
{
	dst = _strcat(dst, delim);	
	dst = _strcat(dst, src);
	return dst;
}


char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + _strlen(s);
    
    while(isspace(*--back));
    
    *(back+1) = '\0';
    
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}