#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int _putchar(char c) {
	
	if(!c) 
		return 0;	
	return (write(1, &c, 1));
}

int _strlen(char *s) {
	if(!s) 
		return 0;
	
	int len = 0;

	for(; *s++;len++);
	
	return len;
}

int main() {
	char s[] = "2021";
	int len = _strlen(s);
	printf("%s -> %i\n", s, len);
	return 0;
}


