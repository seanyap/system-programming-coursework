#include <stdio.h> // for printf(), fgets()
#include <stdlib.h> // for type EXIT_FAILURE and atoi()
#include <unistd.h> // for STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO

// define constants
const int MAX_CHILDREN = 1000;
const int MAX_SIZE = 1000;

int main(int argc, char *argv[]) 
{
  // check if user specify arguments
  if (argc == 1) {
    printf("Error! Too little command-line arguments\n");
    exit(EXIT_FAILURE);
  }
  
  int max_proc;
  int proc_c = 0;
  char *buffer = (char *) malloc(MAX_SIZE * sizeof(char));

  // convert string arg from cmd line to int 
  int n_of_proc = atoi(argv[1]);  // assume user input expected value: int 
  // TODO check for error; wrong type

  // check the number of processes user specified is within allowable bounds
  if (n_of_proc >= 1 && n_of_proc <= MAX_CHILDREN) {
    max_proc = n_of_proc;

    while (proc_c <= max_proc) {  // TODO add EOF check
      if (proc_c == max_proc) {
        // TODO wait on child process
        // proc_c -= 1;
      }
      // read input line from stdin to buffer
      if (fgets(buffer, MAX_SIZE-1, stdin)) {
        char *ptr = buffer;
        char *path = buffer;
        char * p_arr[MAX_SIZE];
        
        int count = 0;
        // set pointers that point at each arguments 
        while(*ptr != '\n') {
          // found one argument
          if (*ptr == ' ' || *ptr == '\t') {
            *ptr = '\0';// replace cur char with NULL char
            p_arr[count] = ++ptr;
            count += 1;
          } else {
            ptr += 1;
          }
        }
        p_arr[count] = NULL; // set last pointer in pointers array NULL

        for(char **p = p_arr; *p; p++)
          printf("pointer = %s\n", *p);

        printf("Line loaded into path = %s\n",path);
      } 
      // else {
      //   printf("Error reading from standard input");
      //   exit(EXIT_FAILURE);
      // }

      proc_c += 1;
    }

  } else {
    printf("Error! Invalid number of running processes\n");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}