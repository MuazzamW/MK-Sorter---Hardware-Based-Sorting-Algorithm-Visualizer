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
  RESET 
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

} GENERAL_ELEMENT;

typedef struct{

    GENERAL_ELEMENT parent;
    char* text;
    short int textColour;
    bool isClicked;
    bool isHover;
    enum BUTTON_ACTION action;

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


#endif






