#include "shell.h"
void test_unit();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {
	test_unit();
	return 0;
}

void test_unit() 
{

	char *buffer = malloc(BUFF_MAX);
	int  x = 0;
	int  n = -23472;

	__itoa(n, &buffer);
	x = __atoi(buffer);
	
	printf("%i -> %s\n", n, buffer);
	printf("x => %i\n", x);
}