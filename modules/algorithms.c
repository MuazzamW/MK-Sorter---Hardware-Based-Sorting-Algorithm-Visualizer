// PROGRAM: DEFINES ALL ALGORITHMS

#include "algorithms.h"

#include <stdbool.h>
#include <stdio.h>

// DECLARATIONS ---------------------------------------------------------
void swap(int* first, int* second);
void saveArray(int arr[], int n, int steps_arr[MAX_STEPS][n], int* step_count);
int quickSortPartition(int arr[], int low, int high, int n,
                       int steps_arr[MAX_STEPS][n], int* step_count);
void quickSortRecursive(int arr[], int low, int high, int n,
                        int steps_arr[MAX_STEPS][n], int* step_count);

// HELPERS ---------------------------------------------------------------

// Function that swaps two values
void swap(int* first, int* second) {
  int temp = *first;
  *first = *second;
  *second = temp;
}

// Copies the original array into the 2D array
void saveArray(int arr[], int n, int steps_arr[MAX_STEPS][n], int* step_count) {
  for (int element = 0; element < n; element++) {
    steps_arr[*step_count][element] = arr[element];
  }
  *step_count = *step_count + 1;
}

// Function to partition the function
int quickSortPartition(int arr[], int low, int high, int n,
                       int steps_arr[MAX_STEPS][n], int* step_count) {
  int pivot = arr[high];
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (arr[j] < pivot) {
      i++;

      // Only copy over array if there is a change
      if (i != j) {
        swap(&arr[i], &arr[j]);
        saveArray(arr, n, steps_arr, step_count);
      }
    }
  }

  // Only swap pivot and copy over array if needed
  if (i + 1 != high) {
    swap(&arr[i + 1], &arr[high]);
    saveArray(arr, n, steps_arr, step_count);
  }

  return i + 1;
}

// Quick sort recursive function
void quickSortRecursive(int arr[], int low, int high, int n,
                        int steps_arr[MAX_STEPS][n], int* step_count) {
  if (low < high) {
    int pi = quickSortPartition(arr, low, high, n, steps_arr, step_count);

    quickSortRecursive(arr, low, pi - 1, n, steps_arr, step_count);
    quickSortRecursive(arr, pi + 1, high, n, steps_arr, step_count);
  }
}

// BUBBLE SORT -------------------------------------------------------------

// Function that takes an array of size n and returns an array containing each
// step sorted using bubble sort Inputs: the original array, the number of
// elements in the array, the 2D array being returned, the number of swaps it
// takes to sort the array
void bubbleSort(int original_arr[], int num_size,
                int steps_arr[MAX_STEPS][num_size], int* step_count) {
  int inner_loop;
  int outer_loop;
  bool swapped;

  // First, copy the original array as the first array in the list
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Second, start the bubble sort process
  for (outer_loop = 0; outer_loop < num_size - 1; outer_loop++) {
    swapped = false;

    for (inner_loop = 0; inner_loop < num_size - outer_loop - 1; inner_loop++) {
      if (original_arr[inner_loop] > original_arr[inner_loop + 1]) {
        swap(&original_arr[inner_loop], &original_arr[inner_loop + 1]);
        swapped = true;

        // Copy the bubble sort step as a new array entry in the 2D array
        saveArray(original_arr, num_size, steps_arr, step_count);
      }
    }

    // If the swap didn't happen, return
    if (swapped == false) {
      break;
    }
  }
}

// INSERTION SORT ----------------------------------------------------------

// Function that takes an array of size n and returns an array containing each
// step sorted using insertion Inputs: the original array, the number of
// elements in the array, the 2D array being returned, the number of swaps it
// takes to sort the array
void insertionSort(int original_arr[], int num_size,
                   int steps_arr[MAX_STEPS][num_size], int* step_count) {
  int inner_loop;
  int outer_loop;
  int key;

  // First, copy the original array as the first array in the list
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Second, start the insertion sort process
  for (outer_loop = 1; outer_loop < num_size - 1; outer_loop++) {
    key = original_arr[outer_loop];
    inner_loop = outer_loop - 1;

    // Shift all elements to the right
    while (inner_loop >= 0 && original_arr[inner_loop] > key) {
      original_arr[inner_loop + 1] = original_arr[inner_loop];

      // Copy the bubble sort step as a new array entry in the 2D array after
      // each step
      saveArray(original_arr, num_size, steps_arr, step_count);

      inner_loop = inner_loop - 1;
    }

    // Insert the key in the correct position
    original_arr[inner_loop + 1] = key;

    // Copy the bubble sort step as a new array entry in the 2D array after each
    // insertion if it actually changed something
    if (inner_loop + 1 != outer_loop) {
      saveArray(original_arr, num_size, steps_arr, step_count);
    }
  }
}

// RADIX SORT ------------------------------

// QUICK SORT ------------------------------------------------------

void quickSort(int original_arr[], int num_size,
               int steps_arr[MAX_STEPS][num_size], int* step_count) {
  // First, copy the original array as the first array in the list
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Recursively sort the array
  quickSortRecursive(original_arr, 0, num_size - 1, num_size, steps_arr,
                     step_count);
}
