// --- Start of src/algorithms.h ---
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
// --- End of src/algorithms.h ---

// --- Start of src/interrupt_handler.h ---
#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

// Stripped local include: #include "address_map.h"
#include <stdbool.h>
#include <stdio.h>

// extern volatile struct mouse_coordinates mouse_location; 
// extern volatile bool mouseClicked;

typedef struct {
    volatile double dx;
    volatile double dy;
    volatile char flags;
    volatile bool leftButtonClicked;
    volatile bool rightButtonClicked;

    volatile double x;
    volatile double y;

}mouse_packet;

int set_up_interrupt_handler(void);
mouse_packet get_mouse_packet(void);

extern volatile int arrayStep;
extern volatile bool currentlyDisplaying;

#endif

// --- End of src/interrupt_handler.h ---

// --- Start of src/renderer.h ---
#ifndef RENDERER_H
#define RENDERER_H

void clearScreen();
void plotPixel(int x, int y, short int line_color);
void drawLine(int x1, int y1, int x2, int y2, short int line_color);
void drawRectangle(int x1, int y1, int x2, int y2, short int line_color);
void drawBorder(int x1, int y1, int x2, int y2, short int line_color);
void drawSmallChar(int x, int y, char c, short int color);
void drawSmallText(int x, int y, char* text, short int color);
void drawMediumChar(int x, int y, char c, short int color);
void drawMediumText(int x, int y, char* text, short int color);
void drawLargeChar(int x, int y, char c, short int color);
void drawLargeText(int x, int y, char* text, short int color);
void drawBackground();
void drawResetScreen();
void clearBackground();
void waitForSync();
void drawSortSteps(int arr[], int n, int steps_arr[][n], int step_count,
                   volatile int* SW_ptr);

void initializeBuffers(void);

#endif
// --- End of src/renderer.h ---

// --- Start of src/address_map.h ---
/*******************************************************************************
 * This file provides address values that exist in the DE1-SoC Computer
 ******************************************************************************/

#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__

#define BOARD				"DE1-SoC"

/* Memory */
#define DDR_BASE			0x40000000
#define DDR_END				0x7FFFFFFF
#define A9_ONCHIP_BASE			0xFFFF0000
#define A9_ONCHIP_END			0xFFFFFFFF
#define SDRAM_BASE			0x00000000
#define SDRAM_END			0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x0803FFFF
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE			0xFF200000
#define LEDR_BASE			0xFF200000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE				0xFF200040
#define KEY_BASE			0xFF200050
#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define JTAG_UART_BASE			0xFF201000
#define IrDA_BASE			0xFF201020
#define TIMER_BASE			0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define RGB_RESAMPLER_BASE    		0xFF203010
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE			0xFF203040
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE		0xFF203070
#define ADC_BASE			0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE			0xFFC04000
#define I2C1_BASE			0xFFC05000
#define I2C2_BASE			0xFFC06000
#define I2C3_BASE			0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE			0xFFD0501C

#endif

// --- End of src/address_map.h ---

// --- Start of main.c ---
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
// MAIN ----------------------------------------------------------

int main(void) {
  set_up_interrupt_handler();
  initializeBuffers();
  int prev_sw = 0;
  int selected_sort = -1;
  int n =
      25;  // default number of rectangles selected if user forgets to choose
  bool ready_to_run = false;

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

// --- End of main.c ---

// --- Start of src/algorithms.c ---
// PROGRAM: DEFINES ALL ALGORITHMS

// Stripped local include: #include "algorithms.h"

#include <stdbool.h>
#include <stdio.h>

// HELPERS ---------------------------------------------------------------

// Function that swaps two values
void swap(int* first, int* second) {
  int temp = *first;
  *first = *second;
  *second = temp;
}

// Copies the original array into the 2D array
void saveArray(int arr[], int n, int steps_arr[][n], int* step_count) {
  for (int element = 0; element < n; element++) {
    steps_arr[*step_count][element] = arr[element];
  }
  *step_count = *step_count + 1;
}

// Function that takes a section of the array (low to high) and picks the last
// element as the pivot point Returns the index of the pivot point after
// everything < pivot is on the left and everything >= pivot is on the right
int quickSortPartition(int arr[], int low, int high, int n, int steps_arr[][n],
                       int* step_count) {
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
void quickSortRecursive(int arr[], int low, int high, int n, int steps_arr[][n],
                        int* step_count) {
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
void bubbleSort(int original_arr[], int num_size, int steps_arr[][num_size],
                int* step_count) {
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
void insertionSort(int original_arr[], int num_size, int steps_arr[][num_size],
                   int* step_count) {
  int inner_loop;
  int outer_loop;
  int key;

  // First, copy the original array as the first array in the list
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Second, start the insertion sort process
  for (outer_loop = 1; outer_loop < num_size; outer_loop++) {
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
void radixSort(int original_arr[], int num_size, int steps_arr[][num_size],
               int* step_count) {
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
void quickSort(int original_arr[], int num_size, int steps_arr[][num_size],
               int* step_count) {
  // First, copy the original array as the first array in the list
  *step_count = 0;
  saveArray(original_arr, num_size, steps_arr, step_count);

  // Call the recurive helper function to sort the array
  quickSortRecursive(original_arr, 0, num_size - 1, num_size, steps_arr,
                     step_count);
}

// --- End of src/algorithms.c ---

// --- Start of src/interrupt_handler.c ---
// Stripped local include: #include "interrupt_handler.h"

#define CLOCK_RATE 100000000
#define LOAD_VALUE 5000000
#define EXIT_SUCCESS 0
#define VGA_WIDTH 320
#define VGA_HEIGHT 240
#define MOUSE_RESOLUTION 2 // 2 counts/mm
#define MAX_MOUSE_COUNT 250 //maximum displcement in either x or y direction as +- 250 counts
#define MOUSE_PIXELS_PER_COUNT_X ((double)VGA_WIDTH / (2.0 * MAX_MOUSE_COUNT))
#define MOUSE_PIXELS_PER_COUNT_Y ((double)VGA_HEIGHT / (2.0 * MAX_MOUSE_COUNT))
#define MOUSE_GAIN_X 0.5
#define MOUSE_GAIN_Y 0.5


struct mouse_ptr {

    volatile unsigned char data;
    volatile unsigned char rValid;
    volatile unsigned short rAvail;
    volatile unsigned char reField; // for enabling interrupts
    volatile unsigned char pendingAndError; // bits to indicate pending interrupt and error when sending message
    volatile unsigned short unused;
};

volatile mouse_packet mouse_info;
volatile bool mouseClicked;

volatile int arrayStep = 0;
volatile bool currentlyDisplaying = false;

//MMIO POINTERS
volatile int * timer_ptr = (int *) TIMER_BASE;
struct mouse_ptr *const mousep = ((struct mouse_ptr *) PS2_BASE);

//interrupt handler address
static void handler(void) __attribute__ ((interrupt("machine")));
void set_interval_timer(void);
void interval_timer_ISR(void);
void mouse_handler_ISR(volatile mouse_packet* p);


//helper functions for mouse input:
static int ps2_byte_available(void) {
    return (mousep->rValid & 0x80) != 0;  
}

static char ps2_read_byte(void) {
    return (char)(mousep->data);
}

static void ps2_write_byte(char b) {
    mousep->data = b;
}

static char ps2_wait_byte(void){
    while(!ps2_byte_available()){}
    return ps2_read_byte();
}

static inline void ps2_enable_irq(void) {
    mousep->reField = 0x1;   // RE bit
}

static int ps2_decode_delta(unsigned char flags, unsigned char data, int sign_mask) {
    int v = data | ((flags & sign_mask) ? 0x100 : 0);
    if (v & 0x100) v -= 0x200;
    return v;
}

mouse_packet get_mouse_packet(void){
    return mouse_info;
}

void ps2_mouse_init(void){
    printf("entered mouse init\n");
    char b;

    ps2_write_byte(0xFF); //reset
    b = ps2_wait_byte(); //expect FA

    printf("reset ack = 0x%02X\n", b);

    b = ps2_wait_byte();           // expect AA
    printf("self test = 0x%02X\n", b);

    b = ps2_wait_byte();           // expect 00
    printf("mouse id   = 0x%02X\n", b);

    ps2_write_byte(0xE8);          // set resolution
    b = ps2_wait_byte();           // expect FA
    printf("E8 ack     = 0x%02X\n", b);

    ps2_write_byte(0x01);          // 1 counts/mm
    b = ps2_wait_byte();           // expect FA
    printf("res ack    = 0x%02X\n", b);

    ps2_enable_irq();              // enable PS/2 core interrupt

    ps2_write_byte(0xF4);          // enable streaming mode
    b = ps2_wait_byte();           // expect FA
    printf("F4 ack     = 0x%02X\n", b);
}

int set_up_interrupt_handler(void){
    int mstatus_value, mtvec_value, mie_value;

    //set initial mouse location
    mouse_info.x = VGA_WIDTH/2;
    mouse_info.y = VGA_HEIGHT/2;

    mstatus_value = 0x8;
    // disable interrupts
    __asm__ volatile ("csrc mstatus, %0" :: "r"(mstatus_value));

    //configure external hardware
    set_interval_timer();
    //ps2_mouse_init();

    mstatus_value = 0b1000; // interrupt bit mask

    mtvec_value = (int) &handler; // set trap address
    __asm__ volatile ("csrw mtvec, %0" :: "r"(mtvec_value));
    // disable all interrupts that are currently enabled
    __asm__ volatile ("csrr %0, mie" : "=r"(mie_value));
    __asm__ volatile ("csrc mie, %0" :: "r"(mie_value));

    mie_value = 0x450000; // KEY, itimer, PS/2
    // set interrupt enables
    __asm__ volatile ("csrs mie, %0" :: "r"(mie_value));
    // enable Nios V interrupts
    __asm__ volatile ("csrs mstatus, %0" :: "r"(mstatus_value));

    return EXIT_SUCCESS;

}

void handler(void){
    int mcause_value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause_value));
    mcause_value = mcause_value & 0x7FFFFFFF;
    if(mcause_value ==  16){
        //this is due to the interval timer
        interval_timer_ISR();
    }else if(mcause_value == 18){
        //KEYISR
    }else if(mcause_value == 22){
        mouse_handler_ISR(&mouse_info);
    }
}

void mouse_handler_ISR(volatile mouse_packet *p){
    //printf("entered mouse handler ISR\n");
    static unsigned char buffer[3];
    static int count = 0;

    while (ps2_byte_available()) {
        unsigned char byte = (unsigned char)ps2_read_byte();

        if (count == 0 && !(byte & 0x08)) {
            continue;   // wait for a valid first packet byte
        }

        buffer[count++] = byte;

        if (count == 3) {
            p->flags = buffer[0];
            p->dx = ps2_decode_delta(buffer[0], buffer[1], 0x10);
            p->dy = ps2_decode_delta(buffer[0], buffer[2], 0x20);

            p->x += p->dx*MOUSE_GAIN_X;
            p->y += p->dy*MOUSE_GAIN_Y; 

            if (p->x < 0) p->x = 0;
            if (p->x > VGA_WIDTH - 1) p->x = VGA_WIDTH - 1;

            if (p->y < 0) p->y = 0;
            if (p->y > VGA_HEIGHT - 1) p->y = VGA_HEIGHT - 1;

            p->leftButtonClicked  = (buffer[0] & 0x01) != 0;
            p->rightButtonClicked = (buffer[0] & 0x02) != 0;

            count = 0;

            if (p->leftButtonClicked) {
                printf("left button clicked: %d\n", p->leftButtonClicked);
            }
            if (p->rightButtonClicked) {
                printf("right button clicked: %d\n", p->rightButtonClicked);
            }

            printf("x location %f\n",p->x);
            printf("y location %f\n",p->y);

        }
    }
}


void interval_timer_ISR(){
    //clear the intterupt
    *timer_ptr = 0;
    if(currentlyDisplaying){
        arrayStep++;
    }
    //printf("timer triggered\n");

}

void set_interval_timer(){
    int loadValue = LOAD_VALUE;
    *(timer_ptr + 2) = (loadValue & 0xFFFF);
    *(timer_ptr + 3) = (loadValue>>16) & 0xFFFF;
    *(timer_ptr + 1) = 0x7;
}
// --- End of src/interrupt_handler.c ---

// --- Start of src/renderer.c ---
// PROGRAM: DISPLAYS STARTING SCREEN

// Stripped local include: #include "renderer.h"
// Stripped local include: #include "interrupt_handler.h"
// Stripped local include: #include "address_map.h"
#include <stdbool.h>
#include <stdlib.h>

// GLOBALS ------------------------------------------------
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

// IMPLEMENTATIONS -----------------------------------------------------

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
  drawBackground();
  drawResetScreen();
  waitForSync();
  PIXEL_BUFFER_START_1 = (volatile short int*)(*(PIXEL_CTRL_PTR_1 + 1));


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

  // Bubble sort button
  drawRectangle(3, 35, 42, 60, COLORS[6]);
  drawBorder(3, 35, 42, 60, COLORS[1]);

  // Insertion sort button
  drawRectangle(3, 63, 42, 87, COLORS[6]);
  drawBorder(3, 63, 42, 87, COLORS[1]);

  // Radix sort button
  drawRectangle(3, 90, 42, 115, COLORS[6]);
  drawBorder(3, 90, 42, 115, COLORS[1]);

  // Quick sort button
  drawRectangle(3, 118, 42, 143, COLORS[6]);
  drawBorder(3, 118, 42, 143, COLORS[1]);

  // Reset Button
  drawRectangle(3, 182, 42, 207, COLORS[7]);
  drawBorder(3, 182, 42, 207, COLORS[1]);

  // Go button
  drawRectangle(3, 210, 42, 235, COLORS[8]);
  drawBorder(3, 210, 42, 235, COLORS[1]);

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

// Function that draws white pixels on every pixel to "clear" the screen
void clearScreen() {
  // Loop through all the pixels and color them black to "clear" the screen
  // First for loop: loop through vertical pixels
  for (int y = 0; y <= 239; y++) {
    // Second for loop: loop through horizontal pixels
    for (int x = 0; x <= 319; x++) {
      // Color each pixel white
      plotPixel(x, y, COLORS[2]);
    }
  }
}

// Function that draws the background color only on the background portion of
// the screen
void clearBackground() {
  for (int y = 30; y <= 239; y++) {
    // Second for loop: loop through horizontal pixels
    for (int x = 45; x <= 319; x++) {
      // Color each pixel white
      plotPixel(x, y, COLORS[2]);
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
}

// --- End of src/renderer.c ---

