#ifndef LCD
#define LCD

#include "targeting.h"
#include "board.h"
#include "bool.h"
#include <delays.h>

// Initialize the GLCD, return true when done successfully
bool startup_glcd(void);

// Print a string on the GLCD starting at x and y
bool write_string(char* string, int x, int y);

// Draws the whole board, probably will only be called at the beginning on the empty board
bool draw_board(Board* board);

// Draws an individual cell, probably called every time a target is chosen
bool draw_cell(Cell cell, char x, char y);

#endif
