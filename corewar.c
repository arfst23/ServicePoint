//******************************************************************************

#include "display_text.h"
#include "display.h"

#include <stdio.h>

#include <stdlib.h>
#include <assert.h>

#define HEIGHT (DISPLAY_TILES_VERT * 3) // 60
#define WIDTH ((DISPLAY_WIDTH - 13) / 3) // 145
#define LENGTH (WIDTH * HEIGHT) // 8700

//******************************************************************************

int main(int ac, const char *av[])
{
  int display_select = 0;
  for (int ai = 1; ai < ac; ai++)
  {
    assert(av[ai][0] == '-');
    switch (av[ai][1])
    {
    case 'x':
      display_select |= DISPLAY_SELECT_GX;
      break;
    case 's':
      display_select |= DISPLAY_SELECT_SP;
      break;
    default:
      assert(0);
    }
  }

  display_create(display_select);

  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
    {
      int yy = 3 * y - y / 3;
      int xx = 3 * x;
      display_set(xx, yy, true);
    }

  display_chr(0, DISPLAY_COLS - 1, '0', false);
  display_chr(1, DISPLAY_COLS - 1, '1', false);
  display_chr(3, DISPLAY_COLS - 1, 'A', false);
  display_chr(4, DISPLAY_COLS - 1, 'L', false);
  display_chr(5, DISPLAY_COLS - 1, 'U', false);
  display_chr(6, DISPLAY_COLS - 1, '2', false);
  display_chr(7, DISPLAY_COLS - 1, '3', false);

  display_chr(DISPLAY_ROWS - 9, DISPLAY_COLS - 1, 'F', false);
  display_chr(DISPLAY_ROWS - 8, DISPLAY_COLS - 1, 'O', false);
  display_chr(DISPLAY_ROWS - 7, DISPLAY_COLS - 1, 'O', false);
  display_chr(DISPLAY_ROWS - 6, DISPLAY_COLS - 1, 'B', false);
  display_chr(DISPLAY_ROWS - 5, DISPLAY_COLS - 1, 'A', false);
  display_chr(DISPLAY_ROWS - 4, DISPLAY_COLS - 1, 'R', false);
  display_chr(DISPLAY_ROWS - 2, DISPLAY_COLS - 1, '0', false);
  display_chr(DISPLAY_ROWS - 1, DISPLAY_COLS - 1, '0', false);

  for (int x = 50; x < 50 + 16; x++)
  {
    int y = 12;
    int yy = 3 * y - y / 3;
    int xx = 3 * x + 1;
    display_set(xx, yy, true);
  }

  for (int x = 100; x < 100 + 24; x++)
  {
    int y = 24;
    int yy = 3 * y - y / 3 + 1;
    int xx = 3 * x;
    display_set(xx, yy, true);
  }

  display_flush();
  display_wait();
  display_free();
  return EXIT_SUCCESS;
}

//******************************************************************************

