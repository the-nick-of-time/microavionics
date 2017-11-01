#ifndef LCD
#define LCD

#include "targeting.h"

bool startup_lcd(void);

bool startup_glcd(void);

bool set_lcd_character(char c, unsigned char cursor);

bool draw_board(Board* board);

#endif
