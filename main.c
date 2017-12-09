#include <p18cxxx.h>
#include "board.h"
#include "lcd.h"
#include "targeting.h"
#include "comms.h"
#include "bool.h"

int main(int argc, char const *argv[]) {
	// 1 for your turn/transmit mode; 0 for the other person's turn/recieve mode
	bool mode;
	// bool success;
	Target target;
	Cell cell;
	Board myBoard = create_board();
	Board theirBoard = blank_board();
	// keep on trying to start up things until they succeed
	while (!startup_glcd()){}
	while (!startup_usart()){}
	// main loop
	while (1) {
		if (mode == MY_TURN) {
			target = determine_target();
			send_target(target);
			receive_confirmation(&theirBoard, target); // if there is an error, it will get stuck forever
			mode = rx_mode();
		}
		else {
			do {
				target = receive_target();
			} while (target.error);
			send_confirmation(&myBoard, target); // if there is an error, it will get stuck forever
			mode = tx_mode();
		}
	}
	return 0;
}
