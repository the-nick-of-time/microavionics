#include <p18cxxx.h>
#include "comms.h"
#include "board.h"

#define BR_9600 103

Target char_to_target(char c);
char target_to_char(Target t);
Handshake char_to_handshake(char c);
char handshake_to_char(Handshake h);

bool startup_usart(void){
	// Set tris bits
	TRISC = 0b10000000;
	// Configure transmit status for 8-bit async mode
	TXSTA1 = 0b00100110;
	// Configure recieve in 8-bit async continuous mode, turn it on
	RCSTA1 = 0b10010000;
	// Configure baud rate for 8-bit precision
	BAUDCON1 = 0b01000000;
	// Formula is FOSC/(16*(SPBRG+1))
	// FOSC = 16 MHz for this microcontroller
	// Baud rate is 9600
	SPBRG1 = BR_9600;
	// Transmit interrupt is low priority when it is enabled
	IPR1bits.TX1IP = 0;
	PIR1bits.TX1IF = 0;
	// Recieve interrupt is low priority when it is enabled
	PIR1bits.RC1IF = 0;
	IPR1bits.RC1IP = 0;
	return true;
}


bool tx_mode(void) {
	PIE1bits.TX1IE = 1;
	// Turn off recieve
	PIE1bits.RC1IE = 0;
	return MY_TURN;
}

bool rx_mode(void) {
	PIE1bits.RC1IE = 1;
	// Turn off send
	PIE1bits.TX1IE = 0;
	return THEIR_TURN;
}


bool send_target(Target t) {
	// TARGET NEEDS TO BE <=8 BITS, I DON'T FORSEE IT CHANGING THOUGH SO IT SHOULD BE FINE
	while (!PIR1bits.TX1IF) {}
	// ERROR
	TXREG1 = target_to_char(t);
	return true;
}

Target receive_target(void) {
	Target temp;
	PIR1bits.RC1IF = 0;
	if (FRAMING_ERROR) {
		// Clear and ignore due to framing error
		// ERROR
		temp = char_to_target(RCREG1);
		temp.error = 0b01;
		return temp;
	} else if (OVERRUN_ERROR) {
		// Reset due to overflow
		RCSTA1bits.CREN = 0;
		RCSTA1bits.CREN = 1;
		temp.error = 0b10;
		return temp;
	} else {
		// ERROR
		temp = char_to_target(RCREG1);
		temp.error = 0b00;
		return temp;
	}
}


bool end_game(Board* board) {
	int i;
	Cell* c;
	for (i = 0; i < WIDTH*HEIGHT; i++) {
		c = get_cell(board, i%WIDTH, i/WIDTH);
		if (c->occupied && !c->targeted){
			return false;
		}
	}
	return true;
}


Handshake send_confirmation(Board* board, Target targeted) {
	Cell* c;
	Handshake rv;
	c = get_cell(board, targeted.row, targeted.col);
	c->targeted = true;
	if (c->occupied) {
		rv.hit = true;
	}
	rv.gameover = end_game(board);
	rv.error = 0;
	// COPIED FROM SEND_TARGET
	while (!PIR1bits.TX1IF) {}
	// ERROR
	TXREG1 = handshake_to_char(rv);
	// /END COPIED CODE
	return rv;
}


Handshake receive_confirmation(Board* board, Target targeted) {
	Cell* c;
	Handshake h;
	// MINOR MODIFICATIONS FROM RECEIVE_TARGET
	PIR1bits.RC1IF = 0;
	if (FRAMING_ERROR) {
		// Clear and ignore due to framing error
		// ERROR
		h = char_to_handshake(RCREG1);
		h.error = 0b01;
		return h;
	} else if (OVERRUN_ERROR) {
		// Reset due to overflow
		RCSTA1bits.CREN = 0;
		RCSTA1bits.CREN = 1;
		h.error = 0b10;
		return h;
	} else {
		// ERROR
		h = char_to_handshake(RCREG1);
		h.error = 0b00;
	}
	// /END COPIED CODE
	c = get_cell(board, targeted.row, targeted.col);
	c->targeted = true;
	if (h.hit) {
		c->occupied = true;
	}
	return h;
}




Target char_to_target(char c) {
	Target rv;
	rv.row = c & 0x07;
	rv.col = (c & 0x38) >> 3;
	rv.error = (c & 0xC0) >> 6;
	return rv;
}

char target_to_char(Target t) {
	char rv;
	rv = t.row + (t.col << 3) + (t.error << 6);
	return rv;
}


Handshake char_to_handshake(char c) {
	Handshake rv;
	rv.hit = c & 0x01;
	rv.gameover = (c & 0x02) >> 1;
	rv.error = (c & 0xFC) >> 2;
	return rv;
}

char handshake_to_char(Handshake h) {
	char rv;
	rv = h.hit + (h.gameover << 1) + (h.error << 2);
	return rv;
}
