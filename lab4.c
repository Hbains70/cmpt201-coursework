#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#define BUF_SIZE 128
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(char *message) {
  write(STDERR_FILENO, message, strlen(message));
  write(STDERR_FILENO, "\n", 1);
  exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format, // NOLINT
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {

  void *heap_space = sbrk(256);
  if (heap_space == (void *)-1) {
    perror("Sbrk failed\n");
    return 1;
  }

  size_t size_of_block = 128;
  size_t size_of_header = sizeof(struct header);
  size_t size_of_data = size_of_block - size_of_header;

  struct header *block1 = (struct header *)heap_space;
  (*block1).next = NULL;
  (*block1).size = size_of_block;

  struct header *block2 = (struct header *)(char *)heap_space + size_of_block;
  (*block2).next = block1;
  (*block2).size = size_of_block;

  bzero((char *)block1 + size_of_header, size_of_data);     // NOLINT
  memset((char *)block2 + size_of_header, 1, size_of_data); // NOLINT

  print_out("first block:       %p\n", &block1, sizeof(block1));
  print_out("second block:      %p\n", &block2, sizeof(block2));
  print_out("first block size:  %lu\n", &block1->size, sizeof(block1->size));
  print_out("first block next:  %p\n", &block1->next, sizeof(block1->next));
  print_out("second block size: %lu\n", &block2->size, sizeof(block2->size));
  print_out("second block next: %p\n", &block2->next, sizeof(block2->next));

  unsigned char *data_block1 = (unsigned char *)((char *)block1 + size_of_header);
  for (size_t i = 0; i < size_of_data; i++) {
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, "%u\n", data_block1[i]); // NOLINT
    if (len < 0) {
      handle_error("snprintf");
    }
    write(STDOUT_FILENO, buf, len);
  }

  unsigned char *data_block2 = (unsigned char *)((char *)block2 + size_of_header);
  for (size_t i = 0; i < size_of_data; i++) {
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, "%u\n", data_block2[i]); // NOLINT
    if (len < 0) {
      handle_error("snprintf");
    }
    write(STDOUT_FILENO, buf, len);
  }
  return 0;
}
