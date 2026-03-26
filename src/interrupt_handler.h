
#include "address_map.h"
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

