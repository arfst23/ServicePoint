//******************************************************************************

#include "display.h"
#include "display_graphix.h"
#include "graphix.h"
#include "servicepoint.h"

#include <stdbool.h>
#include <assert.h>

#if DISPLAY_TILE_SIZE != SP_TILE_SIZE
#error DISPLAY_TILE_SIZE != SP_TILE_SIZE
#endif

#if DISPLAY_WIDTH != SP_PIXEL_WIDTH
#error DISPLAY_WIDTH != SP_PIXEL_WIDTH
#endif

#if DISPLAY_HEIGHT != SP_PIXEL_HEIGHT
#error DISPLAY_HEIGHT != SP_PIXEL_HEIGHT
#endif

static int display_select = 0;

static graphix gx = NULL;

static sp_connection connection = NULL;
static sp_bitmap bitmap = NULL;

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
    assert(!connection);
    connection = sp_connection_open(ADDRESS);
    assert(connection);
    assert(!bitmap);
    bitmap = sp_bitmap_new(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    sp_bitmap_fill(bitmap, false);
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
    assert(connection);
    assert(bitmap);
    sp_connection_free(connection);
    sp_bitmap_free(bitmap);
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
    assert(connection);
    assert(bitmap);
    sp_bitmap_set(bitmap, (size_t)x, (size_t)y, value);
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
    assert(connection);
    assert(bitmap);
    sp_bitmap_fill(bitmap, false);
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
    assert(connection);
    assert(bitmap);
    sp_bitmap bitmap_copy = sp_bitmap_clone(bitmap);
    sp_command command = sp_command_bitmap_linear_win(0, 0, bitmap_copy,
      SP_COMPRESSION_CODE_LZMA);
    assert(command);
    bool sent = sp_connection_send_command(connection, command);
    assert(sent);
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
