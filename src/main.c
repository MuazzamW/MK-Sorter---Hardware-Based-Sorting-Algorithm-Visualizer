#include "interrupt_handler.h"
#include "renderer.h"
#include "images.h"

#define VGA_PIXEL_WIDTH 320
#define VGA_PIXEL_HEIGHT 240

int main(void){

    mouse_packet mouseInfo;


    set_up_interrupt_handler();
    initializeBuffers();

    while(1){
        clearScreen();
        mouseInfo = get_mouse_packet();
        drawCursor(mouseInfo.x,mouseInfo.y);
        waitForSync();
    }

}