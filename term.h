//******************************************************************************

#pragma once

#define TERM_WHITE 0
#define TERM_RED 1
#define TERM_YELLOW 2
#define TERM_GREEN 3
#define TERM_CYAN 4
#define TERM_MAGENTA 5

//******************************************************************************

void term_clear(void);
void term_home(void);
void term_color(int color);

void term_raw(void);
void term_restore(void);

//******************************************************************************
