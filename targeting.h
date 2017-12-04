#ifndef TARGETING
#define TARGETING

typedef struct {
	// numbsers between 0 and 8; needs to be only 3 bits
	unsigned char row:3;
	unsigned char col:3;
	unsigned char error:2;
} Target;

// Waits for two button presses and returns the target that is interpreted from that
Target determine_target(void);

// Sending a target and recieving a target are covered in comms instead

#endif
