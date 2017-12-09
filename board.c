#include "board.h"
// FOR DEBUGGING
#include <stdio.h>


Board create_board(void){
	// Seed the RNG
	srand(time(NULL));
	Board board = blank_board();
	// In the real game we have 5, 4, 3, 3, 3, 2
	unsigned char lengths[] = {4, 3, 3, 2};
	Ship ships[4];
	bool locationFound = false;
	bool orientationFound = false;
	unsigned char x, y, orientation, i;
	for (i = 0; i < 4; i++) {
		ships[i].length = lengths[i];
		while (!locationFound) {
			// select x and y randomly
			x = (unsigned char) (rand() % HEIGHT);
			y = (unsigned char) (rand() % WIDTH);
			// select a starting orientation semi-randomly
			// This prevents biases towards one orientation, as long as HEIGHT % 4 is 0 so the distribution of orientations are uniform
			orientation = x % 4;
			ships[i].x = x;
			ships[i].y = y;
			while (!orientationFound) {
				ships[i].orientation = orientation;
				if (is_valid(&board, ships[i])) {
					orientationFound = true;
					locationFound = true;
				}
				else {
					// turn by 90 degrees and try again
					orientation = (orientation + 1) % 4;
					if (orientation == x % 4) {
						// we have checked every orientation so this location doesn't work
						// go to the outer loop to reselect the x and y
						break;
					}
				}
			}
		}
		// The ship location and orientation is okay so add it to the board
		place_ship(&board, ships[i]);
		// Reset variables for next run
		locationFound = false;
		orientationFound = false;
	}
	return board;
}


Board blank_board(void){
	Board board;
	for (size_t i = 0; i < 64; i++) {
		board.cells[i].occupied = false;
		board.cells[i].targeted = false;
	}
	return board;
}


Cell* get_cell(Board* board, int row, int col){
	return &(board->cells[row * WIDTH + col]);
}

void advance(int* row, int* col, int orientation) {
	/* Goes to the next cell along the vector defined by the orientation
	orientation: 0123 for NESW
	row: positive downwards
	col: positive rightwards
	*/
	switch (orientation) {
		case 0: //north
			(*row)--;
			break;
		case 1: //east
			(*col)++;
			break;
		case 2: //south
			(*row)++;
			break;
		case 3: //west
			(*col)--;
			break;
	}
}


void place_ship(Board* board, Ship ship){
	/* Mark the cells on the board that are occupied by the ship */
	Cell* c;
	int row = ship.y, col = ship.x;
	for (char i = 0; i < ship.length; i++) {
		c = get_cell(board, row, col);
		c->occupied = true;
		advance(&row, &col, ship.orientation);
	}
}


bool is_valid(Board* board, Ship ship){
	/* Checks whether the ship can fit on the board in its current location and orientation */
	Cell* c;
	int row = ship.y, col = ship.x;
	for (char i = 0; i < ship.length; i++) {
		if (row >= HEIGHT || row < 0 || col >= WIDTH || col < 0){
			// off the board
			return false;
		}
		c = get_cell(board, row, col);
		if (c->occupied){
			// a cell is already taken by another ship
			return false;
		}
		// else move to the next cell along the ship and go on
		advance(&row, &col, ship.orientation);
	}
	// If it has survived this far, it is good
	return true;
}
