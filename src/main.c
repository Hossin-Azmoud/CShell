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

	/* Testing parser */
	int i = 0;
	
	char s[BUFF_MAX] = "cd ; ls ; cd\0";
	Command  **command_array = parse_commands(s);

	
	while(command_array[i] != NULL) {
		printf("MAIN: %s\n", command_array[i]->name);
		i++;
	}
	
	command_array = command_array;

	free_cmd_grid(command_array);

}