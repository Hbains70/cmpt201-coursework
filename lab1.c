#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  char *buff = NULL;
  size_t size = 0;

  while (true) {

    printf("Please enter some text: ");
    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char == -1) {
      perror("The getline fails");
      exit(EXIT_FAILURE);
    }

    char *saveptr;
    char *token = strtok_r(buff, " ", &saveptr);
    printf("Tokens:\n");
    while (token != NULL) {
      printf("%s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }
  }
  free(buff);
  return 0;
}
