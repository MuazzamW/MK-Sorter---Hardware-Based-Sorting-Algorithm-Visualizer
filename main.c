#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "address_map.h"
#include "algorithms.h"
#include "interrupt_handler.h"
#include "mouse_interface.h"
#include "renderer.h"

#define MAX_SIZE 1000

// GLOBALS -------------------------------------------------------

volatile short int* PIXEL_BUFFER_START;
volatile int* PIXEL_CTRL_PTR;
volatile int* SW_PTR = (int*)0xFF200040;
volatile int* KEY_PTR = (int*)0xFF200050;
short int BUFFER1[240][512];
short int BUFFER2[240][512];

// MAIN ----------------------------------------------------------

int main(void) {
  PIXEL_CTRL_PTR = (int*)0xFF203020;
  *(PIXEL_CTRL_PTR + 1) =
      (int)&BUFFER1;  // first store the address in the  back buffer
  waitForSync();
  PIXEL_BUFFER_START = (volatile short int*)(*PIXEL_CTRL_PTR);
  clearScreen();
  *(PIXEL_CTRL_PTR + 1) = (int)&BUFFER2;
  PIXEL_BUFFER_START = (volatile short int*)(*(PIXEL_CTRL_PTR + 1));

  clearScreen();
  drawBackground();
  drawResetScreen();
  waitForSync();
  PIXEL_BUFFER_START = (volatile short int*)(*(PIXEL_CTRL_PTR + 1));

  int prev_sw = 0;
  int selected_sort = -1;
  int n =
      25;  // default number of rectangles selected if user forgets to choose
  bool ready_to_run = false;
  int arr[MAX_SIZE];
  int steps_arr[MAX_SIZE][MAX_SIZE];
  int step_count = 0;

  while (1) {
    int curr_sw = *SW_PTR;
    int sw_changed = curr_sw & (~prev_sw);

    if (sw_changed & (1 << 0)) {
      selected_sort = 0;
      printf("Bubble Sort selected\n");
    }
    if (sw_changed & (1 << 1)) {
      selected_sort = 1;
      printf("Insertion Sort selected\n");
    }
    if (sw_changed & (1 << 2)) {
      selected_sort = 2;
      printf("Radix Sort selected\n");
    }
    if (sw_changed & (1 << 3)) {
      selected_sort = 3;
      printf("Quick Sort selected\n");
    }

    if (sw_changed & (1 << 4)) {
      printf("System Reset\n");

      // reset state
      selected_sort = -1;
      n = 25;  // default value
      ready_to_run = false;

      // redraw screen immediately
      clearScreen();
      drawBackground();
      drawResetScreen();

      waitForSync();
      PIXEL_BUFFER_START = (volatile short int*)(*(PIXEL_CTRL_PTR + 1));

      continue;
    }

    if (sw_changed & (1 << 5)) {
      // GO
      if (selected_sort != -1 && n != 0) {
        ready_to_run = true;
        printf("GO pressed\n");
      } else {
        printf("Select sort and n first\n");
      }
    }

    prev_sw = curr_sw;
    int edgeCap = *(KEY_PTR + 3);

    // KEY 0 -> N = 10
    if ((edgeCap & 0b0001) != 0) {
      *(KEY_PTR + 3) = 0b1111;
      printf("n = 10\n");
      n = 10;
    }

    // KEY 1 -> N = 50
    if ((edgeCap & 0b0010) != 0) {
      *(KEY_PTR + 3) = 0b1111;
      printf("n = 25\n");
      n = 25;
    }

    // KEY 2 -> N = 100
    if ((edgeCap & 0b0100) != 0) {
      *(KEY_PTR + 3) = 0b1111;
      printf("n = 50\n");
      n = 50;
    }

    // KEY 3 -> N = 150
    if ((edgeCap & 0b1000) != 0) {
      *(KEY_PTR + 3) = 0b1111;
      printf("n = 60\n");
      n = 66;
    }

    if (ready_to_run == true) {
      printf("Running sort...\n");

      // Example: initialize array (replace later with random)
      for (int rect_num = 0; rect_num < n; rect_num++) {
        int randomY = rand() % 120 + 50;
        arr[rect_num] = randomY;
      }

      step_count = 0;

      switch (selected_sort) {
        case 0:
          bubbleSort(arr, n, steps_arr, &step_count);
          break;
        case 1:
          insertionSort(arr, n, steps_arr, &step_count);
          break;
        case 2:
          radixSort(arr, n, steps_arr, &step_count);
          break;
        case 3:
          quickSort(arr, n, steps_arr, &step_count);
          break;
      }

      drawSortSteps(arr, n, steps_arr, step_count, SW_PTR);
      ready_to_run = false;
      n = 25;
      step_count = 0;
    }
  }

  return 0;
}
