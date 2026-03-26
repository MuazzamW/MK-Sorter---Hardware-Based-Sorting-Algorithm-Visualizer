#ifndef ALGORITHMS_H
#define ALGORITHMS_H

void bubbleSort(int arr[], int n, int steps_arr[][n], int* step_count);
void insertionSort(int arr[], int n, int steps_arr[][n], int* step_count);
void radixSort(int arr[], int n, int steps_arr[][n], int* step_count);
void quickSort(int arr[], int n, int steps_arr[][n], int* step_count);
void swap(int* first, int* second);
void saveArray(int arr[], int n, int steps_arr[][n], int* step_count);
int quickSortPartition(int arr[], int low, int high, int n, int steps_arr[][n],
                       int* step_count);
void quickSortRecursive(int arr[], int low, int high, int n, int steps_arr[][n],
                        int* step_count);
int getMaxNum(int arr[], int n);
void countingSortByDigit(int arr[], int n, int exp);

#endif