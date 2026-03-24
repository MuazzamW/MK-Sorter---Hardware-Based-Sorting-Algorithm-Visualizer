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
int getMaxNum(int arr[], int n);
void countingSortByDigit(int arr[], int n, int exp);

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

// Function that takes a section of the array (low to high) and picks the last
// element as the pivot point Returns the index of the pivot point after
// everything < pivot is on the left and everything >= pivot is on the right
int quickSortPartition(int arr[], int low, int high, int n,
                       int steps_arr[MAX_STEPS][n], int* step_count) {
  // Last element of the array is the pivot point
  int pivot = arr[high];

  // Store the index of everything less than pivot point
  int i = low - 1;

  // Scrans through the array and stops before the pivot point
  for (int j = low; j < high; j++) {
    // If the current element is smaller than pivot, it belongs to left
    // partition
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

// Quick sort recursive function that takes a portion of the array (low to high)
// and partions the array into two separate arrays around the pivot point
// Then it recursively sorts the left and right side
void quickSortRecursive(int arr[], int low, int high, int n,
                        int steps_arr[MAX_STEPS][n], int* step_count) {
  // Base condition
  if (low < high) {
    // Rearranges the subarray and places the pivot in its final position
    int pivot_index =
        quickSortPartition(arr, low, high, n, steps_arr, step_count);

    // At this point everything left of pivot_index is < pivot_index and
    // everything right is >= pivot_index Now we recursively sort the left and
    // right side
    quickSortRecursive(arr, low, pivot_index - 1, n, steps_arr, step_count);
    quickSortRecursive(arr, pivot_index + 1, high, n, steps_arr, step_count);
  }
}

// Function that returns the maximum number in the array
int getMaxNum(int arr[], int n) {
  int max_num = arr[0];

  // Loop through the array and update only the largest number
  for (int element = 0; element < n; element++) {
    if (arr[element] > max_num) {
      max_num = arr[element];
    }
  }

  return max_num;
}

void countingSortByDigit(int arr[], int n, int exp) {
  int output_arr[n];
  int digits_arr[10] = {0};

  // Count the occurrences of digits
  for (int element = 0; element < n; element++) {
    int digit = (arr[element] / exp) % 10;
    digits_arr[digit] = digits_arr[digit] + 1;
  }

  // Convert the prefix sum
  for (int index = 1; index < 10; index++) {
    digits_arr[index] = digits_arr[index] + digits_arr[index - 1];
  }

  // Build the ouptut array
  for (int element = n - 1; element >= 0; element--) {
    int digit = (arr[element] / exp) % 10;
    output_arr[digits_arr[digit] - 1] = arr[element];
    digits_arr[digit] = digits_arr[digit] - 1;
  }

  // Copy back to the original array
  for (int element = 0; element < n; element++) {
    arr[element] = output_arr[element];
  }
}

// BUBBLE SORT -------------------------------------------------------------

// Function that takes an array of size n and returns an array containing each
// step sorted using bubble sort
// Inputs: the original array, the number of elements in the array, the 2D array
// being returned, the number of swaps it takes to sort the array
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
// step sorted using insertion
// Inputs: the original array, the number of elements in the array, the 2D array
// being returned, the number of swaps it takes to sort the array
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

// RADIX SORT --------------------------------------------------------

// Function that takes an array of size n and returns an array containing each
// step sorted using raxdix sort
// Inputs: the original array, the number of elements in the array, the 2D array
// being returned, the number of swaps it takes to sort the array
void radixSort(int original_arr[], int num_size,
               int steps_arr[MAX_STEPS][num_size], int* step_count) {
  // Copy the original array
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Get the largest value in the array
  int max = getMaxNum(original_arr, num_size);

  // Use counting sort for each digit
  for (int exp = 1; max / exp > 0; exp *= 10) {
    countingSortByDigit(original_arr, num_size, exp);
    saveArray(original_arr, num_size, steps_arr, step_count);
  }
}

// QUICK SORT ---------------------------------------------------------

// Function that takes an array of size n and returns an array containing each
// step sorted using quick sort
// Inputs: the original array, the number of elements in the array, the 2D array
// being returned, the number of swaps it takes to sort the array
void quickSort(int original_arr[], int num_size,
               int steps_arr[MAX_STEPS][num_size], int* step_count) {
  // First, copy the original array as the first array in the list
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Call the recurive helper function to sort the array
  quickSortRecursive(original_arr, 0, num_size - 1, num_size, steps_arr,
                     step_count);
}
