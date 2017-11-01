#ifndef COMMUNICATIONS
#define COMMUNICATIONS

bool startup_usart(void);

// send cell that is being targeted
void send_target(Target tosend);

// recieve cell that is being targeted
Target recieve_target(void);

#endif
