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
<<<<<<< HEAD
	bool mode = THEIR_TURN;
=======
	bool mode;
>>>>>>> 9dd49353480de15cea8a9be9ad30d942f977e834
	// bool success;
	Target target;
	Cell* cell;
	Board myBoard = create_board();
	//Board theirBoard = blank_board();
	char i;
    char my_turn[] = {'M','Y',' ','T','U','R','N','\0'};
    char their_turn[] = {'T','H','E','I','R',' ','T','U','R','N','\0'};
	// keep on trying to start up things until they succeed
	while (!startup_glcd()){}
	while (!startup_usart()){}
    TRISJ = 0xFF;
    TRISF = 0x00;
    LATF = 0x00;
    TRISE = 0xFF;
//    cell = get_cell(&myBoard, 0, 0);
//    cell->targeted = false;
    mode = MY_TURN;
	// main loop
	while (1) {
		draw_board(&myBoard);
<<<<<<< HEAD
 //       target = determine_target();
//        cell = get_cell(&myBoard, target.row, target.col);
//        cell->targeted = true;
 //       send_target(target);
		if (mode == MY_TURN) {
            LATF = 0xF0;
            write_string(my_turn, 7+64, 3);
=======
        Nop();
		if (mode == MY_TURN) {
//            write_string("MY TURN", 64+7, 3);
>>>>>>> 9dd49353480de15cea8a9be9ad30d942f977e834
			target = determine_target();
			send_target(target);
            LATF = 0xF0;
            mode = THEIR_TURN;
		}
		else {
<<<<<<< HEAD
            LATF = 0x0F;
            write_string(their_turn, 7+64, 3);
=======
//            write_string("THEIR TURN", 64+7, 3);
>>>>>>> 9dd49353480de15cea8a9be9ad30d942f977e834
			do {
				target = receive_target();
			} while (target.error);
//            target.row = 5;
//            target.col = 6;
//            target.error = 0;
//            cell = get_cell(&myBoard, target.row, target.col);
//            cell->targeted = true;
            LATF = 0x0F;
            mode = MY_TURN;
		}
	}
}
