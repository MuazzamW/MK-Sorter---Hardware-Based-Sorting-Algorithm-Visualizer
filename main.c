#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/address_map.h"
#include "src/renderer.h"
#include "src/IMAGES.h"
#include "src/interrupt_handler.h"
#include "src/algorithms.h"
#include "src/UI_ELEMENTS.h"
#include "src/APP_STATE.h"
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

PROGRAM_STATE currentState = STARTING_SCREEN;
int selected_sort = -1;
int numButtonsSelected = 0;

int prev_sw = 0;
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
    if(currentState == STARTING_SCREEN){

      mouseInfo = get_mouse_packet();
      bool clicked_now = mouseInfo.leftButtonClicked && !prevLeft;

      updateAllButtons(&mouseInfo);
      buttonElement* clickedButton = getClickedButton(&mouseInfo, clicked_now);
      if(clickedButton){
        //printf("button state is %d\n", clickedButton->action);
        if(clickedButton->onClick){
          clickedButton->onClick(clickedButton);
          continue;
        }
      }

      prevLeft = mouseInfo.leftButtonClicked;
      clearBackground();
      drawStartScreen();
      drawCursor(mouseInfo.x, mouseInfo.y);
      waitForSync();


    }else if(currentState == MAIN_SCREEN){
      //Logic to check if any buttons are clicked and if the program is ready to display an algorithm
      mouseInfo = get_mouse_packet();

      bool clicked_now = mouseInfo.leftButtonClicked && !prevLeft;
      updateAllButtons(&mouseInfo);
      buttonElement* clickedButton = getClickedButton(&mouseInfo, clicked_now);
      if(clickedButton){
        switch(clickedButton->action){
          case BUBBLE_SORT:
          case INSERTION_SORT:
          case RADIX_SORT:
          case QUICK_SORT:
            clearSortSelections();
            // clickedButton->isClicked = true;
            // clickedButton->isSelected = true;
            break;
          default:
            break;
        }

        if(clickedButton->onClick){
          clickedButton->onClick(clickedButton);
        }

      }

      char barSelectionText[20];
      // KEY 0 -> N = 10
      if (keyNumber == 1) {
        //printf("n = 10\n");
        n = 10;
      }else if(keyNumber == 2){
        //printf("n = 25\n");
        n = 25;
      }else if(keyNumber == 4){
        //printf("n = 50\n"); 
        n = 50;
      }else if(keyNumber == 8){
        //printf("n = 60\n");
        n = 66;
      }

      sprintf(barSelectionText,"N = %d",n);

      prevLeft = mouseInfo.leftButtonClicked;
      clearBackground();
      drawBackground();
      drawResetScreen();
      drawSmallText(3,150,barSelectionText,0x0000);

      drawCursor(mouseInfo.x,mouseInfo.y);
      waitForSync();
      
    }else if(currentState == RESET_STATE){
        // RESET_STATE state
        selected_sort = -1;
        n = 25;  // default value
        step_count = 0;
        ready_to_run = false;
        sortSelected = false;
        currentState = MAIN_SCREEN;
        //ensure no buttons are highlighted or clicked when reset
        for(int element = 0; element < UI_ELEMENT_COUNT; element++){
          if(UI_ELEMENTS[element]->type == BUTTON){
            buttonElement* button = (buttonElement*)UI_ELEMENTS[element];
            button->isHover = false;
            button->isClicked = false;
            button->isSelected = false;
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
      
    }
  }

  return 0;
}