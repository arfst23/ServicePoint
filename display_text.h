//******************************************************************************
 
#pragma once

#include "display.h"

#define DISPLAY_ROWS DISPLAY_TILES_VERT
#define DISPLAY_COLS DISPLAY_TILES_HORIZ

//******************************************************************************
 
void display_chr(int row, int col, char c, bool rev);
void display_str(int row, int col, const char *str);

//******************************************************************************
