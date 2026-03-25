#ifndef RENDERER_H
#define RENDERER_H
#define MAX_SIZE 1000000

void clearScreen();
void plotPixel(int x, int y, short int line_color);
void drawLine(int x1, int y1, int x2, int y2, short int line_color);
void drawRectangle(int x1, int y1, int x2, int y2, short int line_color);
void drawBorder(int x1, int y1, int x2, int y2, short int line_color);
void drawSmallChar(int x, int y, char c, short int color);
void drawSmallText(int x, int y, char* text, short int color);
void drawMediumChar(int x, int y, char c, short int color);
void drawMediumText(int x, int y, char* text, short int color);
void drawLargeChar(int x, int y, char c, short int color);
void drawLargeText(int x, int y, char* text, short int color);
void drawBackground();
void drawResetScreen();
void clearBackground();
void waitForSync();
void drawSortSteps(int arr[], int n, int steps_arr[][MAX_SIZE], int step_count);

#endif