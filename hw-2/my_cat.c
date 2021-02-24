#include <stdio.h>
#include <stdlib.h> // for exit() and exit_code 

int main (int argc, char * argv[]) 
{
  FILE * pFile;

  // Check command line arguments
  if (argc == 1) 
  {
    printf("No file was provided \n");
    exit(EXIT_FAILURE);
  }

  // iterate through all provided files
  for (int i=1; i<argc; i++)
  { 
    char c;
    // Open file
    pFile = fopen (argv[i],"r");
    if (pFile == NULL) 
    {
      printf("Cannot open file \n");
      exit(EXIT_FAILURE);
    }
    printf("\nFile #%d:\n", i);

    // Read contents from file
    c = fgetc(pFile);
    while (c != EOF) 
    {
      printf("%c", c);
      c = fgetc(pFile);
    }
   }
  
  fclose (pFile);
  return EXIT_SUCCESS;
}
