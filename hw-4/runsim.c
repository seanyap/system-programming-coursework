#include <stdio.h> // for printf(), fgets()
#include <stdlib.h> // for type EXIT_FAILURE and atoi()
#include <sys/types.h> // for pid_t
#include <sys/wait.h> // for wait() and waitpid()
#include <unistd.h> 

// define constants
const int MAX_PROC = 1000;
const int MAX_SIZE = 10000;

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
  char *ptr;
  char *filepath;
  char *p_arr[MAX_SIZE];
  int status;

  // convert char arg to int 
  int n_of_proc = atoi(argv[1]);  
  if (n_of_proc == 0) {
      printf("Invalid argument!\n");
      exit(EXIT_FAILURE);
  }

  // check the number of processes user specified is within allowable bounds
  if (n_of_proc < 1 || n_of_proc > MAX_PROC) {
    printf("Error! Invalid number of running processes\n");
    exit(EXIT_FAILURE);
  }

  while ((fgets(buffer, MAX_SIZE-1, stdin)) != NULL) {    // EOF and new line check
    if (proc_c >= n_of_proc) {
      pid_t ret = wait(&status);     // wait on child process
      if (ret == -1) 
        perror("wait");
      printf("Process with pid %d: Normal termination with exit status = %d\n", ret, WEXITSTATUS(status));
      proc_c -= 1;
    }

    // process buffer and create pointers to each arg in buffer
    int count = 1;
    ptr = buffer;
    filepath = buffer;
    // set pointers in arr to point at each arguments provided
    while(*ptr != '\n') {
      // found one argument
      if (*ptr == ' ' || *ptr == '\t') {
        *ptr = '\0';  // replace empty space/tab with terminating char
        p_arr[count] = ++ptr;   // current pointer pointing at \0 
        count += 1;
      } 
      else {
        ptr += 1;
      }
    }
    *ptr = '\0';   // replace \n with a \0
    p_arr[0] = filepath + 2; // set first pointer in pointers array to file path without ./
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
      perror("execv returned due to an error");
      exit(EXIT_FAILURE);
    }

    proc_c += 1;  

    // check if any child has terminated
    pid_t cpid = waitpid(-1, &status, WNOHANG);  
    if (cpid != 0) {
      if (WIFEXITED(status))
        printf("Process with pid %d: Normal termination with exit status = %d\n", cpid, WEXITSTATUS(status));
      if (WIFSIGNALED(status))
        printf("Process with pid %d: Killed by signal = %d\n", cpid, WTERMSIG(status), WCOREDUMP(status) ? "(dumped core)" : "");
      if (WIFSTOPPED(status))
        printf("Process with pid %d: Stopped by signal = %d\n", cpid, WSTOPSIG(status));
      if (WIFCONTINUED(status))
        printf("Process with pid %d: Continued\n", cpid);
      proc_c -= 1;
    }
  }

  // wait on remaining children that are still running
  int pid;
  while (proc_c > 0) {
    if ((pid = wait(&status)) != 0) {   // block (wait) upon the remaining children without continuing to doing other stuff
      if (WIFEXITED(status))
        printf("Remaining Process with pid %d: Normal termination with exit status = %d\n", pid, WEXITSTATUS(status));
      if (WIFSIGNALED(status))
        printf("Remaining Process with pid %d: Killed by signal = %d\n", pid, WTERMSIG(status), WCOREDUMP(status) ? "(dumped core)" : "");
      if (WIFSTOPPED(status))
        printf("Reminaing Process with pid %d: Stopped by signal = %d\n", pid, WSTOPSIG(status));
      if (WIFCONTINUED(status))
        printf("Process with pid %d: Continued\n", pid);
      proc_c -= 1;
    }
  }

  free(buffer); // free array on heap

  return EXIT_SUCCESS;
}