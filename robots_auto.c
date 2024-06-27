//******************************************************************************

#include "robots_auto.h"
#include "robots.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TO_X 0
#define TO_N 1
#define TO_NW 2
#define TO_W 3
#define TO_SW 4
#define TO_S 5
#define TO_SE 6
#define TO_E 7
#define TO_NE 8

#define NIL 0

//******************************************************************************
// >>> validity

static const char* validity(const char board[SCREEN_ROWS][SCREEN_COLS], int row, int col)
{
  static char valid[9];

  valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W] = valid[TO_SW]
    = valid[TO_S] = valid[TO_SE] = valid[TO_E] = valid[TO_NE] = 1;

  // borders
  if (row == 0)
    valid[TO_N] = valid[TO_NW] = valid[TO_NE] = 0;
  if (row == SCREEN_ROWS - 1)
    valid[TO_SW] = valid[TO_S] = valid[TO_SE] = 0;
  if (col == 0)
    valid[TO_NW] = valid[TO_W] = valid[TO_SW] = 0;
  if (col == SCREEN_COLS - 1)
    valid[TO_SE] = valid[TO_E] = valid[TO_NE] = 0;

  // 8 primary neighbors
  if (row > 0)
  {
    if (board[row - 1][col] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W]
	= valid[TO_E] = valid[TO_NE] = 0;
    else if (board[row - 1][col] == ROBOTS_JUNK)
      valid[TO_N] = 0;
  }

  if (row > 0 && col > 0)
  {
    if (board[row - 1][col - 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W] = 0;
    else if (board[row - 1][col - 1] == ROBOTS_JUNK)
      valid[TO_NW] = 0;
  }

  if (col > 0)
  {
    if (board[row][col - 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W]
	= valid[TO_SW] = valid[TO_S] = 0;
    else if (board[row][col - 1] == ROBOTS_JUNK)
      valid[TO_W] = 0;
  }

  if (row < SCREEN_ROWS - 1 && col > 0)
  {
    if (board[row + 1][col - 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_W] = valid[TO_SW] = valid[TO_S] = 0;
    else if (board[row + 1][col - 1] == ROBOTS_JUNK)
      valid[TO_SW] = 0;
  }

  if (row < SCREEN_ROWS - 1)
  {
    if (board[row + 1][col] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_W] = valid[TO_SW] = valid[TO_S]
	= valid[TO_SE] = valid[TO_E] = 0;
    else if (board[row + 1][col] == ROBOTS_JUNK)
      valid[TO_S] = 0;
  }

  if (row < SCREEN_ROWS - 1 && col < SCREEN_COLS - 1)
  {
    if (board[row + 1][col + 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_S] = valid[TO_SE] = valid[TO_E] = 0;
    else if (board[row + 1][col + 1] == ROBOTS_JUNK)
      valid[TO_SE] = 0;
  }

  if (col < SCREEN_COLS - 1)
  {
    if (board[row][col + 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_S] = valid[TO_SE]
	= valid[TO_E] = valid[TO_NE] = 0;
    else if (board[row][col + 1] == ROBOTS_JUNK)
      valid[TO_E] = 0;
  }

  if (row > 0 && col < SCREEN_COLS - 1)
  {
    if (board[row - 1][col + 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_E] = valid[TO_NE] = 0;
    else if (board[row - 1][col + 1] == ROBOTS_JUNK)
      valid[TO_NE] = 0;
  }

  // 16 secondary neightbors
  if (row > 1
    && board[row - 2][col] == ROBOTS_ROBOT)
    valid[TO_N] = valid[TO_NW] = valid[TO_NE] = 0;

  if (row > 1 && col > 0
    && board[row - 2][col - 1] == ROBOTS_ROBOT)
    valid[TO_N] = valid[TO_NW] = 0;

  if (row > 1 && col > 1
    && board[row - 2][col - 2] == ROBOTS_ROBOT)
    valid[TO_NW] = 0;

  if (row > 0 && col > 1
    && board[row - 1][col - 2] == ROBOTS_ROBOT)
    valid[TO_NW] = valid[TO_W] = 0;

  if (col > 1
    && board[row][col - 2] == ROBOTS_ROBOT)
    valid[TO_NW] = valid[TO_W] = valid[TO_SW] = 0;

  if (row < SCREEN_ROWS - 1 && col > 1
    && board[row + 1][col - 2] == ROBOTS_ROBOT)
    valid[TO_W] = valid[TO_SW] = 0;

  if (row < SCREEN_ROWS - 2 && col > 1
    && board[row + 2][col - 2] == ROBOTS_ROBOT)
    valid[TO_SW] = 0;

  if (row < SCREEN_ROWS - 2 && col > 0
    && board[row + 2][col - 1] == ROBOTS_ROBOT)
    valid[TO_SW] = valid[TO_S] = 0;

  if (row < SCREEN_ROWS - 2
    && board[row + 2][col] == ROBOTS_ROBOT)
    valid[TO_SW] = valid[TO_S] = valid[TO_SE] = 0;

  if (row < SCREEN_ROWS - 2 && col < SCREEN_COLS - 1
    && board[row + 2][col + 1] == ROBOTS_ROBOT)
    valid[TO_S] = valid[TO_SE] = 0;

  if (row < SCREEN_ROWS - 2 && col < SCREEN_COLS - 2
    && board[row + 2][col + 2] == ROBOTS_ROBOT)
    valid[TO_SE] = 0;

  if (row < SCREEN_ROWS - 1 && col < SCREEN_COLS - 2
    && board[row + 1][col + 2] == ROBOTS_ROBOT)
    valid[TO_SE] = valid[TO_E] = 0;

  if (col < SCREEN_COLS - 2
    && board[row][col + 2] == ROBOTS_ROBOT)
    valid[TO_SE] = valid[TO_E] = valid[TO_NE] = 0;

  if (row > 0 && col < SCREEN_COLS - 2
    && board[row - 1][col + 2] == ROBOTS_ROBOT)
    valid[TO_E] = valid[TO_NE] = 0;

  if (row > 1 && col < SCREEN_COLS - 2
    && board[row - 2][col + 2] == ROBOTS_ROBOT)
    valid[TO_NE] = 0;

  if (row > 1 && col < SCREEN_COLS - 1
    && board[row - 2][col + 1] == ROBOTS_ROBOT)
    valid[TO_N] = valid[TO_NE] = 0;

  return valid;
}

// <<<
//******************************************************************************
// >>> abovebelow

static int abovebelow(const char* valid, int selfrow, int minrow, int maxrow,
  int selfcol, int avgcol0, int avgcol1)
{
  if (selfrow < minrow) // above
  {
    if (selfcol < avgcol0)
    {
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
      if (valid[TO_E])
	return ROBOTS_KEY_W;
      if (valid[TO_N])
	return ROBOTS_KEY_N;
    }
    else if (selfcol > avgcol1)
    {
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_N])
	return ROBOTS_KEY_N;
    }
    else // avgcol0 <= selfcol <= avgcol1
    {
      if (valid[TO_N])
	return ROBOTS_KEY_N;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
    }
  }
  else if (selfrow > maxrow) // below
  {
    if (selfcol < avgcol0)
    {
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
      if (valid[TO_E])
	return ROBOTS_KEY_W;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
    }
    else if (selfcol > avgcol1)
    {
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
    }
    else // avgcol0 <= selfcol <= avgcol1
    {
      if (valid[TO_S])
	return ROBOTS_KEY_S;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
    }
  }
  return NIL;
}

// <<<
//******************************************************************************
// >>> leftrightof

static int leftrightof(const char* valid, int selfcol, int mincol, int maxcol,
  int selfrow, int avgrow0, int avgrow1)
{
  if (selfcol < mincol) // leftofit
  {
    if (selfrow < avgrow0)
    {
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_N])
	return ROBOTS_KEY_N;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
    }
    else // avgrow0 <= selfrow <= avgrow1
    {
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
    }
  }
  else if (selfcol > maxcol) // rightofit
  {
    if (selfrow < avgrow0)
    {
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_N])
	return ROBOTS_KEY_N;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
    }
    else // avgrow0 <= selfrow <= avgrow1
    {
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
    }
  }
  return NIL;
}

// <<<
//******************************************************************************
// >>> junkrow

static int junkrow(const char board[SCREEN_ROWS][SCREEN_COLS], const char *valid,
  int selfrow, int selfcol, int robrow, int robcol)
{
  int num = SCREEN_COLS; 
  char key = NIL;

  for (int row = 0; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
      if (board[row][col] == ROBOTS_JUNK)
      {
	if (selfcol < robcol - 1 && col < robcol)
	{
	  int robnum = robcol - (col + 1);
	  if (abs(robrow - row) <= robnum)
	  {
	    if (selfcol < col)
	    {
	      if (selfrow < row)
	      {
		int selfnum = row - selfrow;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfcol < col - 1 && valid[TO_SE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SW;
		  }
		  else if (valid[TO_S])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_S;
		  }
		}
	      }
	      else if (selfrow > row)
	      {
		int selfnum = selfrow - row;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfcol < col - 1 && valid[TO_NE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NW;
		  }
		  else if (valid[TO_N])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_N;
		  }
		}
	      }
	      else // selfrow == row
		return ROBOTS_KEY_WAIT;
	    }
	    else // selfcol >= col
	    {
	      int selfnumcol = selfcol - (col - 1);
	      if (selfrow < row)
	      {
		int selfnumrow = row - selfrow;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_W])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_E;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_SW])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_SE;
		  }
		}
	      }
	      else if (selfrow > row)
	      {
		int selfnumrow = selfrow - row;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_W])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_E;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_NW])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_NE;
		  }
		}
	      }
	      else // selfrow == row
	      {
		int selfnum = selfcol - (col - 1);
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[TO_NW])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NE;
		  }
		  else if (valid[TO_SW])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SE;
		  }
		}
	      } 
	    }
	  }
	}
	else if (selfcol > robcol + 1 && col > robcol)
	{
	  int robnum = (col - 1) - robcol;
	  if (abs(robrow - row) <= robnum)
	  {
	    if (selfcol > col)
	    {
	      if (selfrow < row)
	      {
		int selfnum = row - selfrow;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfcol > col + 1 && valid[TO_SW])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SE;
		  }
		  else if (valid[TO_S])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_S;
		  }
		}
	      }
	      else if (selfrow > row)
	      {
		int selfnum = selfrow - row;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfcol > col + 1 && valid[TO_NW])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NE;
		  }
		  else if (valid[TO_N])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_N;
		  }
		}
	      }
	      else // selfrow == row
		return ROBOTS_KEY_WAIT;
	    }
	    else // selfcol <= col
	    {
	      int selfnumcol = (col + 1) - selfcol;
	      if (selfrow < row)
	      {
		int selfnumrow = row - selfrow;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_E])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_W;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_SE])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_SW;
		  }
		}
	      }
	      else if (selfrow > row)
	      {
		int selfnumrow = selfrow - row;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_E])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_W;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_NE])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_NW;
		  }
		}
	      }
	      else // selfrow == row
	      {
		int selfnum = (col + 1) - selfcol;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[TO_NE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NW;
		  }
		  else if (valid[TO_SE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SW;
		  }
		}
	      }
	    }
	  }
	}
      }

  if (num < SCREEN_COLS)
    return key;

  return NIL;
}

// <<<
//******************************************************************************
// >>> junkcol

static int junkcol(const char board[SCREEN_ROWS][SCREEN_COLS], const char *valid,
  int selfrow, int selfcol, int robrow, int robcol)
{
  int num = SCREEN_COLS; 
  char key = NIL;

  for (int row = 0; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
      if (board[row][col] == ROBOTS_JUNK)
      {
	if (selfrow < robrow - 1 && row < robrow)
	{
	  int robnum = robrow - (row + 1);
	  if (abs(robcol - col) <= robnum)
	  {
	    if (selfrow < row)
	    {
	      if (selfcol < col)
	      {
		int selfnum = col - selfcol;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfrow < row - 1 && valid[TO_SE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SW;
		  }
		  else if (valid[TO_E])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_W;
		  }
		}
	      }
	      else if (selfcol > col)
	      {
		int selfnum = selfcol - col;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfrow < row - 1 && valid[TO_W])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SE;
		  }
		  else if (valid[TO_W])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_E;
		  }
		}
	      }
	      else // selfcol == col
		return ROBOTS_KEY_WAIT;
	    }
	    else // selfrow >= row
	    {
	      int selfnumrow = selfrow - (row - 1);
	      if (selfcol < col)
	      {
		int selfnumcol = col - selfcol;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_N])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_N;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_NE])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_NW;
		  }
		}
	      }
	      else if (selfcol > col)
	      {
		int selfnumcol = selfcol - col;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_N])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_N;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_NW])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_NE;
		  }
		}
	      }
	      else // selfcol == col
	      {
		int selfnum = selfrow - (row - 1);
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[TO_NW])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NE;
		  }
		  else if (valid[TO_NE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NW;
		  }
		}
	      } 
	    }
	  }
	}
	else if (selfrow > robrow + 1 && row > robrow)
	{
	  int robnum = (row - 1) - robrow;
	  if (abs(robcol - col) <= robnum)
	  {
	    if (selfrow > row)
	    {
	      if (selfcol < col)
	      {
		int selfnum = col - selfcol;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfrow > row + 1 && valid[TO_NE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NW;
		  }
		  else if (valid[TO_E])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_W;
		  }
		}
	      }
	      else if (selfcol > col)
	      {
		int selfnum = selfcol - col;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (selfrow > row + 1 && valid[TO_NW])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_NE;
		  }
		  else if (valid[TO_W])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_E;
		  }
		}
	      }
	      else // selfcol == col
		return ROBOTS_KEY_WAIT;
	    }
	    else // selfrow <= row
	    {
	      int selfnumrow = (row + 1) - selfrow;
	      if (selfcol < col)
	      {
		int selfnumcol = col - selfcol;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_S])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_S;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_SE])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_SW;
		  }
		}
	      }
	      else if (selfcol > col)
	      {
		int selfnumcol = selfcol - col;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[TO_S])
		  {
		    num = selfnumrow;
		    key = ROBOTS_KEY_S;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[TO_SW])
		  {
		    num = selfnumcol;
		    key = ROBOTS_KEY_SE;
		  }
		}
	      }
	      else // selfcol == col
	      {
		int selfnum = (row + 1) - selfrow;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[TO_NE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SW;
		  }
		  else if (valid[TO_SE])
		  {
		    num = selfnum;
		    key = ROBOTS_KEY_SE;
		  }
		}
	      }
	    }
	  }
	}
      }

  if (num < SCREEN_COLS)
    return key;

  return NIL;
}

// <<<
//******************************************************************************
// >>> junkesc

static int junkesc(const char board[SCREEN_ROWS][SCREEN_COLS], const char *valid,
  int selfrow, int selfcol, int robrow, int robcol)
{
  (void)robrow;

  int junkrow = 0;
  int junknum = 0;
  for (int row = 0; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
      if (board[row][col] == ROBOTS_JUNK)
      {
	junkrow += row;
	junknum++;
      }
  junkrow /= junknum;

  char key = NIL;
  if (selfcol <= robcol)
  {
    if (selfrow < junkrow)
    {
      if (valid[TO_SW])
	key = ROBOTS_KEY_SE;
      else if (valid[TO_W])
	key = ROBOTS_KEY_E;
    }
    else if (selfrow > junkrow)
    {
      if (valid[TO_NW])
	key = ROBOTS_KEY_NE;
      else if (valid[TO_W])
	key = ROBOTS_KEY_E;
    }
    else // selfrow == junkrow
    {
      if (valid[TO_W])
	key = ROBOTS_KEY_E;
      else if (valid[TO_NW])
	key = ROBOTS_KEY_NE;
      else if (valid[TO_SW])
	key = ROBOTS_KEY_SE;
    }
  }
  else if (selfcol > robcol)
  {
    if (selfrow < junkrow)
    {
      if (valid[TO_SE])
	key = ROBOTS_KEY_SW;
      else if (valid[TO_E])
	key = ROBOTS_KEY_W;
    }
    else if (selfrow > junkrow)
    {
      if (valid[TO_NE])
	key = ROBOTS_KEY_NW;
      else if (valid[TO_E])
	key = ROBOTS_KEY_W;
    }
    else // selfrow == junkrow
    {
      if (valid[TO_E])
	key = ROBOTS_KEY_W;
      else if (valid[TO_NE])
	key = ROBOTS_KEY_NW;
      else if (valid[TO_SE])
	key = ROBOTS_KEY_SW;
    }
  } 
  return key;
}

// <<<
//******************************************************************************
// >>> inbetween

static char inbetween(const char *valid, int selfrow, int avgrow0, int avgrow1,
  int selfcol, int avgcol0, int avgcol1, int minrow, int maxrow, int mincol, int maxcol)
{
  if (selfcol < avgcol0)
    if (selfrow < avgrow0)
    {
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
      if (valid[TO_E])
	return ROBOTS_KEY_W;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
      if (valid[TO_E])
	return ROBOTS_KEY_W;
      if (valid[TO_N])
	return ROBOTS_KEY_N;
    }
    else // avgrow0 <= selfrow <= avgrow1
    {
      if (valid[TO_E])
	return ROBOTS_KEY_W;
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
    }
  else if (selfcol > avgcol1)
    if (selfrow < avgrow0)
    {
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_N])
	return ROBOTS_KEY_N;
    }
    else // avgrow0 <= selfrow <= avgrow1
    {
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
    }
  else // avgcol0 <= selfcol <= avgcol1
    if (selfrow < avgrow0)
    {
      if (valid[TO_S])
	return ROBOTS_KEY_S;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[TO_N])
	return ROBOTS_KEY_N;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
    }

  if (selfcol <= avgcol1)
  {
    if (valid[TO_E])
      return ROBOTS_KEY_W;
    if (valid[TO_NE])
      return ROBOTS_KEY_NW;
    if (valid[TO_SE])
      return ROBOTS_KEY_SW;
  }
  else if (selfcol >= avgcol0)
  {
    if (valid[TO_W])
      return ROBOTS_KEY_E;
    if (valid[TO_NW])
      return ROBOTS_KEY_NE;
    if (valid[TO_SW])
      return ROBOTS_KEY_SE;
  }

  if (selfrow <= avgrow1)
  {
    if (valid[TO_S])
      return ROBOTS_KEY_S;
    if (valid[TO_SW])
      return ROBOTS_KEY_SE;
    if (valid[TO_SE])
      return ROBOTS_KEY_SW;
  }
  else if (selfrow >= avgrow0)
  {
    if (valid[TO_N])
      return ROBOTS_KEY_N;
    if (valid[TO_NW])
      return ROBOTS_KEY_NE;
    if (valid[TO_NE])
      return ROBOTS_KEY_NW;
  }

  if (selfcol <= maxcol)
  {
    if (valid[TO_E])
      return ROBOTS_KEY_W;
    if (valid[TO_NE])
      return ROBOTS_KEY_NW;
    if (valid[TO_SE])
      return ROBOTS_KEY_SW;
  }
  else if (selfcol >= mincol)
  {
    if (valid[TO_W])
      return ROBOTS_KEY_E;
    if (valid[TO_NW])
      return ROBOTS_KEY_NE;
    if (valid[TO_SW])
      return ROBOTS_KEY_SE;
  }

  if (selfrow <= maxrow)
  {
    if (valid[TO_S])
      return ROBOTS_KEY_S;
    if (valid[TO_SW])
      return ROBOTS_KEY_SE;
    if (valid[TO_SE])
      return ROBOTS_KEY_SW;
  }
  else if (selfrow >= minrow)
  {
    if (valid[TO_N])
      return ROBOTS_KEY_N;
    if (valid[TO_NW])
      return ROBOTS_KEY_NE;
    if (valid[TO_NE])
      return ROBOTS_KEY_NW;
  }

  return NIL;
}

// <<<
//******************************************************************************
// >>> strategy

int robots_strategy_auto(const char board[SCREEN_ROWS][SCREEN_COLS])
{
  int selfrow = 0, selfcol = 0;
  int minrow = SCREEN_ROWS, mincol = SCREEN_COLS;
  int maxrow = 0, maxcol = 0;

  for (int row = 0; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
    {
      if (board[row][col] == ROBOTS_PLAYER)
      {
	selfrow = row;
	selfcol = col;
      }
      else if (board[row][col] == ROBOTS_ROBOT)
      {
	if (row > maxrow)
	  maxrow = row;
	if (row < minrow)
	  minrow =  row;
	if (col > maxcol)
	  maxcol = col;
	if (col < mincol)
	  mincol =  col;
      }
    }

  int avgrow0 = (maxrow + minrow) / 2;
  int avgrow1 = (maxrow + minrow + 1) / 2;
  int avgcol0 = (maxcol + mincol) / 2;
  int avgcol1 = (maxcol + mincol + 1) / 2;

  const char* valid = validity(board, selfrow, selfcol);

  if (minrow == maxrow && mincol == maxcol) // one robot
  {
    char key = junkrow(board, valid, selfrow, selfcol, minrow, mincol);
    if (key)
      return key;

    key = junkcol(board, valid, selfrow, selfcol, minrow, mincol);
    if (key)
      return key;

    key = junkesc(board, valid, selfrow, selfcol, minrow, mincol);
    if (key)
      return key;
  }
  else if (minrow == maxrow && mincol < maxcol) // one row of robots
  {
    if (selfcol < mincol)
    {
      char key = junkrow(board, valid, selfrow, selfcol, minrow, mincol);
      if (key)
	return key;
    }
    else if (selfcol > maxcol)
    {
      char key = junkrow(board, valid, selfrow, selfcol, maxrow, maxcol);
      if (key)
	return key;
    }

    char key = abovebelow(valid, selfrow, minrow, maxrow, selfcol, avgcol0, avgcol1);
    if (key)
      return key;

    if (selfcol > mincol && selfcol < maxcol)
    {
      if (valid[TO_W])
	return ROBOTS_KEY_E;
      if (valid[TO_E])
	return ROBOTS_KEY_W;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
    }
  }
  else if (minrow < maxrow && mincol == maxcol) // one column of robots
  {
    if (selfrow < minrow)
    {
      char key = junkcol(board, valid, selfrow, selfcol, minrow, mincol);
      if (key)
	return key;
    }
    else if (selfrow > maxcol)
    {
      char key = junkcol(board, valid, selfrow, selfcol, maxrow, maxcol);
      if (key)
	return key;
    }

    char key = leftrightof(valid, selfcol, mincol, maxcol, selfrow, avgrow0, avgrow1);
    if (key)
      return key;

    if (selfrow > minrow && selfrow < maxrow)
    {
      if (valid[TO_N])
	return ROBOTS_KEY_N;
      if (valid[TO_S])
	return ROBOTS_KEY_S;
      if (valid[TO_NW])
	return ROBOTS_KEY_NE;
      if (valid[TO_NE])
	return ROBOTS_KEY_NW;
      if (valid[TO_SW])
	return ROBOTS_KEY_SE;
      if (valid[TO_SE])
	return ROBOTS_KEY_SW;
    }
  }
  else // spread out
  {
    char key = leftrightof(valid, selfcol, mincol, maxcol, selfrow, avgrow0, avgrow1);
    if (key)
      return key;

    key = abovebelow(valid, selfrow, minrow, maxrow, selfcol, avgcol0, avgcol1);
    if (key)
      return key;

    key = inbetween(valid, selfrow, avgrow0, avgrow1, selfcol, avgcol0, avgcol1,
      minrow, maxrow, mincol, maxcol);
    if (key)
      return key;
  }
   
  if (valid[TO_X])
    return ROBOTS_KEY_X;

  return ROBOTS_KEY_TELE;
}

// <<<
//******************************************************************************
