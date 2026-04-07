#ifndef APP_STATE_H
#define APP_STATE_H

typedef enum {
    STARTING_SCREEN,
    MAIN_SCREEN,
    DISPLAYING,
    RESET_STATE
} PROGRAM_STATE;

extern PROGRAM_STATE currentState;
extern int selected_sort;
extern int numButtonsSelected;

#endif