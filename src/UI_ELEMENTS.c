#include "UI_ELEMENTS.h"
#include "renderer.h"
#include "IMAGES.h"


//returns in the mouse is hovering over any of the elements in the current rendering layer
bool mouseHover(int mousex, int mousey,GENERAL_ELEMENT* element){

    if(RENDERING_LAYER != element->layer){
        return false; //if element is not drawn on the most recent layer, it cannot be hovered over
    }

    //printf("mouse hover called\n");

    int left   = element->boundary.topLeft.x + CURSOR_OFFSET_X;
    int right  = element->boundary.bottomRight.x;
    int top    = element->boundary.topLeft.y;
    int bottom = element->boundary.bottomRight.y;

    return (mousex >= left && mousex <= right &&
            mousey >= top  && mousey <= bottom);

}