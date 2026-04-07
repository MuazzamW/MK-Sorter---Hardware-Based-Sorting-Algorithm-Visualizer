#ifndef APP_STATE_H
#define APP_STATE_H


/*

This header file gives access to the current state of the program to all
other files that need it such as renderer.c, interrupt_handler.c, etc.
The program state is one of 4 values in the enum below


*/

typedef enum {
    STARTING_SCREEN,
    MAIN_SCREEN,
    DISPLAYING,
    RESET_STATE
} PROGRAM_STATE;

extern PROGRAM_STATE currentState;
extern int selected_sort;
//number of buttons selected in the uiS
extern int numButtonsSelected;

#endif