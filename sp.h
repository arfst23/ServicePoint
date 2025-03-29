//******************************************************************************

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define SP_TILE_SIZE 8
#define SP_TILES_HORIZ 56
#define SP_TILES_VERT 20
#define SP_SEPARATOR_WIDTH 4
#define SP_WIDTH (SP_TILES_HORIZ * SP_TILE_SIZE) // 448
#define SP_HEIGHT (SP_TILES_VERT * SP_TILE_SIZE) // 160
#define SP_HEIGHT_VIRT (SP_HEIGHT + (SP_TILES_VERT - 1) * SP_SEPARATOR_WIDTH) // 236

//******************************************************************************

void sp_create();
void sp_free();

void sp_send();
void sp_clear();
void sp_set(int y, int x, bool val);
uint8_t *get(int y, int col);

//******************************************************************************
