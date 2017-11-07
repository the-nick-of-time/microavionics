#ifndef LCD
#define LCD

#include "targeting.h"
#include "bool.h"
#include <delays.h>

// Initialize the LCD, return true when done successfully
bool startup_lcd(void);

// Initialize the GLCD, return true when done successfully
bool startup_glcd(void);

// Write a string to the LCD starting at the cursor position given
bool write_lcd_string(char* s, unsigned char cursor);

// Draws the whole board, probably will only be called at the beginning on the empty board
bool draw_board(Board* board);

// Draws an individual cell, probably called every time a target is chosen
bool draw_cell(Cell cell, char x, char y);

#endif
