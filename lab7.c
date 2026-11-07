#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MAXIMUM 100

typedef struct {
  int number_line;
  int val_double;
} IntermediateInput;

typedef struct {
  int number_line;
  int val;
} Input;

typedef struct {
  int val_double;
  int numbers_lines[INPUT_MAXIMUM];
  int counter;
} Output;

void map(Input *input, IntermediateInput *intermediate_input);
void groupByKey(IntermediateInput *input, Output *output, int *counter_result);
void reduce(Output output);

int main() {
  Input data_input[INPUT_MAXIMUM];
  int size_input = 0;
  int val;

  printf("Enter values (one per line). Type 'end' to finish:\n");
  while (size_input < INPUT_MAXIMUM) {
    char buf[100];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      break;
    }

    if (sscanf(buf, "%d", &val) == 1) { // NOLINT
      data_input[size_input].number_line = size_input + 1;
      data_input[size_input].val = val;
      size_input++;
    } else if (strncmp(buf, "end", 3) == 0) {
      break;
    } else {
      printf("Invalid input. Please enter an integer or 'end' to finish.\n");
    }
  }

  // mapping part
  IntermediateInput results_mapped[INPUT_MAXIMUM] = {0};
  for (int i = 0; i < size_input; i++) {
    map(&data_input[i], &results_mapped[i]);
  }

  // Group by key part
  Output results_output[INPUT_MAXIMUM] = {0};
  int counter_result = 0;
  for (int i = 0; i < size_input; i++) {
    groupByKey(&results_mapped[i], results_output, &counter_result);
  }

  // Reduce part
  for (int i = 0; i < counter_result; i++) {
    if (results_output[i].counter > 0) {
      reduce(results_output[i]);
    }
  }
  return 0;
}

// Complete map part
void map(Input *input, IntermediateInput *intermediate_input) {
  intermediate_input->number_line = input->number_line;
  intermediate_input->val_double = input->val * 2;
}

void groupByKey(IntermediateInput *input, Output *output, int *counter_result) {
  for (int i = 0; i < *counter_result; i++) {
    if (output[i].val_double == input->val_double) {
      output[i].numbers_lines[output[i].counter] = input->number_line;
      output[i].counter++;
      return;
    }
  }

  output[*counter_result].val_double = input->val_double;
  output[*counter_result].numbers_lines[0] = input->number_line;
  output[*counter_result].counter = 1;
  (*counter_result)++;
}

void reduce(Output output) {
  printf("(%d, [", output.val_double);
  for (int i = 0; i < output.counter; i++) {
    printf("%d", output.numbers_lines[i]);
    if (i < output.counter - 1) {
      printf(", ");
    }
  }
  printf("])\n");
}
