//******************************************************************************
 
#include "display.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define OFFSET_X ((DISPLAY_TILES_HORIZ - 6 * 2) * DISPLAY_TILE_SIZE / 2)
#define OFFSET_Y (4 * DISPLAY_TILE_SIZE)
#define BORDER_WIDTH 4

//******************************************************************************
// >>> symbol_bits[5][16]

static const uint16_t symbol_bits[5][16] =
{
  { // 0
    0b1111111111111111,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
 // 0b1000000000000001,
 // 0b1000000000000001,
 // 0b1000000000000001,
 // 0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1111111111111111
  },
  { // 1
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0011000000001100,
    0b0011000000001100,
    0b0001100000011000,
    0b0000110000110000,
    0b0000110000110000,
 // 0b0000001111000000,
 // 0b0000001111000000,
 // 0b0000001111000000,
 // 0b0000001111000000,
    0b0000011001100000,
    0b0000110000110000,
    0b0000110000110000,
    0b0001100000011000,
    0b0011000000001100,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
  },
  { // 2
    0b0000000000000000,
    0b0000000000000000,
    0b0011000000001100,
    0b0011100000011100,
    0b0001110000111000,
    0b0001110000111000,
    0b0000111001110000,
    0b0000111001110000,
 // 0b0000011111100000,
 // 0b0000001111000000,
 // 0b0000001111000000,
 // 0b0000011111100000,
    0b0000111001110000,
    0b0000111001110000,
    0b0001110000111000,
    0b0001110000111000,
    0b0011100000011100,
    0b0011000000001100,
    0b0000000000000000,
    0b0000000000000000
  },
  { // 3
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000001111000000,
    0b0000011111100000,
    0b0000110000110000,
    0b0000110000110000,
    0b0001100000011000,
 // 0b0001100000011000,
 // 0b0001100000011000,
 // 0b0001100000011000,
 // 0b0001100000011000,
    0b0001100000011000,
    0b0001110000110000,
    0b0001110000110000,
    0b0000111111100000,
    0b0000001111000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
  },
  { // 4
    0b0000000000000000,
    0b0000000000000000,
    0b0000001111000000,
    0b0000011111100000,
    0b0000111111110000,
    0b0001110000111000,
    0b0001110000111000,
    0b0011100000011100,
 // 0b0011100000011100,
 // 0b0011100000011100,
 // 0b0011100000011100,
 // 0b0011100000011100,
    0b0011100000011100,
    0b0001110000111000,
    0b0001110000111000,
    0b0000111111110000,
    0b0000011111100000,
    0b0000001111000000,
    0b0000000000000000,
    0b0000000000000000
  }
};

// <<<
//******************************************************************************

static void init()
{
  for (int i = 1; i <= BORDER_WIDTH; i++)
    for (int x = OFFSET_X - BORDER_WIDTH;
	x < OFFSET_X + 6 * 2 * DISPLAY_TILE_SIZE + BORDER_WIDTH; x++)
    {
      display_set(x, OFFSET_Y - i, true);
      display_set(x, OFFSET_Y + 6 * 2 * DISPLAY_TILE_SIZE + i, true);
    }

  for (int i = 1; i <= BORDER_WIDTH; i++)
    for (int y = OFFSET_Y; y < OFFSET_Y + 6 * 2 * DISPLAY_TILE_SIZE; y++)
    {
      display_set(OFFSET_X - i, y, true);
      display_set(OFFSET_X + 6 * 2 * DISPLAY_TILE_SIZE + i - 1, y, true);
    }

  display_flush();
}

static void draw_symbol(int x, int y, int symbol, bool frame)
{
  for (int row = 0; row < 2 * DISPLAY_TILE_SIZE; row++)
  {
    uint16_t bits = symbol ? symbol_bits[symbol][row] : 0;
    if (frame)
      bits |= symbol_bits[0][row];
    for (int col = 0; col < 2 * DISPLAY_TILE_SIZE; col++)
      display_set(x + col, y + row, (bits << col) & 0b1000000000000000);
  }
}

static void draw_board_symbol(int row, int col, int symbol)
{
  int x = OFFSET_X + col * 2 * DISPLAY_TILE_SIZE;
  int y = OFFSET_Y + row * 2 * DISPLAY_TILE_SIZE;
  draw_symbol(x, y, symbol, true);
}



//******************************************************************************

int main(int ac, const char *av[])
{
  int display_select
      = ac == 1 ? DISPLAY_SELECT_GX | DISPLAY_SELECT_SP
      : av[1][0] == 'g' ? DISPLAY_SELECT_GX
      : av[1][0] == 's' ? DISPLAY_SELECT_SP
      : 0;
  display_create(display_select);

  init();
  for (int row = 0; row < 6; row++)
    for (int col = 0; col < 6; col++)
      draw_board_symbol(row, col, (row + col) % 5);

  display_wait();
  display_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
