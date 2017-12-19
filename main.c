#include <p18cxxx.h>
#include "board.h"
#include "lcd.h"
#include "targeting.h"
#include "comms.h"
#include "bool.h"
#include "ADCread.h"

#pragma config FOSC=HS1, PWRTEN=ON, BOREN=ON, BORV=2, PLLCFG=OFF
#pragma config WDTEN=OFF, CCP2MX=PORTC, XINST=OFF

void main(void) {
	// 1 for your turn/transmit mode; 0 for the other person's turn/recieve mode
	bool mode;
	Target target;
	Cell* cell;
	Board myBoard;
	char myturn[] = {'M','Y',' ','T','U','R','N','\0'};
	char theirturn[] = {'T','H','E','I','R',' ','T','U','R','N','\0'};
	// keep on trying to start up things until they succeed
	while (!startup_glcd()){}
	while (!startup_usart()){}
	while (!startup_targeting()){}
	while (!startup_ADC()){}
	// DEBUGGING
	TRISF = 0x00;
	LATF = 0x00;
	// /DEBUGGING
	myBoard = create_board();
	// note: if this moves before "myBoard = create_board();", mode gets corrupted
	mode = MY_TURN;
	// main loop
	while (1) {
		draw_board(&myBoard);
		Nop();
		if (mode == MY_TURN) {
			LATF = 0xF0;
			// write_string(myturn, 64+7, 3);
			target = determine_target();
			send_target(target);
			mode = THEIR_TURN;
		}
		else {
			LATF = 0x0F;
			// write_string(theirturn, 64+7, 3);
			do {
				target = receive_target();
			} while (target.error);
			cell = get_cell(&myBoard, target.row, target.col);
			cell->targeted = true;
			mode = MY_TURN;
		}
	}
}
