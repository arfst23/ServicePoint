//******************************************************************************

#include "term.h"

#include <stdio.h>
#include <termios.h>
#include <assert.h>

static struct termios tio_backup;

//******************************************************************************

void term_clear(void)
{
  printf("\e[2J");
}

void term_home(void)
{
  printf("\e[0;0H");
}

void term_color(int color)
{
  switch (color)
  {
  case TERM_WHITE:
    printf("\e[00m");
    break;
  case TERM_RED:
    printf("\e[01;31m");
    break;
  case TERM_YELLOW:
    printf("\e[01;33m");
    break;
  case TERM_GREEN:
    printf("\e[01;32m");
    break;
  case TERM_CYAN:
    printf("\e[01;36m");
    break;
  case TERM_MAGENTA:
    printf("\e[01;35m");
    break;
  default:
    assert(0);
  }
}

//******************************************************************************

void term_raw(void)
{
  struct termios tio;
  int rc = tcgetattr(0, &tio);
  assert(rc != 1);
  tio_backup = tio;

  tio.c_iflag &= ~(INLCR | ICRNL | ISTRIP | IXON | BRKINT);
  tio.c_lflag &= ~(ICANON | ECHO);
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 0;

  rc = tcsetattr(0, TCSANOW, &tio);
  assert(rc != 1);
}

void term_restore(void)
{
  int rc = tcsetattr(0, TCSANOW, &tio_backup);
  assert(rc != 1);
}

//******************************************************************************
