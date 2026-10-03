#define _POSIX_C_SOURCE 2008909L
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINES_FOR_PRINT 5

int main() {

  char *buff = NULL;
  size_t size = 0;
  int linecount = 0;
  char *lines[MAX_LINES_FOR_PRINT] = {NULL};

  while (true) {

    printf("Enter input: ");
    fflush(stdout);
    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char == -1) {
      break;
    }

    if (num_char > 0 && buff[num_char - 1] == '\n') {
      buff[num_char - 1] = '\0';
    }

    // Parts for lab 3
    // 5 lines inputted means shifting once and free the oldest one
    if (linecount == MAX_LINES_FOR_PRINT) {
      free(lines[0]);
      for (int i = 0; i < MAX_LINES_FOR_PRINT - 1; ++i) {
        lines[i] = lines[i + 1];
      }
      linecount--;
    }

    // Now take in the input and copy it
    lines[linecount] = strdup(buff);
    linecount++;

    // Check once user enters print and print the last 5 lines of words
    if (strcmp(buff, "print") == 0) {
      for (int i = 0; i < linecount; ++i) {
        printf("%s\n", lines[i]);
      }
    }
  }
  free(buff);
  for (int i = 0; i < linecount; ++i) {
    free(lines[i]);
  }
  return 0;
}
