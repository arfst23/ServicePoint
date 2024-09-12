//******************************************************************************
 
#include "display_text.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define ROWS DISPLAY_TILES_VERT // 20
#define COLS DISPLAY_TILES_HORIZ // 56
#define SIZE DISPLAY_TILE_SIZE // 8

#define DELAY_ROLL 70

#define delay(ms) usleep(ms * 1000)

//******************************************************************************

char curr[ROWS][COLS];
char next[ROWS][COLS];

int main(int ac, char *av[])
{
  int display_select = 0;
  time_t timeout = 0;
  for (int ai = 1; ai < ac; ai++)
  {
    assert(av[ai][0] == '-');
    if (av[ai][1] == 's')
      display_select |= DISPLAY_SELECT_SP;
    else if (av[ai][1] == 'x')
      display_select |= DISPLAY_SELECT_GX;
    else if (isdigit(av[ai][1]))
      timeout = atoi(&av[ai][1]) + time(NULL);
    else
      assert(0);
  }
  display_create(display_select);

  memset(curr, ' ', ROWS * COLS);
  memset(next, ' ', ROWS * COLS);

  for (;;)
  {
    int row = 0;
    int col = 0;
    bool nl = false;

    for (;;)
    {
      if (timeout && time(NULL) > timeout)
	goto END;

      int c = getchar();
      if (c == EOF)
	goto END;

      if (c == '\n')
      {
	if (nl)
	  break;

	if (row < ROWS)
	  for (; col < COLS; col++)
	    next[row][col] = ' ';

	row++;
	col = 0;
	nl = true;
      }
      else
      {
	if (row < ROWS && col < COLS)
	  next[row][col] = c;
	col++;
	nl = false;
      }
    }

    for (int row = 0; row < ROWS; row++)
    {
      for (int i = 0; i < SIZE; i++)
      {
	for (int col = 0; col < COLS; col++)
	  display_chr_offset(row, col, curr[row][col],
	      next[row][col] == curr[row][col] ? 0 : -i);
	display_flush();
	delay(DELAY_ROLL);
      }

      for (int i = SIZE - 1; i >= 0; i--)
      {
	for (int col = 0; col < COLS; col++)
	  display_chr_offset(row, col, next[row][col],
	      next[row][col] == curr[row][col] ? 0 : i);
	display_flush();
	delay(DELAY_ROLL);
      }
    }

    memcpy(curr, next, ROWS * COLS);
  }

END:

  display_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
