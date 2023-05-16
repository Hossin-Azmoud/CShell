#include "shell.h"
#include <assert.h>

#define TESTING 1

void test_unit();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
	#if TESTING
		test_unit();
	#else
		shell();
	#endif
	
	return 0;
}

void test_unit() 
{
	{
		/* testing unset env. */
		char *val;
		setEnv("Hello", "World");
		
		printf("set!\n");
		
		val = getEnv("Hello");
		printf("%s\n", val);
		
		unsetEnv("Hello");
		printf("unset!\n");
			
		val = getEnv("Hello");
		
		assert(val == NULL && "Value was found after unsetting");
	}
}