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
	bool mode = MY_TURN;
	// bool success;
	Target target;
	Cell* cell;
	Board myBoard = create_board();
	//Board theirBoard = blank_board();
	char i;
	// keep on trying to start up things until they succeed
	while (!startup_glcd()){}
	while (!startup_usart()){}
    TRISJ = 0xFF;
    TRISF = 0x00;
    TRISE = 0xFF;
//	for (i = 0; i < 64; i++) {
//		cell = get_cell(&myBoard, i%WIDTH, i/WIDTH);
//		cell->targeted = true;
//	}
	// main loop
	while (1) {
		draw_board(&myBoard);
        target = determine_target();
//        cell = get_cell(&myBoard, target.row, target.col);
//        cell->targeted = true;
        send_target(target);
		if (mode == MY_TURN) {
			target = determine_target();
			send_target(target);
//			receive_confirmation(&theirBoard, target); // if there is an error, it will get stuck forever
//			mode = rx_mode();
            mode = THEIR_TURN;
		}
		else {
			do {
				target = receive_target();
			} while (target.error);
//			send_confirmation(&myBoard, target); // if there is an error, it will get stuck forever
//			mode = tx_mode();
            cell = get_cell(&myBoard, target.row, target.col);
            cell->targeted = true;
            mode = MY_TURN;
		}
	}
}
