// PROGRAM: DISPLAYS STARTING SCREEN

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// GLOBALS ------------------------------------------------
	
volatile short int *PIXEL_BUFFER_START;
short int BUFFER1[240][512];
short int BUFFER2[240][512];
short int COLORS[9] = {0xFFFF, 0x0000, 0xEF5D, 0xC618, 0x8D96, 0x8D91, 0x6B4D, 0xF800, 0x07E0};
char SMALL_CHAR[26][5] = {
{2,5,7,5,5}, // A
{6,5,6,5,6}, // B
{3,4,4,4,3}, // C
{6,5,5,5,6}, // D
{7,4,6,4,7}, // E
{7,4,6,4,4}, // F
{3,4,5,5,3}, // G
{5,5,7,5,5}, // H
{7,2,2,2,7}, // I
{1,1,1,5,2}, // J
{5,5,6,5,5}, // K
{4,4,4,4,7}, // L
{5,7,7,5,5}, // M
{5,7,7,7,5}, // N
{2,5,5,5,2}, // O
{6,5,6,4,4}, // P
{2,5,5,7,3}, // Q
{6,5,6,5,5}, // R
{3,4,2,1,6}, // S
{7,2,2,2,2}, // T
{5,5,5,5,7}, // U
{5,5,5,5,2}, // V
{5,5,7,7,5}, // W
{5,5,2,5,5}, // X
{5,5,2,2,2}, // Y
{7,1,2,4,7}  // Z
};
char MEDIUM_CHAR[26][7] = {
{14,17,17,31,17,17,17}, // A
{30,17,17,30,17,17,30}, // B
{14,17,16,16,16,17,14}, // C
{30,17,17,17,17,17,30}, // D
{31,16,16,30,16,16,31}, // E
{31,16,16,30,16,16,16}, // F
{14,17,16,16,19,17,14}, // G
{17,17,17,31,17,17,17}, // H
{14,4,4,4,4,4,14},      // I
{1,1,1,1,17,17,14},     // J
{17,18,20,24,20,18,17}, // K
{16,16,16,16,16,16,31}, // L
{17,27,21,17,17,17,17}, // M
{17,25,21,19,17,17,17}, // N
{14,17,17,17,17,17,14}, // O
{30,17,17,30,16,16,16}, // P
{14,17,17,17,21,18,13}, // Q
{30,17,17,30,20,18,17}, // R
{15,16,16,14,1,1,30},   // S
{31,4,4,4,4,4,4},       // T
{17,17,17,17,17,17,14}, // U
{17,17,17,17,17,10,4},  // V
{17,17,17,17,21,27,17}, // W
{17,17,10,4,10,17,17},  // X
{17,17,10,4,4,4,4},     // Y
{31,1,2,4,8,16,31}      // Z
};
char LARGE_CHAR[26][8] = {
{24,36,66,126,66,66,66,0}, // A
{124,66,66,124,66,66,124,0}, // B
{60,66,64,64,64,66,60,0}, // C
{120,68,66,66,66,68,120,0}, // D
{126,64,64,124,64,64,126,0}, // E
{126,64,64,124,64,64,64,0}, // F
{60,66,64,78,66,66,60,0}, // G
{66,66,66,126,66,66,66,0}, // H
{60,16,16,16,16,16,60,0}, // I
{30,4,4,4,68,68,56,0}, // J
{66,68,72,112,72,68,66,0}, // K
{64,64,64,64,64,64,126,0}, // L
{66,102,90,66,66,66,66,0}, // M
{66,98,82,74,70,66,66,0}, // N
{60,66,66,66,66,66,60,0}, // O
{124,66,66,124,64,64,64,0}, // P
{60,66,66,66,74,68,58,0}, // Q
{124,66,66,124,72,68,66,0}, // R
{60,64,64,60,2,2,124,0}, // S
{126,16,16,16,16,16,16,0}, // T
{66,66,66,66,66,66,60,0}, // U
{66,66,66,66,66,36,24,0}, // V
{66,66,66,90,90,102,66,0}, // W
{66,36,24,24,24,36,66,0}, // X
{66,66,36,24,16,16,16,0}, // Y
{126,4,8,16,32,64,126,0} // Z
};

// DECLARATIONS -------------------------------------------

void clearScreen();
void plotPixel(int x, int y, short int line_color);
void drawLine(int x1, int y1, int x2, int y2, short int line_color);
void drawRectangle(int x1, int y1, int x2, int y2, short int line_color);
void drawBorder(int x1, int y1, int x2, int y2, short int line_color);
void drawSmallChar(int x, int y, char c, short int color);
void drawSmallText(int x, int y, char *text, short int color);
void drawMediumChar(int x, int y, char c, short int color);
void drawMediumText(int x, int y, char *text, short int color);
void drawLargeChar(int x, int y, char c, short int color);
void drawLargeText(int x, int y, char *text, short int color);

// MAIN ---------------------------------------------------

int main(void) {
	volatile int *pixel_ctrl_ptr = (int *)0xFF203020;		// Front buffer
	PIXEL_BUFFER_START = (volatile short int *)(*pixel_ctrl_ptr);
	
	clearScreen();
	
	// Top Panel 
	drawRectangle(0, 0, 319, 30, COLORS[4]);
	drawBorder(0, 0, 319, 30, COLORS[1]);
	
	// Side Panel
	drawRectangle(0, 32, 45, 239, COLORS[3]);
	drawBorder(0, 32, 45, 239, COLORS[1]);
	
	// Bubble sort button
	drawRectangle(3, 35, 42, 60, COLORS[6]);
	drawBorder(3, 35, 42, 60, COLORS[1]);
	
	// Insertion sort button
	drawRectangle(3, 63, 42, 87, COLORS[6]);
	drawBorder(3, 63, 42, 87, COLORS[1]);
	
	// Radix sort button
	drawRectangle(3, 90, 42, 115, COLORS[6]);
	drawBorder(3, 90, 42, 115, COLORS[1]);
	
	// Quick sort button
	drawRectangle(3, 118, 42, 143, COLORS[6]);
	drawBorder(3, 118, 42, 143, COLORS[1]);
	
	// Reset Button
	drawRectangle(3, 182, 42, 207, COLORS[7]);
	drawBorder(3, 182, 42, 207, COLORS[1]);
	
	// Go button
	drawRectangle(3, 210, 42, 235, COLORS[8]);
	drawBorder(3, 210, 42, 235, COLORS[1]);
	
	// Random rectangles
	int randomY = rand() % 120 + 50;
	int starting_x = 50;
	int dx = 17;
	for (int rect_num = 0; rect_num <= 13; rect_num++) {
		drawRectangle(starting_x, randomY, starting_x + dx, 239, COLORS[5]);
		drawBorder(starting_x, randomY, starting_x + dx, 239, COLORS[1]);
		starting_x = starting_x + dx + 2;
		randomY = rand() % 120 + 50;
	}
	
	// Draw text on the screen
	drawLargeText(130, 14, "MK SORTER", COLORS[1]);
	drawSmallText(270, 20, "ECE PROJECT", COLORS[1]);
	drawSmallText(5, 20, "MUAZZAM W AND KARINA K", COLORS[1]);
	drawSmallText(7, 40, "BUBBLE", COLORS[0]);
	drawSmallText(7, 50, "SORT", COLORS[0]);
	drawSmallText(5, 68, "INSERTION", COLORS[0]);
	drawSmallText(5, 78, "SORT", COLORS[0]);
	drawSmallText(7, 95, "RADIX", COLORS[0]);
	drawSmallText(7, 105, "SORT", COLORS[0]);
	drawSmallText(7, 123, "QUICK", COLORS[0]);
	drawSmallText(7, 133, "SORT", COLORS[0]);
	drawMediumText(9, 192, "RESET", COLORS[0]);
	drawMediumText(17, 220, "GO", COLORS[0]);
	
	return 0;
}

// Function that draws white pixels on every pixel to "clear" the screen
void clearScreen() { 
	// Loop through all the pixels and color them black to "clear" the screen
	// First for loop: loop through vertical pixels
	for (int y = 0; y <= 239; y++) {
		// Second for loop: loop through horizontal pixels
		for (int x = 0; x <= 319; x++) {
			// Color each pixel white
			plotPixel(x, y, COLORS[2]);
		}
	}
}

// Function that plots one pixel in a given color
void plotPixel(int x, int y, short int line_color) {
	volatile short int *one_pixel_address;
	one_pixel_address = PIXEL_BUFFER_START + (y << 9) + x;
	*one_pixel_address = line_color;
}

// Function that draws a line from (x1, y1) to (x2, y2)
void drawLine(int x1, int y1, int x2, int y2, short int line_color) {
	// First check how steep the line is
	bool is_steep = false;
	if (abs(y2 - y1) > abs(x2 - x1)) {
		is_steep = true;
	}
	
	// Swap the x and y variables if the line is steep
	if (is_steep) {
		int temp = x1;
		x1 = y1;
		y1 = temp;

		temp = x2;
		x2 = y2;
		y2 = temp;
	}

	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Calculate the horizontal and vertical distances the line is being drawn
	int delta_x = x2 - x1;
	int delta_y = abs(y2 - y1);
	
	// Error variable to keep track of when the y-coordinate should change
	int error = -(delta_x / 2);
	int y = y1;
	int y_step;
	
	// Is the line pointing up or down
	if (y1 < y2) {
		y_step = 1;
	} else {
		y_step = -1;
	}
	
	// Loop through each x coordinate of the line
	for (int x = x1; x <= x2; x++) {
		// If the line is steep, switch the x and y coordinates
		if (is_steep == true) {
			plotPixel(y, x, line_color);
		} else {
			plotPixel(x, y, line_color);
		}
		// Update the error
		error = error + delta_y;
		// If the error becomes too large then we are too far from the line
		// Shift the y coordinate occasionally
		if (error > 0) {
			y = y + y_step;
			error = error - delta_x;
		}
	}
}
// Function that draws a rectangle
void drawRectangle(int x1, int y1, int x2, int y2, short int line_color) {
	// Draw a horizontal line for each y-value
	for (int y = y1; y <= y2; y++) {
		drawLine(x1, y, x2, y, line_color);
	}
}

// Function that draws a border (empty rectangle)
void drawBorder(int x1, int y1, int x2, int y2, short int line_color) {
	drawLine(x1, y1, x2, y1, line_color);
	drawLine(x2, y1, x2, y2, line_color);
	drawLine(x1, y1, x1, y2, line_color);
	drawLine(x1, y2, x2, y2, line_color);
}

void drawSmallChar(int x, int y, char c, short int color)
{
    if (c < 'A' || c > 'Z') return;
    int index = c - 'A';
    for (int row = 0; row < 5; row++)
    {
        char bits = SMALL_CHAR[index][row];
        for (int col = 0; col < 3; col++)
        {
            if (bits & (1 << (2 - col)))
            {
                plotPixel(x + col, y + row, color);
            }
        }
    }
}

void drawSmallText(int x, int y, char *text, short int color)
{
    while (*text)
    {
        drawSmallChar(x, y, *text, color);
        x += 4;
        text++;
    }
}

void drawMediumChar(int x, int y, char c, short int color)
{
    if (c < 'A' || c > 'Z') return;
    int index = c - 'A';
    for (int row = 0; row < 7; row++)
    {
        char bits = MEDIUM_CHAR[index][row];
        for (int col = 0; col < 5; col++)
        {
            if (bits & (1 << (4 - col)))
            {
                plotPixel(x + col, y + row, color);
            }
        }
    }
}

void drawMediumText(int x, int y, char *text, short int color)
{
    while (*text)
    {
        drawMediumChar(x, y, *text, color);
        x += 6;
        text++;
    }
}

void drawLargeChar(int x, int y, char c, short int color)
{
    if (c < 'A' || c > 'Z') return;
    int index = c - 'A';
    for (int row = 0; row < 8; row++)
    {
        unsigned char bits = LARGE_CHAR[index][row];
        for (int col = 0; col < 8; col++)
        {
            if (bits & (1 << (7 - col)))
            {
                plotPixel(x + col, y + row, color);
            }
        }
    }
}

void drawLargeText(int x, int y, char *text, short int color)
{
    while (*text)
    {
        drawLargeChar(x, y, *text, color);
        x += 9;
        text++;
    }
}
