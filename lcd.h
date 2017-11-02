#ifndef LCD
#define LCD

#include "targeting.h"
#include "bool.h"

bool startup_lcd(void);

bool startup_glcd(void);

bool write_lcd_character(char c, unsigned char cursor);

bool write_lcd_string(char* s, unsigned char cursor);

bool draw_board(Board* board);

#endif
