#include <stdio.h> // for printf(), fgets()
#include <stdlib.h> // for type EXIT_FAILURE and atoi()
#include <sys/types.h> // for pid_t
#include <sys/wait.h> // for wait() and waitpid()
#include <unistd.h> 

// define constants
const int MAX_PROC = 1000;
const int MAX_SIZE = 1000;

int main(int argc, char *argv[]) 
{
  // check if user specify arguments
  if (argc == 1) {
    printf("Error! Too little command-line arguments\n");
    exit(EXIT_FAILURE);
  }
  
  // int max_proc;
  int proc_c = 0;   // process counter
  char *buffer = (char *) malloc(MAX_SIZE * sizeof(char));
  char *ptr = buffer;
  char *filepath = buffer;
  char *p_arr[MAX_SIZE];
  int status;

  // convert char arg to int 
  int n_of_proc = atoi(argv[1]);  // assume user input expected value: int 
  // TODO check for error; wrong type

  // check the number of processes user specified is within allowable bounds
  if (n_of_proc < 1 || n_of_proc > MAX_PROC) {
    printf("Error! Invalid number of running processes\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, MAX_SIZE-1, stdin)) {    // EOF and new line check
    if (proc_c == n_of_proc) {
      pid_t ret = wait(&status);     // wait on child process
      if (ret == -1) 
        perror("wait");
      printf("Normal termination with exit status = %d\n", WEXITSTATUS(status));
      proc_c -= 1;
    }

    // process buffer and create pointers to each arg in buffer
    int count = 1;
    // set pointers in arr to point at each arguments provided
    while(*ptr != '\n') {
      // found one argument
      if (*ptr == ' ' || *ptr == '\t') {
        *ptr = '\0';// replace cur char with terminating char
        p_arr[count] = ++ptr;
        count += 1;
      } 
      else {
        ptr += 1;
      }
    }
    p_arr[0] = filepath;
    p_arr[count] = NULL; // set last pointer in pointers array to NULL pointer

    // fork and exec
    pid_t pid = fork(); 
    if (pid == -1) {                 // error forking
      perror("Can not fork");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0) {             // inside child
      printf("Process with pid %d successfully forked!\n", getpid());
      execv(filepath, p_arr);
      proc_c += 1;
    }

    // check if any child has terminated
    pid_t cpid = waitpid(-1, &status, WNOHANG);  
    if (cpid != 0) {
      printf("Process with pid %d: Normal termination with exit status = %d\n", cpid, WEXITSTATUS(status));
      proc_c -= 1;
    } 

    // Test to check if correct args were read into buffer
    // printf("filepath = %s\n",filepath);
    // for(char **p = p_arr; *p; p++)
    //   printf("pointer = %s\n", *p); 
  }

  // TODO wait on remaining children that are still running
  int pid;
  while (proc_c > 0) {
    if ((pid = waitpid(-1, &status, WNOHANG)) != 0) {
      printf("Process with pid %d: Normal termination with exit status = %d\n", pid, WEXITSTATUS(status));
      proc_c -= 1;
    }
  }

  free(buffer); //free array on heap

  return EXIT_SUCCESS;
}