#ifndef COMMUNICATIONS
#define COMMUNICATIONS

#include "targeting.h"
#include "bool.h"

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
bool send_confirmation(Board* board, Target targeted);
bool receive_confirmation(Board* board, Target targeted);

#endif
