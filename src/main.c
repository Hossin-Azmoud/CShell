#include "./util.h"

char **splitbydelim(char *command, char delim) {
	return ();
}

// TODO: Make util function lib.
// TODO: Create Main Shell Loop.
// TODO: Create hash table for commands.
int main() {
	int cap  = 25;
	int size = 0;
	int run = 1;
	char c;	
	char buff[cap];
	
	while(run)
	{
		_puts("[C-SHEL] > ");

		while((c = getchar()) != '\n' && c != EOF) {
			buff[size++] = (char) c;		
		}
		
		buff[size++] = '\0';
		_puts(buff);
		_puts("\n");

		if(_strcmp(buff, "exit") == 1) 
		{
			_puts("exiting..\n");
			run = 0;
		}

	

	}

	return 0;
}







