#ifndef TARGETING
#define TARGETING

#include "bool.h"

typedef struct {
	// numbsers between 0 and 8; needs to be only 3 bits
	unsigned char row:3;
	unsigned char col:3;
} Target;

// Waits for two button presses and returns the target that is interpreted from that
Target determine_target(Board* board);

#endif
