#include <stdio.h> // for printf(), fgets()
#include <stdlib.h> // for type EXIT_FAILURE and atoi()
#include <sys/types.h>
#include <unistd.h> // for getpid(), getppid()

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
  char *ptr = buffer;
  char *filepath = buffer;
  char *p_arr[MAX_SIZE];

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
        printf("SUCCESS!!\n");
        exit(EXIT_SUCCESS);
      }
      // read input line from stdin to buffer
      if (fgets(buffer, MAX_SIZE-1, stdin)) {
        int count = 0;
        // set pointers in arr to point at each arguments provided
        while(*ptr != '\n') {
          // found one argument
          if (*ptr == ' ' || *ptr == '\t') {
            *ptr = '\0';// replace cur char with NULL char
            p_arr[count] = ++ptr;
            count += 1;
          } 
          else {
            ptr += 1;
          }
        }
        p_arr[count] = NULL; // set last pointer in pointers array NULL

        // fork and exec
        pid_t pid = fork(); 
        if (pid == -1) {                 // error forking
          perror("Can not fork");
          exit(EXIT_FAILURE);
        }
        else if (pid == 0) {             // inside child
          printf("child successfully forked! pid: %d\n", getpid());
          // TODO execv
          exit(EXIT_SUCCESS);
        }
        else {                           // inside parent
          printf("parent pid: %d\n", getpid());          
        }

        // printf("filepath = %s\n",filepath);
        // for(char **p = p_arr; *p; p++)
        //   printf("pointer = %s\n", *p);
      } 
      else {
        printf("Error reading from standard input\n");
        exit(EXIT_FAILURE);
      }

      proc_c += 1;
    }

  } else {
    printf("Error! Invalid number of running processes\n");
    exit(EXIT_FAILURE);
  }

  free(buffer); //free array on heap

  return EXIT_SUCCESS;
}