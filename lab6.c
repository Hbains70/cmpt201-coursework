#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)                                                                               \
  {                                                                                                \
    if (!(expr)) {                                                                                 \
      fprintf(stderr, "Assertion failed: %s\n", #expr);                                            \
      exit(1);                                                                                     \
    }                                                                                              \
  }

#define TEST(expr)                                                                                 \
  {                                                                                                \
    if (!(expr)) {                                                                                 \
      fprintf(stderr, "Test failed: %s\n", #expr);                                                 \
      exit(1);                                                                                     \
    } else {                                                                                       \
      printf("Test passed: %s\n", #expr);                                                          \
    }                                                                                              \
  }

typedef struct node {
  uint64_t data;
  struct node *next;
} node_t;

node_t *head = NULL;

void insert_sorted(uint64_t data) {
  node_t *new_node = malloc(sizeof(node_t));
  new_node->data = data;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
  } else {
    node_t *curr = head;
    node_t *prev = NULL;

    bool inserted = false;
    while (curr != NULL && !inserted) {
      if (data < curr->data) {
        if (prev == NULL) { // FIX on code 45- 47 is a head insertion
          new_node->next = head;
          head = new_node;
        } else {
          prev->next = new_node;
          new_node->next = curr;
        }
        inserted = true;
      }
      prev = curr;
      curr = curr->next;
    }
    if (!inserted) { // FIX on code 57-58 is insertion at last
      prev->next = new_node;
    }
  }
}

int index_of(uint64_t data) {
  node_t *curr = head;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

int main() {
  insert_sorted(1);
  insert_sorted(2);
  insert_sorted(5);
  insert_sorted(3);

  TEST(index_of(3) == 2);

  insert_sorted(0);
  insert_sorted(4);

  TEST(index_of(4) == 4);

  return 0;
}
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)                                                                               \
  {                                                                                                \
    if (!(expr)) {                                                                                 \
      fprintf(stderr, "Assertion failed: %s\n", #expr);                                            \
      fprintf(stderr, "  at %s:%d\n", __FILE__, __LINE__);                                         \
      exit(1);                                                                                     \
    }                                                                                              \
  }

#define TEST(expr)                                                                                 \
  {                                                                                                \
    if (!(expr)) {                                                                                 \
      fprintf(stderr, "Test failed: %s\n", #expr);                                                 \
      exit(1);                                                                                     \
    }                                                                                              \
  }

typedef struct node {
  uint64_t data;
  struct node *next;
} node_t;

typedef struct info {
  uint64_t sum;
} info_t;

node_t *head = NULL;
info_t info = {0};

// implementing sum of all elements in a list
uint64_t add_up_list() {
  uint64_t sum = 0;
  node_t *current = head;
  while (current != NULL) {
    sum += current->data;
    current = current->next;
  }
  return sum;
}

void insert_sorted(uint64_t data) {
  node_t *new_node = malloc(sizeof(node_t));
  new_node->data = data;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
  } else if (data < head->data) {
    new_node->next = head;
    head = new_node;
  } else {
    node_t *curr = head;
    node_t *prev = NULL;

    while (curr != NULL) {
      if (data < curr->data) {
        break;
      }

      prev = curr;
      curr = curr->next;
    }

    prev->next = new_node;
    if (curr != NULL) {
      new_node->next = curr; // FIX to no longer being curr->next
    }
  }

  info.sum += data;
}

int index_of(uint64_t data) {
  node_t *curr = head;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

int main() {
  insert_sorted(1);
  ASSERT(info.sum = add_up_list());
  insert_sorted(3);
  ASSERT(info.sum = add_up_list());

  insert_sorted(5);
  ASSERT(info.sum = add_up_list());
  insert_sorted(2);
  ASSERT(info.sum = add_up_list());

  TEST(info.sum == 1 + 3 + 5 + 2);
  TEST(index_of(2) == 1);
  ASSERT(info.sum = add_up_list()); // This is last of check

  return 0;
}
