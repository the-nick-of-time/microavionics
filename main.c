#include <p18cxxx.h>
#include "board.h"
#include "lcd.h"
#include "targeting.h"
#include "comms.h"
#include "bool.h"

#define MY_TURN 1
#define THEIR_TURN 0

int main(int argc, char const *argv[]) {
	// 1 for your turn/transmit mode; 0 for the other person's turn/recieve mode
	bool mode;
	// bool success;
	Target target;
	Cell cell;
	Board myBoard = create_board();
	Board theirBoard = blank_board();
	// keep on trying to start up things until they succeed
	while (!startup_lcd()){}
	while (!startup_glcd()){}
	while (!startup_usart()){}
	// main loop
	while (1) {
		if (mode == MY_TURN) {
			write_lcd_string("Your turn", 0x80);
			target = determine_target();
			send_target(target);
			receive_confirmation(&theirBoard, target);
			mode = rx_mode();
		}
		else {
			write_lcd_string("Their turn", 0x80);
			target = receive_target();
			send_confirmation(&myBoard, target);
			mode = tx_mode();
		}
	}
	return 0;
}
