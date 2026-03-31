#include <stdbool.h>

enum ELEMENT_TYPE { 
  PANEL,
  TEXT,
  BUTTON 
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

} GENERAL_ELEMENT;

typedef struct{

    GENERAL_ELEMENT parent;
    char* text;
    short int textColour;
    bool isClicked;

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







