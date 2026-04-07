#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include "address_map.h"
#include <stdbool.h>
#include <stdio.h>


#define CLOCK_RATE 100000000
#define LOAD_VALUE 5000000

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
extern volatile int keyNumber;

#endif
