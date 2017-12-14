#ifndef TARGETING
#define TARGETING
#include "bool.h"

#define ROWPORT PORTJ
#define COLPORT PORTE
#define ROWTRIS TRISJ
#define COLTRIS TRISE

typedef struct {
	// numbsers between 0 and 8; needs to be only 3 bits
	unsigned char row:3;
	unsigned char col:3;
	unsigned char error:2;
} Target;

// Waits for two button presses and returns the target that is interpreted from that
Target determine_target(void);

bool startup_targeting(void);

// Sending a target and recieving a target are covered in comms instead

#endif
