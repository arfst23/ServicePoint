//******************************************************************************
 
#pragma once

#include "display.h"

#define DISPLAY_ROWS DISPLAY_TILES_VERT
#define DISPLAY_COLS DISPLAY_TILES_HORIZ

//******************************************************************************
 
void display_chr(int row, int col, char c, bool rev);
void display_chr_offset(int row, int col, char c, int off);
void display_str(int row, int col, const char *str);

//******************************************************************************
