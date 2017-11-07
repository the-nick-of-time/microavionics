#ifndef BOARD
#define BOARD

#include <time.h>
#include <stdlib.h>
// for random number generation
#include "bool.h"

#define WIDTH 8
#define HEIGHT 8

typedef struct ship {
	// 0-8 for board position
	unsigned char x:3;
	unsigned char y:3;
	// 2-4 but it might be expanded to 5 to be like the real game
	unsigned char length:3;
	// 0-3 for NESW
	unsigned char orientation:2;
} Ship;

typedef struct cell {
	// This should make them 1 bit long each looking at mplab's .h files
	unsigned targeted:1;
	unsigned occupied:1;
} Cell;

typedef struct board {
	/*
	0----W
	|    |
	|    |
	H----|
	The board origin is in the top left
	Y is positive downwards
	X is positive rightwards

	row-major list of cells
	meaning the indices are like this:
	0 1 2 3 4 5 6 7
	8 9...
	*/
	Cell cells[64];
} Board;

// Creates a board with ships placed on it randomly
Board create_board(void);

// Creates a board that is not occupied
Board blank_board(void);

// Checks whether adding ship to the board fits or whether it runs into something else/off the board
bool is_valid(Board* board, Ship ship);

// Modifies board in place to add "occupied" flags to the cells that the ship is taking up
void place_ship(Board* board, Ship ship);

#endif
