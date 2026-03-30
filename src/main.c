#include "interrupt_handler.h"
#include "renderer.h"
#include "images.h"

#define VGA_PIXEL_WIDTH 320
#define VGA_PIXEL_HEIGHT 240

int main(void){

    set_up_interrupt_handler();
    initializeBuffers();

    while(1){
        clearScreen();
        drawCursor(VGA_PIXEL_WIDTH/2, VGA_PIXEL_HEIGHT/2);
        waitForSync();
    }

}