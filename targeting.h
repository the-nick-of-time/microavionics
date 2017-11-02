#ifndef TARGETING
#define TARGETING

#include "bool.h"

typedef struct identifier {
	// numbsers between 0 and 8; needs to be only 3 bits
	unsigned char row:3;
	unsigned char col:3;
} Target;

Target determine_target(Board* board);

Cell extract_cell(Board* board, Target target);

#endif
