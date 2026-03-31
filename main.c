#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/address_map.h"
#include "src/renderer.h"
#include "src/IMAGES.h"
#include "src/interrupt_handler.h"
#include "src/algorithms.h"

// Stripped local include: #include "src/address_map.h"
// Stripped local include: #include "src/algorithms.h"
// Stripped local include: #include "src/interrupt_handler.h"
//#include "mouse_interface.h"
// Stripped local include: #include "src/renderer.h"

#define MAX_SIZE 1000

// GLOBALS -------------------------------------------------------

// volatile short int* PIXEL_BUFFER_START_1;
// volatile int* PIXEL_CTRL_PTR_1;
volatile int* SW_PTR = (int*)SW_BASE;
volatile int* KEY_PTR = (int*)KEY_BASE;
static int arr[MAX_SIZE];
static int steps_arr[MAX_SIZE][MAX_SIZE];

enum PROGRAM_STATE { 
  START_SCREEN,
  MAIN_SCREEN,
  DISPLAYING,
  RESET 
};

enum PROGRAM_STATE currentState;

// MAIN ----------------------------------------------------------

int main(void) {
  mouse_packet mouseInfo;
  set_up_interrupt_handler();
  initializeBuffers();
  int prev_sw = 0;
  int selected_sort = -1;
  int n = 25;  // default number of rectangles selected if user forgets to choose
  bool ready_to_run = false;
  int step_count = 0;
  currentState = MAIN_SCREEN;

  while(1){
    if(currentState == MAIN_SCREEN){
      clearBackground();
      drawBackground();
      drawResetScreen();
      mouseInfo = get_mouse_packet();
      drawCursor(mouseInfo.x,mouseInfo.y);
      waitForSync();

      //check to update for next state
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
        currentState = RESET;
        printf("System Reset\n");
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

      if(ready_to_run){
        currentState = DISPLAYING;
      }
    }else if(currentState == RESET){
        // reset state
        selected_sort = -1;
        n = 25;  // default value
        ready_to_run = false;
        currentState = MAIN_SCREEN;
        continue;
    }else if(currentState == DISPLAYING){
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
      currentState = RESET;
    }
  }

  return 0;
}