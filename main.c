#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/address_map.h"
#include "src/renderer.h"
#include "src/IMAGES.h"
#include "src/interrupt_handler.h"
#include "src/algorithms.h"
#include "src/UI_ELEMENTS.h"
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
  RESET_STATE 
};

enum PROGRAM_STATE currentState = MAIN_SCREEN;

int prev_sw = 0;
int selected_sort = -1;
int n = 25;  // default number of rectangles selected if user forgets to choose
bool ready_to_run = false;
bool sortSelected = false;
bool prevLeft = false;
int step_count = 0;

// MAIN ----------------------------------------------------------

int main(void) {
  mouse_packet mouseInfo;
  set_up_interrupt_handler();
  initializeBuffers();

  while(1){
    if(currentState == MAIN_SCREEN){
      //Logic to check if any buttons are clicked and if the program is ready to display an algorithm
      mouseInfo = get_mouse_packet();

      bool clicked_now = mouseInfo.leftButtonClicked && !prevLeft;

      //update state of buttons
      for (int elementIdx = 0; elementIdx < UI_ELEMENT_COUNT; elementIdx++) {

        GENERAL_ELEMENT* element = UI_ELEMENTS[elementIdx];
        if (element->type != BUTTON) continue;

        buttonElement* button = (buttonElement*)element;
        //check if the button is being hovered over
        bool hovered = mouseHover(mouseInfo.x, mouseInfo.y, element);
        button->isHover = hovered;

        //check if button click was to reset or to initiate GO State
        if (hovered && clicked_now) {
          if (button->action == RESET) {
            currentState = RESET_STATE;
          } else if (button->action == GO) {
            if (selected_sort != -1) {
                //ready_to_run = true;
                currentState = DISPLAYING;
            }
          } else {

            //if neither the GO button nor RESET was selected, then a sorting algorithm button was selected
            // sort button clicked: clear all other sort selections
            for (int i = 0; i < UI_ELEMENT_COUNT; i++) {
                //this logic ensures only one button is clicked at a time
                GENERAL_ELEMENT* e2 = UI_ELEMENTS[i];
                if (e2->type != BUTTON) continue;
                buttonElement* b2 = (buttonElement*)e2;
                if (b2->action != GO && b2->action != RESET) {
                    b2->isClicked = false;
                }
            }

            button->isClicked = true;
            selected_sort = button->action;
            sortSelected = true;
          }
        }
      }
      prevLeft = mouseInfo.leftButtonClicked;
      clearBackground();
      drawBackground();
      drawResetScreen();
      drawCursor(mouseInfo.x,mouseInfo.y);
      waitForSync();

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
    }else if(currentState == RESET_STATE){
        // RESET_STATE state
        selected_sort = -1;
        n = 25;  // default value
        ready_to_run = false;
        sortSelected = false;
        currentState = MAIN_SCREEN;
        //ensure no buttons are highlighted or clicked when reset
        for(int element = 0; element < UI_ELEMENT_COUNT; element++){
          if(UI_ELEMENTS[element]->type == BUTTON){
            buttonElement* button = (buttonElement*)UI_ELEMENTS[element];
            button->isHover = false;
            button->isClicked = false;
          }
        }
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
      currentState = RESET_STATE;
    }
  }

  return 0;
}