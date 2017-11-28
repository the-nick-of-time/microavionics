#include "comms.h"


Target receive_target(void) {
	Target rec;
	while (1) {
		if (BYTE_RECIEVED) {
			if (FRAMING_ERROR) {
				rec = RCREG1;
				// Ignore the byte
				continue;
			} else if (OVERRUN_ERROR) {
				RCSTA1bits.CREN = 0;
				RCSTA1bits.CREN = 1;
				// Ignore the byte
				continue;
			} else {
				// Successful receipt
				break;
			}
		}
	}
	// rec = receive_target();
	rec = (Target) RCREG1;
	return rec;
}
