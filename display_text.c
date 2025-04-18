//******************************************************************************
 
#include "display_text.h"
#include "display.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//******************************************************************************
// >>> font[96][8]

static const uint8_t font[96][8] =
{
  { // space
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  { // !
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
    0b00100,
    0b00000,
  },
  { // "
    0b01010,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  { // #
    0b00000,
    0b01010,
    0b11111,
    0b01010,
    0b01010,
    0b11111,
    0b01010,
    0b00000,
  },
  { // $
    0b00100,
    0b01111,
    0b10100,
    0b01110,
    0b00101,
    0b11110,
    0b00100,
    0b00000,
  },
  { // %
    0b11001,
    0b11001,
    0b00010,
    0b00100,
    0b01000,
    0b10011,
    0b10011,
    0b00000,
  },
  { // &
    0b01000,
    0b10100,
    0b10100,
    0b01000,
    0b10101,
    0b10010,
    0b01101,
    0b00000,
  },
  { // '
    0b00100,
    0b00100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  { // (
    0b00010,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00010,
    0b00000,
  },
  { // )
    0b01000,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01000,
    0b00000,
  },
  { // *
    0b00000,
    0b01010,
    0b00100,
    0b11111,
    0b00100,
    0b01010,
    0b00000,
    0b00000,
  },
  { // +
    0b00000,
    0b00100,
    0b00100,
    0b11111,
    0b00100,
    0b00100,
    0b00000,
    0b00000,
  },
  { // ,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b01100,
    0b00100,
    0b01000,
  },
  { // -
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  { // .
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b01100,
    0b01100,
    0b00000,
  },
  { // /
    0b00000,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b00000,
    0b00000,
  },
  { // 0
    0b01110,
    0b10001,
    0b10011,
    0b10101,
    0b11001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // 1
    0b00100,
    0b01100,
    0b10100,
    0b00100,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  { // 2
    0b01110,
    0b10001,
    0b00001,
    0b00110,
    0b01000,
    0b10000,
    0b11111,
    0b00000,
  },
  { // 3
    0b01110,
    0b10001,
    0b00001,
    0b00110,
    0b00001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // 4
    0b00010,
    0b00110,
    0b01010,
    0b10010,
    0b11111,
    0b00010,
    0b00111,
    0b00000,
  },
  { // 5
    0b11111,
    0b10000,
    0b11110,
    0b00001,
    0b00001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // 6
    0b00111,
    0b01000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // 7
    0b11111,
    0b00001,
    0b00010,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  { // 8
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // 9
    0b01110,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b00001,
    0b01110,
    0b00000,
  },
  { // :
    0b00000,
    0b00000,
    0b01100,
    0b01100,
    0b00000,
    0b01100,
    0b01100,
    0b00000,
  },
  { // ;
    0b00000,
    0b00000,
    0b01100,
    0b01100,
    0b00000,
    0b01100,
    0b00100,
    0b01000,
  },
  { // <
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00000,
  },
  { // =
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
  },
  { // >
    0b01000,
    0b00100,
    0b00010,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b00000,
  },
  { // ?
    0b01110,
    0b10001,
    0b00001,
    0b00110,
    0b00100,
    0b00000,
    0b00100,
    0b00000,
  },
  { // @
    0b01110,
    0b10001,
    0b10111,
    0b10101,
    0b10111,
    0b10000,
    0b01110,
    0b00000,
  },
  { // A
    0b01110,
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  { // B
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b00000,
  },
  { // C
    0b01110,
    0b10001,
    0b10000,
    0b10000,
    0b10000,
    0b10001,
    0b01110,
    0b00000,
  },
  { // D
    0b11110,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11110,
    0b00000,
  },
  { // E
    0b11111,
    0b10000,
    0b10000,
    0b11110,
    0b10000,
    0b10000,
    0b11111,
    0b00000,
  },
  { // F
    0b11111,
    0b10000,
    0b10000,
    0b11110,
    0b10000,
    0b10000,
    0b10000,
    0b00000,
  },
  { // G
    0b01110,
    0b10001,
    0b10000,
    0b10000,
    0b10011,
    0b10001,
    0b10001,
    0b01110,
  },
  { // H
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  { // I
    0b01110,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  { // J
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // K
    0b10001,
    0b10010,
    0b10100,
    0b11000,
    0b10100,
    0b10010,
    0b10001,
    0b00000,
  },
  { // L
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    0b00000,
  },
  { // M
    0b10001,
    0b11011,
    0b10101,
    0b10101,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  { // N
    0b10001,
    0b11001,
    0b10101,
    0b10101,
    0b10011,
    0b10001,
    0b10001,
    0b00000,
  },
  { // O
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // P
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10000,
    0b10000,
    0b10000,
    0b00000,
  },
  { // Q
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b10101,
    0b10010,
    0b01101,
    0b00000,
  },
  { // R
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10010,
    0b10001,
    0b10001,
    0b00000,
  },
  { // S
    0b01110,
    0b10001,
    0b10000,
    0b01110,
    0b00001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // T
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  { // U
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // V
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b00000,
  },
  { // W
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10101,
    0b10101,
    0b01010,
    0b00000,
  },
  { // X
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b10001,
    0b00000,
  },
  { // Y
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  { // Z
    0b11111,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b11111,
    0b00000,
  },
  { // [
    0b00110,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00110,
    0b00000,
  },
  { // backslash
    0b00000,
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00001,
    0b00000,
    0b00000,
  },
  { // ]
    0b01100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01100,
    0b00000,
  },
  { // ^
    0b00100,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  { // _
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
  },
  { // `
    0b01000,
    0b00100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  { // a
    0b00000,
    0b00000,
    0b01111,
    0b00001,
    0b01111,
    0b10001,
    0b01111,
    0b00000,
  },
  { // b
    0b10000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b10001,
    0b11110,
    0b00000,
  },
  { // c
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b10000,
    0b10001,
    0b01110,
    0b00000,
  },
  { // d
    0b00001,
    0b00001,
    0b01111,
    0b10001,
    0b10001,
    0b10001,
    0b01111,
    0b00000,
  },
  { // e
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b11111,
    0b10000,
    0b01110,
    0b00000,
  },
  { // f
    0b00110,
    0b01000,
    0b01000,
    0b11100,
    0b01000,
    0b01000,
    0b01000,
    0b00000,
  },
  { // g
    0b00000,
    0b00000,
    0b01111,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b01110,
  },
  { // h
    0b10000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  { // i
    0b00000,
    0b00100,
    0b00000,
    0b01110,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  { // j
    0b00000,
    0b00010,
    0b00000,
    0b00110,
    0b00010,
    0b00010,
    0b10010,
    0b01100,
  },
  { // k
    0b10000,
    0b10000,
    0b10010,
    0b10100,
    0b11000,
    0b10100,
    0b10010,
    0b00000,
  },
  { // l
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b00110,
    0b00000,
  },
  { // m
    0b00000,
    0b00000,
    0b11010,
    0b10101,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  { // n
    0b00000,
    0b00000,
    0b11100,
    0b10010,
    0b10010,
    0b10010,
    0b10010,
    0b00000,
  },
  { // o
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  { // p
    0b00000,
    0b00000,
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10000,
    0b10000,
  },
  { // q
    0b00000,
    0b00000,
    0b01111,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b00010,
  },
  { // r
    0b00000,
    0b00000,
    0b10110,
    0b01001,
    0b01000,
    0b01000,
    0b11100,
    0b00000,
  },
  { // s
    0b00000,
    0b00000,
    0b01110,
    0b10000,
    0b01110,
    0b00001,
    0b01110,
    0b00000,
  },
  { // t
    0b01000,
    0b01000,
    0b11100,
    0b01000,
    0b01000,
    0b01000,
    0b00110,
    0b00000,
  },
  { // u
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10011,
    0b01101,
    0b00000,
  },
  { // v
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b00000,
  },
  { // w
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10101,
    0b01010,
    0b00000,
  },
  { // x
    0b00000,
    0b00000,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b00000,
  },
  { // y
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b11000,
  },
  { // z
    0b00000,
    0b00000,
    0b11111,
    0b00001,
    0b01110,
    0b10000,
    0b11111,
    0b00000,
  },
  { // {
    0b00010,
    0b00100,
    0b00100,
    0b01100,
    0b00100,
    0b00100,
    0b00010,
    0b00000,
  },
  { // |
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  { // }
    0b01000,
    0b00100,
    0b00100,
    0b00110,
    0b00100,
    0b00100,
    0b01000,
    0b00000,
  },
  { // ~
    0b00000,
    0b00000,
    0b01010,
    0b10100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
};

// <<<
//******************************************************************************

void display_chr(int row, int col, char c, bool rev)
{
  assert(row >= 0);
  assert(row < DISPLAY_ROWS);
  assert(col >= 0);
  assert(col < DISPLAY_COLS);
  assert(c >= ' ');
  assert(c <= '~');

  int x = col * DISPLAY_TILE_SIZE;
  int y = row * DISPLAY_TILE_SIZE;
  int i = c - ' ';

  for (int line = 0; line < 8; line++)
  {
    uint8_t mask = font[i][line] << 1;
    if (rev)
      mask = ~mask;
    for (int bit = 0; bit < 7; bit++)
    {
      bool pixel = mask & 0b1000000;
      display_set(x + bit, y + line, pixel);
      mask <<= 1;
    }
  }
}

//******************************************************************************

void display_chr_offset(int row, int col, char c, int off)
{
  assert(row >= 0);
  assert(row < DISPLAY_ROWS);
  assert(col >= 0);
  assert(col < DISPLAY_COLS);
  assert(c >= ' ');
  assert(c <= '~');

  int x = col * DISPLAY_TILE_SIZE;
  int y = row * DISPLAY_TILE_SIZE;
  int i = c - ' ';

  for (int line = 0; line < 8; line++)
  {
    int l = line + off;
    uint8_t mask = l >= 0 && l < 8 ? font[i][l] << 1 : 0;
    for (int bit = 0; bit < 7; bit++)
    {
      bool pixel = mask & 0b1000000;
      display_set(x + bit, y + line, pixel);
      mask <<= 1;
    }
  }
}

//******************************************************************************

void display_str(int row, int col, const char *str)
{
  assert(row >= 0);
  assert(row < DISPLAY_ROWS);
  assert(col >= 0);
  assert(str);
  int len = strlen(str);
  assert(col + len < DISPLAY_COLS);

  for (int i = 0; i < len; i++)
    display_chr(row, col + i, str[i], false);
}

//******************************************************************************