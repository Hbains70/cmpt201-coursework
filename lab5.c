#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {

  // Going through fress list of which it gets the first block meeting size condition
  struct header *current = free_list_ptr;
  while (current != NULL) {
    if (current->size >= size) {
      return current->id;
    }
    current = current->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {

  // Finding a block whihc is big enough but the lowest that meets size condition
  struct header *current = free_list_ptr;
  int best_fit_id = -1;
  uint64_t best_of_size = UINT64_MAX;
  while (current != NULL) {
    if (current->size < best_of_size && current->size >= size) {
      best_of_size = current->size;
      best_fit_id = current->id;
    }
    current = current->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {

  // Search for the biggest block thats huge to suffice the size condition
  struct header *current = free_list_ptr;
  int worst_fit_id = -1;
  uint64_t worst_of_size = 0;
  while (current != NULL) {
    if (current->size > worst_of_size && current->size >= size) {
      worst_of_size = current->size;
      worst_fit_id = current->id;
    }
    current = current->next;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for the First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for the Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for the Worst-Fit algorithm is: %d\n", worst_fit_id);

  return 0;
}

/*
 * PART 2: Coalescing contiguous free blocks
 *
 * Lets assume the following;
 * free_list_ptr's list is considered coalesced already
 * the free_list_ptr points to the free_list that is organized by memory address increasing
 * Only required to handle the single newer_block which is a newly freed block
 *
 * Pseducode algorithm;
 *
 * previous = NULL
 * current = free_list_ptr
 *
 * // Finding the spot thats the right one through the address for the newer_block
 * while (current !=NULL && address(current) < address(newer_block)):
 *      previous = current
 *      current = current->next
 *
 * // First situation: Regarding the previous free block, get to merging only if adjacent
 * if (previous != NULL && address(previous) + prev->size == address(newer_block)):
 *     previous->size += newer_block->size
 *     newer_block = previous
 * else:
 *    // Implement towards the free_list_ptr's list the newer_block
 *    newer_block->next = current
 *    if (prev == NULL):
 *        free_list_ptr = newer_block
 *    else:
 *        previous->next = newer_block
 *
 * // Second situation: Regarding the next free block, get to merging only if adjacent
 * if (current != NULL && address(newer_block) + newer_block->size == address(current)):
 *    newer_block->size += current->size
 *    newer_block->next = current->next
 */
