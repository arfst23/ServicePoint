//******************************************************************************

#include "display.h"
#include "display_graphix.h"
#include "graphix.h"
#include "sp.h"

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#if DISPLAY_TILE_SIZE != SP_TILE_SIZE
#error DISPLAY_TILE_SIZE != SP_TILE_SIZE
#endif

#if DISPLAY_WIDTH != SP_WIDTH
#error DISPLAY_WIDTH != SP_WIDTH
#endif

#if DISPLAY_HEIGHT != SP_HEIGHT
#error DISPLAY_HEIGHT != SP_HEIGHT
#endif

static int display_select = 0;

static graphix gx = NULL;

//******************************************************************************

void display_create(int select)
{
  display_select = select;

  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(!gx);
    gx = display_graphix_create();
  }
  if (display_select & DISPLAY_SELECT_SP)
  {
    sp_create();
  }
}

void display_free(void)
{
  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(gx);
    graphix_free(gx);
  }
  if (display_select & DISPLAY_SELECT_SP)
  {
    sp_free();
  }
}

//******************************************************************************

void display_set(int x, int y, bool value)
{
  assert(x >= 0);
  assert(x < DISPLAY_WIDTH);
  assert(y >= 0);
  assert(y < DISPLAY_HEIGHT);

  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(gx);
    display_graphix_set(gx, x, y, value);
  }
  if (display_select & DISPLAY_SELECT_SP)
  {
    sp_set(y, x, value);
  }
}

void display_set_virt(int x, int yv, bool value)
{
  assert(x >= 0);
  assert(x < DISPLAY_WIDTH);
  assert(yv >= 0);
  assert(yv < DISPLAY_HEIGHT_VIRT);

  int seperators = yv / (DISPLAY_TILE_SIZE + DISPLAY_SEPARATOR_WIDTH);
  int y = yv - seperators * DISPLAY_SEPARATOR_WIDTH;
  int tiles = seperators + 1;
  if (y >= tiles * DISPLAY_TILE_SIZE)
    return;
  display_set(x, y, value);
}

//******************************************************************************

void display_clear(void)
{
  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(gx);
    display_graphix_clear(gx);
  }
  if (display_select & DISPLAY_SELECT_SP)
  {
    sp_clear();
  }
}

void display_flush(void)
{
  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(gx);
    graphix_flush(gx);
  }
  if (display_select & DISPLAY_SELECT_SP)
  {
    sp_send();
  }
}

//******************************************************************************

int display_button(void)
{
  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(gx);
    return graphix_button(gx);
  }
  return 0;
}

int display_wait(void)
{
  if (display_select & DISPLAY_SELECT_GX)
  {
    assert(gx);
    return graphix_wait(gx);
  }
  return 1;
}

//******************************************************************************
