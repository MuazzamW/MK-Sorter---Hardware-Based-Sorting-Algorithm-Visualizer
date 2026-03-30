#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include "address_map.h"
#include <stdbool.h>
#include <stdio.h>

// extern volatile struct mouse_coordinates mouse_location; 
// extern volatile bool mouseClicked;

typedef struct {
    volatile int dx;
    volatile int dy;
    volatile unsigned char flags;
    volatile bool leftButtonClicked;
    volatile bool rightButtonClicked;

    volatile int x;
    volatile int y;

}mouse_packet;

int set_up_interrupt_handler(void);
mouse_packet get_mouse_packet(void);

extern volatile int arrayStep;
extern volatile bool currentlyDisplaying;

#endif
