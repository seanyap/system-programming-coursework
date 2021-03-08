#include <stdio.h>
#include <sys/fcntl.h>

int main(int argc, char * arg[]) {
  // set default fd to stdin and stdout
  int fdin = 0, fdout = 1;

  
  // find out what file to open or use default fd
  switch (argc) {
    // check for output file. no break statement so will cascade down to the next case for input file
    case 3 :
      if (*arg[2] != '-') {
        // get output file  descrp
        fdout = open(arg[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
        printf("output fd: %d\n", fdout);
        if (fdout == -1) {
          // handle error
        }
      }
    // check for input file
    case 2 :
      if (*arg[1] != '-') {
        // get input file descrp 
        fdin = open(arg[1], O_WRONLY | O_CREAT | O_TRUNC, 0664);
        printf("input fd: %d\n", fdin);
        if (fdin == -1) {
          // handle error
        }
      }
      break;
    // 4 or more args 
    default:
      // perror();
      break;
  }

  // create two buffer to compare

  // read in one line 
  // loop through rest of the lines and check with previous line
      // if both lines are the same
          // skip to next iteration
      // add line to buffer

  // write out buffer to output file

  // close opened file
}