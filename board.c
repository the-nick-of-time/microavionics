#include "board.h"


Board create_board(void){
	Board board = blank_board();
	unsigned char lengths[] = {4, 3, 3, 2};
	Ship ships[4];
	bool locationFound = false;
	bool orientationFound = false;
	unsigned char x, y, orientation;
	for (size_t i = 0; i < 4; i++) {
		ships[i].length = lengths[i];
		while (!locationFound) {
			x = (unsigned char) (rand() % 8);
			y = (unsigned char) (rand() % 8);
			orientation = (x * y) % 4;
			ships[i].x = x;
			ships[i].y = y;
			while (!orientationFound) {
				ships[i].orientation = orientation;
				if (is_valid(&board, ships[i])) {
					orientationFound = true;
					locationFound = true;
				}
				else {
					orientation = (orientation + 1) % 4;
					if (orientation = (x*y) % 4) {
						// we have checked every orientation so this location doesn't work
						break;
					}
				}
			}
		}
		// The ship location and orientation is okay so add it to the board
		place_ship(&board, ships[i]);
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
