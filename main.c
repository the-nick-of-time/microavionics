#include <p18cxxx.h>
#include "board.h"
#include "lcd.h"
#include "targeting.h"
#include "comms.h"
#include "bool.h"

#pragma config FOSC=HS1, PWRTEN=ON, BOREN=ON, BORV=2, PLLCFG=OFF
#pragma config WDTEN=OFF, CCP2MX=PORTC, XINST=OFF

void main(void) {
	// 1 for your turn/transmit mode; 0 for the other person's turn/recieve mode
	bool mode;
	// bool success;
	Target target;
	Cell* cell;
	Board myBoard;
    myBoard = create_board();
	// keep on trying to start up things until they succeed
	while (!startup_glcd()){}
	while (!startup_usart()){}
    TRISJ = 0xFF;
    TRISF = 0x00;
    LATF = 0x00;
    TRISE = 0xFF;
    mode = MY_TURN;
	// main loop
	while (1) {
		draw_board(&myBoard);
		if (mode == MY_TURN) {
            LATF = 0xF0;
            //write_string("MY TURN", 64+7, 3);
			target = determine_target();
			send_target(target);
            mode = THEIR_TURN;
		}
		else {
            //write_string("THEIR TURN", 64+7, 3);
            LATF = 0x0F;
			do {
				target = receive_target();
			} while (target.error);
//            target.row = 5;
//            target.col = 6;
//            target.error = 0;
//            cell = get_cell(&myBoard, target.row, target.col);
//            cell->targeted = true;
            mode = MY_TURN;
		}
	}
}
