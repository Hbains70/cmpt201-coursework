#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {

  char *buff = NULL;
  size_t size = 0;

  while (true) {

    printf("Enter programs to run.\n");
    printf("> ");
    fflush(stdout);
    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char == -1) {
      break;
    }
    if (num_char > 0 && buff[num_char - 1] == '\n') {
      buff[num_char - 1] = '\0';
    }
    char *saveptr;
    char *token = strtok_r(buff, " ", &saveptr);
    if (token == NULL) {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("Fork has failed");
      continue;
    }

    if (pid == 0) {
      execl(token, token, (char *)NULL);
      perror("Exec failure\n");
      exit(EXIT_FAILURE);
    } else {
      waitpid(pid, NULL, 0);
    }
  }
  free(buff);
  return 0;
}
