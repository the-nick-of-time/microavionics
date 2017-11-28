#ifndef COMMUNICATIONS
#define COMMUNICATIONS

#include "targeting.h"
#include "bool.h"
#include "board.h"

#define BYTE_RECIEVED PIR1bits.RC1IF
#define FRAMING_ERROR RCSTA1bits.FERR
#define OVERRUN_ERROR RCSTA1bits.OERR

// Tries tp a usart communication line to the other board and returns whether it was successful
bool startup_usart(void);

// switch to transmit mode (your turn) or recieve mode (other person's turn)
bool tx_mode(void);
bool rx_mode(void);

// send cell that is being targeted
bool send_target(Target tosend);

// recieve cell that is being targeted
Target receive_target(void);

// Confirmation says that 1) the data was successfully received and 2) whether the shot was a hit or a miss
typedef struct {
	unsigned hit:1;
	unsigned error:7;
} Handshake;
Handshake send_confirmation(Board* board, Target targeted);
Handshake receive_confirmation(Board* board, Target targeted);

#endif
