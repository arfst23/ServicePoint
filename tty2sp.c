//******************************************************************************

#include "display_text.h"
#include "display.h"

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define BUFFER_LENGTH 80
 
static char screen[DISPLAY_ROWS][DISPLAY_COLS];

//******************************************************************************

static void screen_clear(void)
{
  for (int row = 0; row < DISPLAY_ROWS; row++)
    for (int col = 0; col < DISPLAY_COLS; col++)
      screen[row][col] = ' ';
}

static void screen_scroll()
{
  for (int row = 0; row < DISPLAY_ROWS - 1; row++)
    for (int col = 0; col < DISPLAY_COLS; col++)
      screen[row][col] = screen[row + 1][col];
  for (int col = 0; col < DISPLAY_COLS; col++)
    screen[DISPLAY_ROWS - 1][col] = ' ';
}

static void screen_display()
{
  for (int row = 0; row < DISPLAY_ROWS; row++)
    for (int col = 0; col < DISPLAY_COLS; col++)
      display_chr(row, col, screen[row][col], false);
}

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

  screen_clear();
  int cursor_row = 0;
  int cursor_col = 0;

  display_create(display_select);
  display_chr(cursor_row, cursor_col, ' ', true);
  display_flush();

  for (;;)
  {
    char buffer[BUFFER_LENGTH];

    int n = read(STDIN_FILENO, buffer, BUFFER_LENGTH);
    if (!n)
      break;

    for (int i = 0; i < n; i++)
      if (buffer[i] == '\r')
      {
	if (cursor_col == 0)
	  continue;
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], false);
	cursor_col = 0;
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], true);
      }
      else if (buffer[i] == '\n')
      {
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], false);
	cursor_col = 0;
	if (cursor_row < DISPLAY_ROWS - 1)
	  cursor_row += 1;
        else
	{
	  screen_scroll();
	  display_clear();
	  screen_display(cursor_row, cursor_col);
	}
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], true);
      }
      else if (buffer[i] == '\b')
      {
	if (cursor_col == 0)
	  continue;
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], false);
	cursor_col -= 1;
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], true);
      }
      else if (buffer[i] == '\f')
      {
	display_clear();
	screen_clear();
	cursor_row = 0;
	cursor_col = 0;
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], true);
      }
      else
      {
	screen[cursor_row][cursor_col] = buffer[i];
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], false);

	if (cursor_col < DISPLAY_COLS - 1)
	  cursor_col += 1;
	else
	{
	  cursor_col = 0;
	  if (cursor_row < DISPLAY_ROWS - 1)
	    cursor_row += 1;
	  else
	  {
	    screen_scroll();
	    display_clear();
	    screen_display(cursor_row, cursor_col);
	  }
	}
	display_chr(cursor_row, cursor_col, screen[cursor_row][cursor_col], true);
      }
	
    if (display_button())
      break;
    
    display_flush();
    usleep(50000); // 50ms
  }

  display_wait();
  display_free();
}

//******************************************************************************
