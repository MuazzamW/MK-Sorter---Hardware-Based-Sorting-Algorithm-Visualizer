#ifndef UIELEMENTS_H
#define UIELEMENTS_H

/*

This header file defines the structure for elements to draw on the screen
There is a main element "GENERAL_ELEMENT" which acts as a base class for any elements to draw
There are three specific elements which extent the general element including:
- Buttons
- Panels
- Bars (when sorting)

Currently only the buttons are used in the main program but the ultimate goal is to define all elements as individual objects

*/


#include <stdbool.h>
#include "interrupt_handler.h"
#include "APP_STATE.h"

typedef struct buttonElement buttonElement;
typedef void (*ButtonCallback)(buttonElement* self);

enum ELEMENT_TYPE { 
  PANEL,
  TEXT,
  BUTTON 
};

enum BUTTON_ACTION { 
  BUBBLE_SORT,
  INSERTION_SORT,
  RADIX_SORT,
  QUICK_SORT,
  GO,
  RESET,
  START 
};

typedef struct {
    int x;
    int y;
} coordinate;

typedef struct{
    coordinate topLeft;
    coordinate bottomRight;
}bounds;

typedef struct {

    bounds boundary;
    double WIDTH;
    double HEIGHT;
    short int backgroundColour;
    short int borderColor;
    char layer;
    enum ELEMENT_TYPE type;
    bool clickable;
    //elements can have up to 10 drawing states
    PROGRAM_STATE drawingStates[10];

} GENERAL_ELEMENT;

typedef struct buttonElement{

    GENERAL_ELEMENT parent;
    char* text;
    short int textColour;
    bool isClicked;
    bool isHover;
    bool isSelected;

    enum BUTTON_ACTION action;
    ButtonCallback onClick;

}buttonElement;

typedef struct{

    GENERAL_ELEMENT parent;
    char* text;
    short int textColour;

}panelElement;

typedef struct{

    GENERAL_ELEMENT parent;

}BAR;

//returns true if the mouse is hovering anywhere over the element
bool mouseHover(int mousex, int mousey, GENERAL_ELEMENT* element);
//void updateButton(buttonElement*self, mouse_packet* mouseInfo);
void updateAllButtons(mouse_packet* mouseInfo);
void clearSortSelections(void);
buttonElement* getClickedButton(mouse_packet* mouseInfo, bool clicked_now);
bool shouldBeDrawn(PROGRAM_STATE currentState, GENERAL_ELEMENT* element);

//BUTTON CALLBACKS
void selectBubbleSort(buttonElement* self);

void selectInsertionSort(buttonElement* self);

void selectRadixSort(buttonElement* self);

void selectQuickSort(buttonElement* self);

void goAction(buttonElement* self);

void resetAction(buttonElement* self);

void startProgram(buttonElement* self);


#endif






