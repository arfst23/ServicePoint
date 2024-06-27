//******************************************************************************
 
#pragma once

#include <stdbool.h>

#define DISPLAY_TILE_SIZE 8
#define DISPLAY_TILES_HORIZ 56
#define DISPLAY_TILES_VERT 20
#define DISPLAY_SEPARATOR_WIDTH 4
#define DISPLAY_WIDTH (DISPLAY_TILES_HORIZ * DISPLAY_TILE_SIZE) // 448
#define DISPLAY_HEIGHT (DISPLAY_TILES_VERT * DISPLAY_TILE_SIZE) // 160
#define DISPLAY_HEIGHT_VIRT (DISPLAY_HEIGHT \
  + (DISPLAY_TILES_VERT - 1) * DISPLAY_SEPARATOR_WIDTH) // 236

#define DISPLAY_SELECT_GX 1
#define DISPLAY_SELECT_SP 2
 
//******************************************************************************
 
void display_create(int select);
void display_free(void);

void display_set(int x, int y, bool value);
void display_set_virt(int x, int y, bool value);
void display_clear(void);
void display_flush(void);

int display_button(void);
int display_wait(void);

//******************************************************************************
