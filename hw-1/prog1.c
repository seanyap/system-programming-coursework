#include <stdio.h>  // For 'perror' and other i/o actions
#include <stdlib.h> // For EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h> // For the 'write' system call
#include <string.h> // For 'strlen'

int main (void) 
{ 
	const char *str = "Finished summoning the daemon!\nLet it quietly die now.\n";
	if(write(STDOUT_FILENO, str, strlen(str)) == -1)
	{
		perror("error with the \'write\' system call.\n");
		exit(EXIT_FAILURE); // Same as return 1;
	}

        return EXIT_SUCCESS; // Same as return 0; 
}
