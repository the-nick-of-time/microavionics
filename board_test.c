// Intended for compilation on the dev machine, NOT FOR MICROCONTROLLER
#include <stdio.h>
#include "board.h"


void print_board(Board* board) {
	int i;
	for (i = 0; i < WIDTH*HEIGHT; i++) {
		if (i % WIDTH == 0) {
			printf("\n");
		}
		if (board->cells[i].occupied) {
			printf("x");
		}
		else {
			printf(".");
		}
	}
	printf("\n");
}


int main(int argc, char const *argv[]) {
	Board board = create_board();
	print_board(&board);
	return 0;
}
