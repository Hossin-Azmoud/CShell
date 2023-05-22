#include "shell.h"
#include <assert.h>

#define TESTING 0

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
	reg_built_ins();
}