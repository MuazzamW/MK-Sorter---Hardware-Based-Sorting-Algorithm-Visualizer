#include "UI_ELEMENTS.h"

bool mouseHover(int mousex, int mousey,GENERAL_ELEMENT* element){

    return (element->boundary.bottomRight.y > mousey && mousey > element->boundary.topLeft.y
            && element->boundary.bottomRight.x > mousex && mousex > element->boundary.topLeft.x);

}