#include <stdio.h>  // printf(), fopen()
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS const
#include <string.h>  // for strle()
#include <pthread.h>

// define constants
const int MAX_FILES = 10000;
const int MAX_SIZE = 1000;

// global var
char * PATTERN;
int TOTAL_LINES = 0;
pthread_mutex_t mutex; 

void * searchfile (void *);  // func called by threads

int main(int argc, char *argv[]) {
  if (argc < 3 || argc > MAX_FILES) {  // check if input arg is within valid bounds
    printf("Invalid input arguments\n");
    exit(EXIT_FAILURE);
  }

  // local var
  pthread_t * threads;
  int thread_ret;
  void * lines_read;
  int pattern_count = 0;

  PATTERN = malloc(strlen(argv[1]));  // allocate memory for pattern char arr
  strcpy(PATTERN, argv[1]);  // copy pattern input into pattern arr 

  threads = malloc((argc - 2) * sizeof (pthread_t)); // alloc mem for threads arr pointer

  // check for mem alloc error
  if (PATTERN == NULL || threads == NULL) {
    printf("Error allocating memory in the heap\n");
    exit(EXIT_FAILURE);
  }

  // creating threads for each filename input
  for (int i = 2; i < argc; i++) {
    thread_ret = pthread_create(&(threads[i - 2]), NULL, searchfile, (void *)(argv[i]));

    // check for error creating threads
    if (thread_ret != 0) {
      perror ("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  // join each threads created 
  for (int i = 0; i < (argc - 2); i++) {
    int ret = pthread_join(threads[i], &lines_read);

    // check for error joining threads
    if (ret != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }

    printf("Thread %d returned with value: %d\n", i, ret);

    pattern_count += (int) lines_read;  // add the pattern read by each thread to total pattern count
  }

  printf("Total of %d matched lines among total of %d lines scanned\n", pattern_count, TOTAL_LINES);

  free(PATTERN);
  free(threads);

  return EXIT_SUCCESS;
}

void * searchfile (void * filename) {
  // local var
  char * buffer;
  int buff_len;
  FILE * file;
  char *ret;
  int pattern_matched_count = 0;

  buffer = malloc(MAX_SIZE * sizeof(char));  // allocate memory

  if (buffer == NULL) {  // check error allocating mem
    printf("Error allocating memory in the heap\n");
    exit(EXIT_FAILURE);
  }

  file = fopen((char *)(filename), "r");  // open file

  if (!file) {  // check error opening file
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  // read each line in file
  while (fgets(buffer, MAX_SIZE, file) != NULL) {
    pthread_mutex_lock(&mutex);
    TOTAL_LINES++;
    pthread_mutex_unlock(&mutex);

    buff_len = strlen(buffer); 

    if (buff_len == MAX_SIZE -1) {  // this checks if line is longer than the buffer 
      perror("Line exceed buffer max allowable size\n");
      exit(EXIT_FAILURE);
    }

    if (buff_len == 0)   // skip this iteration bc no line read from file
      continue;

    ret = strstr(buffer, PATTERN);  // check if pattern exists within a line

    if (ret != NULL) {  // found pattern
      printf("%s: %s\n", filename, buffer);
      pattern_matched_count++;
    }
  }

  printf("--%s has %d matched lines\n", filename, pattern_matched_count);

  // free resources
  free(buffer);
  fclose(file);

  pthread_exit((void *) pattern_matched_count);  // terminate thread
}