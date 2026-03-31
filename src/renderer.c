// PROGRAM: DISPLAYS STARTING SCREEN

#include "renderer.h"
#include "interrupt_handler.h"
#include "address_map.h"
#include "IMAGES.h"
#include <stdbool.h>
#include <stdlib.h>

#define VGA_PIXEL_HEIGHT 240
#define VGA_PIXEL_WIDTH 320

volatile int* PIXEL_CTRL_PTR_1 = (int*)PIXEL_BUF_CTRL_BASE;
volatile short int* PIXEL_BUFFER_START_1;
short int BUFFER1[240][512];
short int BUFFER2[240][512];


short int COLORS[9] = {0xFFFF, 0x0000, 0xEF5D, 0xC618, 0x8D96,
                       0x8D91, 0x6B4D, 0xF800, 0x07E0};
char SMALL_CHAR[26][5] = {
    {2, 5, 7, 5, 5},  // A
    {6, 5, 6, 5, 6},  // B
    {3, 4, 4, 4, 3},  // C
    {6, 5, 5, 5, 6},  // D
    {7, 4, 6, 4, 7},  // E
    {7, 4, 6, 4, 4},  // F
    {3, 4, 5, 5, 3},  // G
    {5, 5, 7, 5, 5},  // H
    {7, 2, 2, 2, 7},  // I
    {1, 1, 1, 5, 2},  // J
    {5, 5, 6, 5, 5},  // K
    {4, 4, 4, 4, 7},  // L
    {5, 7, 7, 5, 5},  // M
    {5, 7, 7, 7, 5},  // N
    {2, 5, 5, 5, 2},  // O
    {6, 5, 6, 4, 4},  // P
    {2, 5, 5, 7, 3},  // Q
    {6, 5, 6, 5, 5},  // R
    {3, 4, 2, 1, 6},  // S
    {7, 2, 2, 2, 2},  // T
    {5, 5, 5, 5, 7},  // U
    {5, 5, 5, 5, 2},  // V
    {5, 5, 7, 7, 5},  // W
    {5, 5, 2, 5, 5},  // X
    {5, 5, 2, 2, 2},  // Y
    {7, 1, 2, 4, 7}   // Z
};
char MEDIUM_CHAR[26][7] = {
    {14, 17, 17, 31, 17, 17, 17},  // A
    {30, 17, 17, 30, 17, 17, 30},  // B
    {14, 17, 16, 16, 16, 17, 14},  // C
    {30, 17, 17, 17, 17, 17, 30},  // D
    {31, 16, 16, 30, 16, 16, 31},  // E
    {31, 16, 16, 30, 16, 16, 16},  // F
    {14, 17, 16, 16, 19, 17, 14},  // G
    {17, 17, 17, 31, 17, 17, 17},  // H
    {14, 4, 4, 4, 4, 4, 14},       // I
    {1, 1, 1, 1, 17, 17, 14},      // J
    {17, 18, 20, 24, 20, 18, 17},  // K
    {16, 16, 16, 16, 16, 16, 31},  // L
    {17, 27, 21, 17, 17, 17, 17},  // M
    {17, 25, 21, 19, 17, 17, 17},  // N
    {14, 17, 17, 17, 17, 17, 14},  // O
    {30, 17, 17, 30, 16, 16, 16},  // P
    {14, 17, 17, 17, 21, 18, 13},  // Q
    {30, 17, 17, 30, 20, 18, 17},  // R
    {15, 16, 16, 14, 1, 1, 30},    // S
    {31, 4, 4, 4, 4, 4, 4},        // T
    {17, 17, 17, 17, 17, 17, 14},  // U
    {17, 17, 17, 17, 17, 10, 4},   // V
    {17, 17, 17, 17, 21, 27, 17},  // W
    {17, 17, 10, 4, 10, 17, 17},   // X
    {17, 17, 10, 4, 4, 4, 4},      // Y
    {31, 1, 2, 4, 8, 16, 31}       // Z
};
char LARGE_CHAR[26][8] = {
    {24, 36, 66, 126, 66, 66, 66, 0},    // A
    {124, 66, 66, 124, 66, 66, 124, 0},  // B
    {60, 66, 64, 64, 64, 66, 60, 0},     // C
    {120, 68, 66, 66, 66, 68, 120, 0},   // D
    {126, 64, 64, 124, 64, 64, 126, 0},  // E
    {126, 64, 64, 124, 64, 64, 64, 0},   // F
    {60, 66, 64, 78, 66, 66, 60, 0},     // G
    {66, 66, 66, 126, 66, 66, 66, 0},    // H
    {60, 16, 16, 16, 16, 16, 60, 0},     // I
    {30, 4, 4, 4, 68, 68, 56, 0},        // J
    {66, 68, 72, 112, 72, 68, 66, 0},    // K
    {64, 64, 64, 64, 64, 64, 126, 0},    // L
    {66, 102, 90, 66, 66, 66, 66, 0},    // M
    {66, 98, 82, 74, 70, 66, 66, 0},     // N
    {60, 66, 66, 66, 66, 66, 60, 0},     // O
    {124, 66, 66, 124, 64, 64, 64, 0},   // P
    {60, 66, 66, 66, 74, 68, 58, 0},     // Q
    {124, 66, 66, 124, 72, 68, 66, 0},   // R
    {60, 64, 64, 60, 2, 2, 124, 0},      // S
    {126, 16, 16, 16, 16, 16, 16, 0},    // T
    {66, 66, 66, 66, 66, 66, 60, 0},     // U
    {66, 66, 66, 66, 66, 36, 24, 0},     // V
    {66, 66, 66, 90, 90, 102, 66, 0},    // W
    {66, 36, 24, 24, 24, 36, 66, 0},     // X
    {66, 66, 36, 24, 16, 16, 16, 0},     // Y
    {126, 4, 8, 16, 32, 64, 126, 0}      // Z
};

//general UI ELEMENTS
GENERAL_ELEMENT* UI_ELEMENTS[15];

int RENDERING_LAYER = 1; //layer to keep count of which ui elements to draw over others
int UI_ELEMENT_COUNT = 0; //total count of ui elements drawn

//ELEMENTS
static buttonElement bubbleSortButton;
static buttonElement insertionSortButton; 
static buttonElement radixSortButton; 
static buttonElement quickSortButton;
static buttonElement resetButton; 
static buttonElement goButton; 


//HELPER FUNCTIONS
short darkenColor(short color) {
    int r = (color >> 11) & 0x1F;
    int g = (color >> 5)  & 0x3F;
    int b = color & 0x1F;

    r = (r * 7) / 10;  // ~0.7
    g = (g * 7) / 10;
    b = (b * 7) / 10;

    return (r << 11) | (g << 5) | b;
}

// IMPLEMENTATIONS -----------------------------------------------------

void drawCursor(int xCoord, int yCoord) {
    for (int y = 0; y < KARINA_CURSOR_HEIGHT; y++) {
        for (int x = 0; x < KARINA_CURSOR_WIDTH; x++) {
          if(KARINA_CURSOR[y*KARINA_CURSOR_WIDTH+x]==0x0000){continue;}
            plotPixel(xCoord + x, yCoord + y,
                      KARINA_CURSOR[y * KARINA_CURSOR_WIDTH + x]);
        }
    }
}

void initializeBuffers(void){

  PIXEL_BUFFER_START_1 = (volatile short int*)(*PIXEL_CTRL_PTR_1);

  *(PIXEL_CTRL_PTR_1 + 1) =
      (int)&BUFFER1;  // first store the address in the  back buffer
  waitForSync();
  PIXEL_BUFFER_START_1 = (volatile short int*)(*PIXEL_CTRL_PTR_1);
  clearScreen();
  *(PIXEL_CTRL_PTR_1 + 1) = (int)&BUFFER2;
  PIXEL_BUFFER_START_1 = (volatile short int*)(*(PIXEL_CTRL_PTR_1 + 1));
  clearScreen();

  //change for onnly buttons

  UI_ELEMENT_COUNT = 0;
  RENDERING_LAYER = 1;


  //INITIALIZE ALL THE BUTTON ELEMENTS
  bubbleSortButton = (buttonElement){
    .action = BUBBLE_SORT,
    .text = "BUBBLE SORT",
    .isClicked = false,
    .isHover = false,
    .parent = {
      .backgroundColour = COLORS[6], .borderColor = COLORS[1],
        .boundary = {.topLeft = {3,35}, .bottomRight = {42,60}},
      .layer = RENDERING_LAYER,
      .clickable = true,
      .type = BUTTON
    }};

  UI_ELEMENTS[UI_ELEMENT_COUNT++] = (GENERAL_ELEMENT*)&bubbleSortButton;

  insertionSortButton = (buttonElement){
    .action = INSERTION_SORT,
    .isClicked = false,
    .isHover = false,
    .parent = {
      .backgroundColour = COLORS[6], .borderColor = COLORS[1],
        .boundary = {.topLeft = {3,63}, .bottomRight = {42,87}},
      .layer = RENDERING_LAYER,
      .clickable = true,
      .type = BUTTON
    }};

  UI_ELEMENTS[UI_ELEMENT_COUNT++] = (GENERAL_ELEMENT*)&insertionSortButton;

  radixSortButton = (buttonElement){
    .action = RADIX_SORT,
    .isClicked = false,
    .isHover = false,
    .parent = {
      .backgroundColour = COLORS[6], .borderColor = COLORS[1],
        .boundary = {.topLeft = {3,90}, .bottomRight = {42,115}},
      .layer = RENDERING_LAYER,
      .clickable = true,
      .type = BUTTON
    }};

  UI_ELEMENTS[UI_ELEMENT_COUNT++] = (GENERAL_ELEMENT*)&radixSortButton;

  quickSortButton = (buttonElement){
    .action = QUICK_SORT,
    .isClicked = false,
    .isHover = false,
    .parent = {
      .backgroundColour = COLORS[6], .borderColor = COLORS[1],
        .boundary = {.topLeft = {3,118}, .bottomRight = {42,143}},
      .layer = RENDERING_LAYER,
      .clickable = true,
      .type = BUTTON
    }};

  UI_ELEMENTS[UI_ELEMENT_COUNT++] = (GENERAL_ELEMENT*)&quickSortButton;

  resetButton = (buttonElement){
    .action = RESET,
    .isClicked = false,
    .isHover = false,
    .parent = {
      .backgroundColour = COLORS[7], .borderColor = COLORS[1],
        .boundary = {.topLeft = {3,182}, .bottomRight = {42,207}},
      .layer = RENDERING_LAYER,
      .clickable = true,
      .type = BUTTON
    }};

  UI_ELEMENTS[UI_ELEMENT_COUNT++] = (GENERAL_ELEMENT*)&resetButton;

  goButton = (buttonElement){
    .action = GO,
    .isClicked = false,
    .isHover = false,
    .parent = {
      .backgroundColour = COLORS[8], .borderColor = COLORS[1],
        .boundary = {.topLeft = {3,210}, .bottomRight = {42,235}},
      .layer = RENDERING_LAYER,
      .layer = RENDERING_LAYER,
      .clickable = true,
      .type = BUTTON
    }};

  UI_ELEMENTS[UI_ELEMENT_COUNT++] = (GENERAL_ELEMENT*)&goButton;
  

}


// Function that draws the steps of the 2D array passed to it (the animation
// part of the whole program)
void drawSortSteps(int arr[], int n, int steps_arr[][n], int step_count,
                   volatile int* SW_ptr) {
  // Define starting x-values and spacing between rectangles
  int start_x = 50;
  int max_x = 319;
  int spacing = 2;
  int available_width = max_x - start_x;
  int total_spacing = (n - 1) * spacing;
  int dx = (available_width - total_spacing) / n;
  if (dx < 1) dx = 1;
  int sw;
  int current_x;
  int currentState = 0;
  arrayStep = 0;
  int arrayStepPrev = 0;
  currentlyDisplaying = true;

  currentlyDisplaying = true;
  while(currentState<step_count){
          // Check if the RESET button was pressed
      sw = *SW_ptr;
      if (sw & (1 << 4)) {
        printf("Animation interrupted by RESET\n");
        currentlyDisplaying = false;
        arrayStep = 0;
        return;
      }
      if(arrayStepPrev!=arrayStep){
        currentState++;
        if (currentState >= step_count) {
          break;
        }
        arrayStepPrev = arrayStep;
      }

      current_x = start_x;

      clearBackground();
      drawBackground();

      // Loop through each rectangle in the current step and display the
      // rectangles
      for (int rect = 0; rect < n; rect++) {
        int value = steps_arr[currentState][rect];

        // Every color of rectangle is green
        short int color = COLORS[5];

        // Rectangle that is changing is in red
        if (currentState > 0 && steps_arr[currentState][rect] != steps_arr[currentState - 1][rect]) {
          color = COLORS[7];
        }

        drawRectangle(current_x, value, current_x + dx, 239, color);
        drawBorder(current_x, value, current_x + dx, 239, COLORS[1]);

        current_x += dx + spacing;
      }

      waitForSync();
      PIXEL_BUFFER_START_1 = (volatile short int*)(*(PIXEL_CTRL_PTR_1 + 1));      // Check if the RESET button was pressed
  }
  currentlyDisplaying = false;
  arrayStep = 0;
}

void drawBackground() {
  // Top Panel
  drawRectangle(0, 0, 319, 30, COLORS[4]);
  drawBorder(0, 0, 319, 30, COLORS[1]);

  // Side Panel
  drawRectangle(0, 32, 45, 239, COLORS[3]);
  drawBorder(0, 32, 45, 239, COLORS[1]);

  //LOOP TO DRAW BUTTONS

  for(int i = 0; i < RENDERING_LAYER; i++){
    for(int buttons = 0; buttons < UI_ELEMENT_COUNT; buttons++){
      GENERAL_ELEMENT* element = UI_ELEMENTS[buttons];
      //printf("%d %d\n",element->boundary.topLeft.y,element->boundary.bottomRight.y);
      if(element->type == BUTTON){
        buttonElement* button = (buttonElement*)element;
        bounds bound = button->parent.boundary;
        if(button->isHover || button->isClicked){
          //printf("darkened button \n");
          short int darkenedColour = darkenColor(button->parent.backgroundColour);
          drawRectangle(bound.topLeft.x,bound.topLeft.y,bound.bottomRight.x,bound.bottomRight.y, darkenedColour);
        }else{
          drawRectangle(bound.topLeft.x,bound.topLeft.y,bound.bottomRight.x,bound.bottomRight.y, button->parent.backgroundColour);
        }
        drawBorder(bound.topLeft.x,bound.topLeft.y,bound.bottomRight.x,bound.bottomRight.y, button->parent.borderColor);
      }
    }
  }

  // Draw text on the screen
  drawLargeText(130, 14, "MK SORTER", COLORS[1]);
  drawSmallText(270, 20, "ECE PROJECT", COLORS[1]);
  drawSmallText(5, 20, "MUAZZAM W AND KARINA K", COLORS[1]);
  drawSmallText(7, 40, "BUBBLE", COLORS[0]);
  drawSmallText(7, 50, "SORT", COLORS[0]);
  drawSmallText(5, 68, "INSERTION", COLORS[0]);
  drawSmallText(5, 78, "SORT", COLORS[0]);
  drawSmallText(7, 95, "RADIX", COLORS[0]);
  drawSmallText(7, 105, "SORT", COLORS[0]);
  drawSmallText(7, 123, "QUICK", COLORS[0]);
  drawSmallText(7, 133, "SORT", COLORS[0]);
  drawMediumText(9, 192, "RESET", COLORS[0]);
  drawMediumText(17, 220, "GO", COLORS[0]);
}

void drawResetScreen() {
  int starting_x = 50;
  int dx = 17;

  // Hardcoded "random-looking" values (range 50–170)
  int heights[14] = {120, 75, 160, 90, 140, 60, 155,
                     110, 80, 170, 95, 130, 70, 150};

  // Loop through each rectangle and draw it on screen
  for (int rect_num = 0; rect_num < 14; rect_num++) {
    int y = heights[rect_num];

    drawRectangle(starting_x, y, starting_x + dx, 239, COLORS[5]);
    drawBorder(starting_x, y, starting_x + dx, 239, COLORS[1]);

    starting_x += dx + 2;
  }
}

//Function that draws white pixels on every pixel to "clear" the screen
void clearScreen() {
  volatile short int* grid_pointer;

    for(int y = 0; y<VGA_PIXEL_HEIGHT;y++){
        grid_pointer = (volatile short int *)(PIXEL_BUFFER_START_1 + (y << 9)); //reset to next row
        for(int x = 0; x < VGA_PIXEL_WIDTH; x++){
            *grid_pointer = COLORS[2];
            grid_pointer++;
        }
    }
}

//Function that draws the background color only on the background portion of
//the screen
void clearBackground() {
  volatile short int* grid_pointer;

  for(int y = 30; y<VGA_PIXEL_HEIGHT;y++){
        grid_pointer = (volatile short int *)(PIXEL_BUFFER_START_1 + (y << 9)) + 45; //reset to next row and start at x=45
        for(int x = 45; x < VGA_PIXEL_WIDTH; x++){
            *grid_pointer = COLORS[2];
            grid_pointer++;
        }
    }
}

// Function that plots one pixel in a given color
void plotPixel(int x, int y, short int line_color) {
  volatile short int* one_pixel_address;
  one_pixel_address = PIXEL_BUFFER_START_1 + (y << 9) + x;
  *one_pixel_address = line_color;
}

// Function that draws a line from (x1, y1) to (x2, y2)
void drawLine(int x1, int y1, int x2, int y2, short int line_color) {
  // First check how steep the line is
  bool is_steep = false;
  if (abs(y2 - y1) > abs(x2 - x1)) {
    is_steep = true;
  }

  // Swap the x and y variables if the line is steep
  if (is_steep) {
    int temp = x1;
    x1 = y1;
    y1 = temp;

    temp = x2;
    x2 = y2;
    y2 = temp;
  }

  if (x1 > x2) {
    int temp = x1;
    x1 = x2;
    x2 = temp;

    temp = y1;
    y1 = y2;
    y2 = temp;
  }

  // Calculate the horizontal and vertical distances the line is being drawn
  int delta_x = x2 - x1;
  int delta_y = abs(y2 - y1);

  // Error variable to keep track of when the y-coordinate should change
  int error = -(delta_x / 2);
  int y = y1;
  int y_step;

  // Is the line pointing up or down
  if (y1 < y2) {
    y_step = 1;
  } else {
    y_step = -1;
  }

  // Loop through each x coordinate of the line
  for (int x = x1; x <= x2; x++) {
    // If the line is steep, switch the x and y coordinates
    if (is_steep == true) {
      plotPixel(y, x, line_color);
    } else {
      plotPixel(x, y, line_color);
    }
    // Update the error
    error = error + delta_y;
    // If the error becomes too large then we are too far from the line
    // Shift the y coordinate occasionally
    if (error > 0) {
      y = y + y_step;
      error = error - delta_x;
    }
  }
}
// Function that draws a rectangle
void drawRectangle(int x1, int y1, int x2, int y2, short int line_color) {
  // Draw a horizontal line for each y-value
  for (int y = y1; y <= y2; y++) {
    drawLine(x1, y, x2, y, line_color);
  }
}

// Function that draws a border (empty rectangle)
void drawBorder(int x1, int y1, int x2, int y2, short int line_color) {
  drawLine(x1, y1, x2, y1, line_color);
  drawLine(x2, y1, x2, y2, line_color);
  drawLine(x1, y1, x1, y2, line_color);
  drawLine(x1, y2, x2, y2, line_color);
}

void drawSmallChar(int x, int y, char c, short int color) {
  if (c < 'A' || c > 'Z') return;
  int index = c - 'A';
  for (int row = 0; row < 5; row++) {
    char bits = SMALL_CHAR[index][row];
    for (int col = 0; col < 3; col++) {
      if (bits & (1 << (2 - col))) {
        plotPixel(x + col, y + row, color);
      }
    }
  }
}

void drawSmallText(int x, int y, char* text, short int color) {
  while (*text) {
    drawSmallChar(x, y, *text, color);
    x += 4;
    text++;
  }
}

void drawMediumChar(int x, int y, char c, short int color) {
  if (c < 'A' || c > 'Z') return;
  int index = c - 'A';
  for (int row = 0; row < 7; row++) {
    char bits = MEDIUM_CHAR[index][row];
    for (int col = 0; col < 5; col++) {
      if (bits & (1 << (4 - col))) {
        plotPixel(x + col, y + row, color);
      }
    }
  }
}

void drawMediumText(int x, int y, char* text, short int color) {
  while (*text) {
    drawMediumChar(x, y, *text, color);
    x += 6;
    text++;
  }
}

void drawLargeChar(int x, int y, char c, short int color) {
  if (c < 'A' || c > 'Z') return;
  int index = c - 'A';
  for (int row = 0; row < 8; row++) {
    unsigned char bits = LARGE_CHAR[index][row];
    for (int col = 0; col < 8; col++) {
      if (bits & (1 << (7 - col))) {
        plotPixel(x + col, y + row, color);
      }
    }
  }
}

void drawLargeText(int x, int y, char* text, short int color) {
  while (*text) {
    drawLargeChar(x, y, *text, color);
    x += 9;
    text++;
  }
}

// Function that swaps the front buffer with the back
void waitForSync() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
  int status;
  // Initalize the swap cycle - writing 1 into the front buffer
  *pixel_ctrl_ptr = 1;
  status = *(pixel_ctrl_ptr + 3);  // Address of the status register
  // Poll the STATUS bit
  while ((status & 0x01) != 0) {
    status = *(pixel_ctrl_ptr + 3);
    // Exits the loop when STATUS = 0
  }

  PIXEL_BUFFER_START_1 = (volatile short int*)(*(PIXEL_CTRL_PTR_1+1));

}