#include "UI_ELEMENTS.h"
#include "renderer.h"
#include "IMAGES.h"
#include "APP_STATE.h"


bool shouldBeDrawn(PROGRAM_STATE currentState, GENERAL_ELEMENT* element){

    int numStates = sizeof(element->drawingStates)/sizeof(element->drawingStates[0]);

    for(int i = 0; i < numStates; i++){
        if(currentState == element->drawingStates[i]){
            //printf("should be drawn\n");
            return true;
        }
    }
    //printf("should not be drawn\n");
    return false;
}


//returns in the mouse is hovering over any of the elements in the current rendering layer
bool mouseHover(int mousex, int mousey,GENERAL_ELEMENT* element){

    // if(RENDERING_LAYER != element->layer){
    //     return false; //if element is not drawn on the most recent layer, it cannot be hovered over
    // }

    //printf("mouse hover called\n");

    int left   = element->boundary.topLeft.x + CURSOR_OFFSET_X;
    int right  = element->boundary.bottomRight.x;
    int top    = element->boundary.topLeft.y;
    int bottom = element->boundary.bottomRight.y;

    return (mousex >= left && mousex <= right &&
            mousey >= top  && mousey <= bottom);

}

void updateButton(buttonElement* self, mouse_packet* mouseInfo) {
    // if(!shouldBeDrawn(currentState,(GENERAL_ELEMENT*)self)){
    //     return;
    // }

    self->isHover = mouseHover(mouseInfo->x, mouseInfo->y, &self->parent);

    //clicked means that mouse is hovering and the left button is pressed down
    self->isClicked = self->isHover && mouseInfo->leftButtonClicked;

    if(currentState == MAIN_SCREEN || currentState == DISPLAYING){
        if(self->isClicked){
            if(numButtonsSelected == 0){
                self->isSelected = true;
                numButtonsSelected++;
            }else{
                for(int i = 0; i < UI_ELEMENT_COUNT; i++){
                    GENERAL_ELEMENT* e = UI_ELEMENTS[i];
                    if (e->type != BUTTON || !shouldBeDrawn(currentState,e)) continue;
                    buttonElement* b = (buttonElement*)e;
                    if(b->isSelected){
                        b->isSelected=false;
                    }
                }
                self->isSelected = true;
            }
        }
    }
}

void updateAllButtons(mouse_packet* mouseInfo) {
    for (int i = 0; i < UI_ELEMENT_COUNT; i++) {
        GENERAL_ELEMENT* e = UI_ELEMENTS[i];
        if (e->type != BUTTON || !shouldBeDrawn(currentState,e)) continue;

        buttonElement* b = (buttonElement*)e;
        updateButton(b, mouseInfo);
    }
}

void clearSortSelections(void) {
    for (int i = 0; i < UI_ELEMENT_COUNT; i++) {
        GENERAL_ELEMENT* e = UI_ELEMENTS[i];
        if (e->type != BUTTON) continue;

        buttonElement* b = (buttonElement*)e;
        if (b->action == BUBBLE_SORT ||
            b->action == INSERTION_SORT ||
            b->action == RADIX_SORT ||
            b->action == QUICK_SORT) {
            b->isSelected = false;
        }
    }
}

buttonElement* getClickedButton(mouse_packet* mouseInfo, bool clicked_now) {
    if (!clicked_now) return NULL;

    for (int i = 0; i < UI_ELEMENT_COUNT; i++) {
        GENERAL_ELEMENT* e = UI_ELEMENTS[i];
        //printf("current state is %d\n",currentState);
        if (e->type != BUTTON || !shouldBeDrawn(currentState,e)) continue;

        buttonElement* b = (buttonElement*)e;
        if (b->isHover) return b;
    }
    return NULL;
}

void selectBubbleSort(buttonElement* self) {
    selected_sort = BUBBLE_SORT;
}

void selectInsertionSort(buttonElement* self) {
    selected_sort = INSERTION_SORT;
}

void selectRadixSort(buttonElement* self) {
    selected_sort = RADIX_SORT;
}

void selectQuickSort(buttonElement* self) {
    selected_sort = QUICK_SORT;
}

void goAction(buttonElement* self) {
    if (selected_sort != -1) {
        currentState = DISPLAYING;
    }
}

void resetAction(buttonElement* self) {
    currentState = RESET_STATE;
}

void startProgram(buttonElement* self) {
    printf("program started\n");
    currentState = MAIN_SCREEN;
}