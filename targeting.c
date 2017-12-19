#include <p18cxxx.h>
#include "targeting.h"
#include "bool.h"


bool startup_targeting(void) {
	// Set targeting buttons as all inputs
	ROWTRIS = 0xFF;
	COLTRIS = 0xFF;
	return true;
}


Target determine_target(void) {
	bool rowfound = false, colfound = false;
	Target target;
	while (!rowfound) {
		// poll row port for button presses
		switch (ROWPORT) {
			// Break out button presses, should be a 2^n
			case 1:
				target.row = 0;
				rowfound = true;
				break;
			case 2:
				target.row = 1;
				rowfound = true;
				break;
			case 4:
				target.row = 2;
				rowfound = true;
				break;
			case 8:
				target.row = 3;
				rowfound = true;
				break;
			case 16:
				target.row = 4;
				rowfound = true;
				break;
			case 32:
				target.row = 5;
				rowfound = true;
				break;
			case 64:
				target.row = 6;
				rowfound = true;
				break;
			case 128:
				target.row = 7;
				rowfound = true;
				break;
			default:
				// More than one button was pressed, or no button pressed yet
				Nop();
				break;
		}
	}
	while (!colfound) {
		// poll column targeting port
		switch (COLPORT) {
			case 1:
				target.col = 0;
				colfound = true;
				break;
			case 2:
				target.col = 1;
				colfound = true;
				break;
			case 4:
				target.col = 2;
				colfound = true;
				break;
			case 8:
				target.col = 3;
				colfound = true;
				break;
			case 16:
				target.col = 4;
				colfound = true;
				break;
			case 32:
				target.col = 5;
				colfound = true;
				break;
			case 64:
				target.col = 6;
				colfound = true;
				break;
			case 128:
				target.col = 7;
				colfound = true;
				break;
			default:
				// More than one button was pressed or no button pressed yet
				break;
		}
	}
	target.error = 0b00;
	return target;
}
