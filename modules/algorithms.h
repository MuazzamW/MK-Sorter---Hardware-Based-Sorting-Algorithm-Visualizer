#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#define MAX_STEPS 1000
#define MAX_SIZE 1000

void bubbleSort(int arr[], int n, int steps_arr[MAX_STEPS][MAX_SIZE],
                int* step_count);
void insertionSort(int arr[], int n, int steps_arr[MAX_STEPS][MAX_SIZE],
                   int* step_count);
void radixSort(int arr[], int n, int steps_arr[MAX_STEPS][MAX_SIZE],
               int* step_count);
void quickSort(int arr[], int n, int steps_arr[MAX_STEPS][MAX_SIZE],
               int* step_count);

#endif