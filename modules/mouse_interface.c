#include "mouse_interface.h"
#include "address_map.h"


struct mouse_coordinates{
    volatile double x;
    volatile double y;
};

struct mouse_ptr {

    volatile unsigned char data;
    volatile unsigned char unused;
    volatile unsigned short rValidRAvail;
    volatile unsigned char reField; // for enabling interrupts
    volatile unsigned char pendingAndError; // bits to indicate pending interrupt and error when sending message
};
 
volatile struct mouse_coordinates mouse_location; 
volatile bool mouseClicked;

struct mouse_ptr *const mouseP = ((struct mouse_ptr *) PS2_BASE);

void setUp(void){

    

}




