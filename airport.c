//******************************************************************************
 
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define ROWS (DISPLAY_TILES_VERT / 2) // 10
#define COLS 24
#define SIZE (2 * DISPLAY_TILE_SIZE) // 16

#define CHARS 37
#define FLIP_DELAY 40
#define FLIP_BREAK 80

#define delay(ms) usleep(ms * 1000)

//******************************************************************************
// >>> chars font

static const uint16_t font[CHARS + 1][16] =
{

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
//  0b1000000000000000,
//  0b1000000000000000,
//  0b1000000000000000,
//  0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000000111000000,
    0b1000000111000000,
    0b1000001101100000,
    0b1000011000110000,
    0b1000011000110000,
    0b1000111111111000,
//  0b1000111111111000,
//  0b1000111111111000,
//  0b1000111111111000,
//  0b1000111111111000,
    0b1000111111111000,
    0b1001100000001100,
    0b1001100000001100,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111100000,
    0b1011111111111000,
    0b1011000000001100,
    0b1011000000000110,
    0b1011000000001100,
    0b1011111111111000,
//  0b1011111111100000,
//  0b1011111111100000,
//  0b1011111111100000,
//  0b1011111111100000,
    0b1011111111111000,
    0b1011000000001100,
    0b1011000000000110,
    0b1011000000001100,
    0b1011111111111000,
    0b1011111111100000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000000110,
    0b1011000000000000,
    0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111000,
    0b1000011111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111000,
    0b1011111111111100,
    0b1011000000001100,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000001100,
    0b1011111111111100,
    0b1011111111111000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011111111111000,
//  0b1011111111111000,
//  0b1011111111111000,
//  0b1011111111111000,
//  0b1011111111111000,
    0b1011111111111000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011111111111000,
//  0b1011111111111000,
//  0b1011111111111000,
//  0b1011111111111000,
//  0b1011111111111000,
    0b1011111111111000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000000110,
    0b1011000000000000,
    0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
    0b1011000000111110,
    0b1011000000111110,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111000,
    0b1000011111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011111111111110,
//  0b1011111111111110,
//  0b1011111111111110,
//  0b1011111111111110,
//  0b1011111111111110,
    0b1011111111111110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000001111000000,
    0b1000001111000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000001111000000,
    0b1000001111000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000000000000110,
    0b1000000000000110,
    0b1000000000000110,
    0b1000000000000110,
    0b1000000000000110,
    0b1000000000000110,
//  0b1000000000000110,
//  0b1000000000000110,
//  0b1000000000000110,
//  0b1000000000000110,
    0b1000000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111000,
    0b1000011111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011000000011100,
    0b1011000001110000,
    0b1011000111000000,
    0b1011011100000000,
    0b1011110000000000,
//  0b1011110000000000,
//  0b1011110000000000,
//  0b1011110000000000,
//  0b1011110000000000,
    0b1011110000000000,
    0b1011011100000000,
    0b1011000111000000,
    0b1011000001110000,
    0b1011000000011100,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011100000001110,
    0b1011110000011110,
    0b1011011000110110,
    0b1011001101100110,
    0b1011000111000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011100000000110,
    0b1011110000000110,
    0b1011011000000110,
    0b1011011000000110,
    0b1011001100000110,
//  0b1011001100000110,
//  0b1011000110000110,
//  0b1011000011000110,
//  0b1011000001100110,
    0b1011000001100110,
    0b1011000000110110,
    0b1011000000110110,
    0b1011000000011110,
    0b1011000000001110,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111000,
    0b1000011111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111100000,
    0b1011111111111000,
    0b1011000000001100,
    0b1011000000000110,
    0b1011000000001100,
    0b1011111111111000,
//  0b1011111111100000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000010110,
    0b1001100000011100,
    0b1000111111111110,
    0b1000011111110010,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111100000,
    0b1011111111111000,
    0b1011000000001100,
    0b1011000000000110,
    0b1011000000001100,
    0b1011111111111000,
//  0b1011111111100000,
//  0b1011100000000000,
//  0b1011100000000000,
//  0b1011100000000000,
    0b1011110000000000,
    0b1011011100000000,
    0b1011000111000000,
    0b1011000001110000,
    0b1011000000011100,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111110,
    0b1000111111111110,
    0b1001100000000000,
    0b1011000000000000,
    0b1001100000000000,
    0b1000111100000000,
//  0b1000011110000000,
//  0b1000001111000000,
//  0b1000000111100000,
//  0b1000000011110000,
    0b1000000001111000,
    0b1000000000001100,
    0b1000000000000110,
    0b1000000000001100,
    0b1011111111111000,
    0b1011111111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111000,
    0b1000011111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1001100000001100,
    0b1001100000001100,
    0b1001100000001100,
//  0b1000110000011000,
//  0b1000110000011000,
//  0b1000110000011000,
//  0b1000110000011000,
    0b1000011000110000,
    0b1000011000110000,
    0b1000011000110000,
    0b1000001101100000,
    0b1000001101100000,
    0b1000000111000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
    0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
//  0b1011000000000110,
    0b1011000111000110,
    0b1011001101100110,
    0b1011011000110110,
    0b1011110000011110,
    0b1011100000001110,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1001100000001100,
    0b1000110000011000,
    0b1000011000110000,
    0b1000001101100000,
    0b1000000111000000,
//  0b1000000111000000,
//  0b1000000111000000,
//  0b1000000111000000,
//  0b1000000111000000,
    0b1000000111000000,
    0b1000001101100000,
    0b1000011000110000,
    0b1000110000011000,
    0b1001100000001100,
    0b1011000000000110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000110,
    0b1001100000001100,
    0b1000110000011000,
    0b1000011000110000,
    0b1000001101100000,
    0b1000000111000000,
//  0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
//  0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000110000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000000001100,
    0b1000000000011000,
    0b1000000000110000,
    0b1000000001100000,
//  0b1000000011000000,
//  0b1000000011000000,
//  0b1000000110000000,
//  0b1000000110000000,
    0b1000001100000000,
    0b1000011000000000,
    0b1000110000000000,
    0b1001100000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000010110,
    0b1011000000100110,
    0b1011000001000110,
//  0b1011000010000110,
//  0b1011000010000110,
//  0b1011000010000110,
//  0b1011000010000110,
    0b1011000100000110,
    0b1011001000000110,
    0b1011010000000110,
    0b1001100000001100,
    0b1000111111111000,
    0b1000011111110000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000000000111000,
    0b1000000011111000,
    0b1000001110011000,
    0b1000111000011000,
    0b1000000000011000,
    0b1000000000011000,
//  0b1000000000011000,
//  0b1000000000011000,
//  0b1000000000011000,
//  0b1000000000011000,
    0b1000000000011000,
    0b1000000000011000,
    0b1000000000011000,
    0b1000000000011000,
    0b1000000000011000,
    0b1000000000011000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1000000000000110,
    0b1000000000001100,
    0b1000000000011000,
//  0b1000000000110000,
//  0b1000000001100000,
//  0b1000000011000000,
//  0b1000000110000000,
    0b1000001100000000,
    0b1000011000000000,
    0b1000110000000000,
    0b1001100000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1000000000000110,
    0b1000000000001100,
    0b1000000000011000,
//  0b1000000000110000,
//  0b1000000000110000,
//  0b1000000000110000,
//  0b1000000000110000,
    0b1000000000011000,
    0b1000000000001100,
    0b1000000000000110,
    0b1001100000001100,
    0b1000111111111100,
    0b1000011111111000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000001100000,
    0b1011000001100000,
    0b1011000001100000,
    0b1011000001100000,
//  0b1011000001100000,
//  0b1011000001100000,
//  0b1011000001100000,
//  0b1011111111111110,
    0b1011111111111110,
    0b1000000001100000,
    0b1000000001100000,
    0b1000000001100000,
    0b1000000001100000,
    0b1000000001100000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011111111110000,
//  0b1011111111110000,
//  0b1000000000110000,
//  0b1000000000110000,
//  0b1000000000110000,
    0b1000000000011000,
    0b1000000000001100,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111100,
    0b1000011111111000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011100000000,
    0b1000110000000000,
    0b1001100000000000,
    0b1011000000000000,
    0b1011000000000000,
    0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011000000000000,
//  0b1011111111110000,
    0b1011111111111000,
    0b1011100000001100,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111100,
    0b1000011111111000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1011111111111110,
    0b1011111111111110,
    0b1000000000000110,
    0b1000000000001100,
    0b1000000000011000,
    0b1000000000110000,
//  0b1000000000110000,
//  0b1000000001100000,
//  0b1000000011000000,
//  0b1000000110000000,
    0b1000000110000000,
    0b1000001100000000,
    0b1000011000000000,
    0b1000110000000000,
    0b1001100000000000,
    0b1011000000000000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000000110,
    0b1001100000001100,
    0b1000110000011000,
//  0b1000011111110000,
//  0b1000011111110000,
//  0b1000011111110000,
//  0b1000011111110000,
    0b1000110000011000,
    0b1001100000001100,
    0b1011000000000110,
    0b1001100000001100,
    0b1000111111111100,
    0b1000011111111000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1111111111111111,
    0b1000000000000000,
    0b1000011111111000,
    0b1000111111111100,
    0b1001100000001100,
    0b1011000000000110,
    0b1001100000001110,
    0b1000111111111110,
//  0b1000011111111110,
//  0b1000000000000110,
//  0b1000000000000110,
//  0b1000000000000110,
    0b1000000000000110,
    0b1000000000000110,
    0b1000000000000110,
    0b1001100000001100,
    0b1000111111111100,
    0b1000011111111000,
    0b1000000000000000,
    0b1111111111111111,
  },

  {
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
//  0b1000000000000000,
//  0b1000000000000000,
//  0b1000000000000000,
//  0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
  },

};

// <<<
//******************************************************************************
// >>> lower upper

static void line(int row, int col, int y, uint16_t bits)
{
  assert(row >= 0);
  assert(row < ROWS);
  assert(col >= 0);
  assert(col < COLS);
  assert(y >= 0);
  assert(y < SIZE);
  
  int y0 = row * SIZE + y;
  int x0 = col * (SIZE + 3);
  for (int x = 0; x < SIZE; x++)
  {
    bool value = ((bits << x) & 0b1000000000000000) != 0;
    display_set(x0 + x, y0, value);
  }
  display_set(x0 + SIZE, y0, true);
}

static void upper(int row, int col, int n, int k)
{
  for (int i = 0; i < k; i++)
    line(row, col, SIZE / 2 - k + i, font[n][i * SIZE / 2 / k]);
}

static void lower(int row, int col, int n, int k)
{
  for (int i = 0; i < k; i++)
    line(row, col, SIZE / 2 + k - 1 - i, font[n][SIZE - 1 - i * SIZE / 2 / k]);
}

// <<<
//******************************************************************************
// >>> full flip

static void full(int row, int col, int n)
{
  upper(row, col, n, 8);
  lower(row, col, n, 8);
}

static void flip(int row, int col, int src, int dst, int i)
{
  switch (i)
  {
  case 0:
    upper(row, col, dst, 8);
    upper(row, col, src, 7);
    lower(row, col, src, 8);
    break;

  case 1:
    upper(row, col, dst, 8);
    upper(row, col, src, 6);
    lower(row, col, src, 8);
    break;

  case 2:
    upper(row, col, dst, 8);
    upper(row, col, src, 4);
    lower(row, col, src, 8);
    break;

  case 3:
    upper(row, col, dst, 8);
    upper(row, col, src, 1);
    lower(row, col, src, 8);
    lower(row, col, dst, 1);
    break;

  case 4:
    upper(row, col, dst, 8);
    lower(row, col, src, 8);
    lower(row, col, dst, 4);
    break;

  case 5:
    upper(row, col, dst, 8);
    lower(row, col, src, 8);
    lower(row, col, dst, 6);
    break;

  case 6:
    upper(row, col, dst, 8);
    lower(row, col, src, 8);
    lower(row, col, dst, 7);
    break;

  default:
    upper(row, col, dst, 8);
    lower(row, col, dst, 8);
    break;
  }
}

// <<<
//******************************************************************************
// >>> clr put change

static int current[ROWS][COLS];
static int target[ROWS][COLS];

static void init()
{
  for (int row = 0; row < ROWS; row++)
  {
    for (int col = 0; col < COLS - 1; col++)
      full(row, col, 0);
    //full(row, COLS - 1, CHARS);
  }
  display_flush();

  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS - 1; col++)
    {
      current[row][col] = 0;
      target[row][col] = 0;
    }
}

static void change()
{
  for (;;)
  {
    for (int row = 0; row < ROWS; row++)
      for (int col = 0; col < COLS - 1; col++)
	if (current[row][col] != target[row][col])
	  goto DONE;
    break;

  DONE:
    for (int i = 0; i < 8; i++)
    {
      for (int row = 0; row < ROWS; row++)
	for (int col = 0; col < COLS - 1; col++)
	  if (current[row][col] != target[row][col])
	  {
	    int src = current[row][col];
	    int dst = (src + 1) % CHARS;
	    flip(row, col, src, dst, i);
	  }
      display_flush();
      delay(FLIP_DELAY);
    }
    delay(FLIP_BREAK);
    
    for (int row = 0; row < ROWS; row++)
      for (int col = 0; col < COLS - 1; col++)
	if (current[row][col] != target[row][col])
	{
	  int src = current[row][col];
	  int dst = (src + 1) % CHARS;
	  current[row][col] = dst;
	}
  }
}

static void put(int row, int col, char c)
{
  if (c >= 'A' && c <= 'Z')
    target[row][col] = c - 'A' + 1;
  else if (c >= 'a' && c <= 'z')
    target[row][col] = c - 'a' + 1;
  else if (c >= '0' && c <= '9')
    target[row][col] = c - '0' + 1 + 26;
  else
    target[row][col] = 0;
}

static void clr()
{
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS - 1; col++)
      target[row][col] = 0;
}

// <<<
//******************************************************************************

int main(int ac, char *av[])
{
  int display_select = 0;
  int timeout = 0;
  for (int ai = 1; ai < ac; ai++)
  {
    assert(av[ai][0] == '-');
    if (av[ai][1] == 's')
      display_select |= DISPLAY_SELECT_SP;
    else if (av[ai][1] == 'x')
      display_select |= DISPLAY_SELECT_GX;
    else if (isdigit(av[ai][1]))
      timeout = atoi(&av[ai][1]) * 36;
    else
      assert(0);
  }
  display_create(display_select);

  init();

  for (;;)
  {
    int row = 0;
    int col = 0;
    bool nl = false;

    for (;;)
    {
      int c = getchar();
      if (c == EOF)
	goto END;

      if (c == '\n')
      {
	if (nl)
	  break;

	row++;
	col = 0;
	nl = true;
      }
      else
      {
	if (row < ROWS && col < COLS)
	  put(row, col, c);
	col++;
	nl = false;
      }
    }

    change();
    clr();
  }

END:
  clr();
  change();

  timeout=timeout;

  display_free();
  return EXIT_SUCCESS;
}

//******************************************************************************