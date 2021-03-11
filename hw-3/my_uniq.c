#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h> 

int main(int argc, char * arg[]) {
  // set default fd to stdin and stdout
  int fdin = 0, fdout = 1;
  const int MAX_SIZE = 1000;
  
  // find out what file to open or use default fd
  switch (argc) {
    // check for output file. no break statement so will cascade down to the next case for input file
    case 1 :
      break;  // fd for input and output is default to stdin and stdout
    case 3 :
      // get 3rd arg
      if (*arg[2] != '-') {
        // get output file  descrp
        fdout = open(arg[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
        // handle error
        if (fdout == -1) {
          printf("Error occured");
          exit(EXIT_FAILURE);
        }
      }
    // check for input file
    case 2 :
      // get 2nd arg
      if (*arg[1] != '-') {
        // get input file descrp 
        fdin = open(arg[1], O_RDONLY);
        // handle error
        if (fdin == -1) {
          printf("File not found");
          exit(EXIT_FAILURE);
        }
      }
      break;
    // 4 or more args 
    default:
      printf("Error! Too many arguments!\n");
      exit(EXIT_FAILURE);
  }

  // create two buffer to compare
  char * buffer1 = malloc(MAX_SIZE * sizeof(char));
  char * buffer2 = malloc(MAX_SIZE * sizeof(char));
  int i = 0;

  if (buffer1 == NULL || buffer2 == NULL) {
    printf("Error allocating memory in the heap\n");
    exit(EXIT_FAILURE);
  }

  int len1 = 0;
  // read in first line to buffer char by char
  while ((read(fdin, &buffer1[i], sizeof(char))) == 1) {
    // line length is longer than allocated buffer size
    if (len1 > MAX_SIZE) {
      printf("Line length is longer than allocated buffer size!\n");
      exit(EXIT_FAILURE);
    }
    // end of first line
    if (buffer1[i] == '\n') {
      buffer1[++i] = '\0';  // add end indicator
      i = 0;  // reset index to beginning
      break;
    }
    len1++;
    i++;
  }
  // write line in buffer to output
  write(fdout, buffer1, strlen(buffer1));

  int len2 = 0;
  // read in second line char by char for comparison
  while (read(fdin, &buffer2[i], sizeof(char)) == 1) {
    // line length is longer than allocated buffer size
    if (len2 > MAX_SIZE) {
      printf("Line length is longer than allocated buffer size!\n");
      exit(EXIT_FAILURE);
    }
    // check for end of line
    if (buffer2[i] == '\n') {
      buffer2[++i] = '\0';  // add end indicator
      // compare strings
      if (strcmp(buffer1, buffer2) != 0) {
        write(fdout, buffer2, strlen(buffer2));
      }
      // swap pointers
      char * temp_ptr = buffer1;
      buffer1 = buffer2;
      buffer2 = temp_ptr;
      i = 0;
      continue;
    }
    i++;
  } 
  // did not work with std in
  if (fdin != 0) 
    close(fdin);  // close input file
  // did not work with std out
  if (fdout != 1) 
    close(fdout);  // close output file

  // free heap char arrays
  free(buffer1);
  free(buffer2);

  return EXIT_SUCCESS;
}