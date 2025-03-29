//******************************************************************************

#include "graphix.h"
#include "sp.h"

#include <stdbool.h>
#include <assert.h>

//******************************************************************************

#define PIXEL_SIZE 2
#define PIXEL_SPACING 2
#define OFFSET (PIXEL_SIZE + PIXEL_SPACING)

#define BORDER_WIDTH (2 * OFFSET)
#define SEPARATORS 19
#define SEPARATOR_HEIGHT (4 * OFFSET)
#define SEPARATOR_PADDING (1 * OFFSET)

#define BORDER_PADDING (13 * OFFSET)
#define PARTS (SEPARATORS + 1)
#define PART_HEIGHT (SP_HEIGHT / PARTS)
#define WIDTH (2 * BORDER_WIDTH + SP_WIDTH * OFFSET + PIXEL_SPACING + 2 * BORDER_PADDING)
#define HEIGHT (2 * BORDER_WIDTH + SP_HEIGHT * OFFSET \
    + PARTS * PIXEL_SPACING + SEPARATORS * SEPARATOR_HEIGHT + 2 * SEPARATOR_PADDING)

#define COLOR_BLACK 0
#define COLOR_GREEN 1
#define COLOR_GRAY 2
#define COLORS 3

static const char *color[] = { "black", "green", "gray20" };

static const char *title = "Servicepoint";

//******************************************************************************

graphix display_graphix_create(void)
{
  graphix gx = graphix_create(title, WIDTH, HEIGHT, COLORS, color);
    graphix_rect(gx, 0, 0, WIDTH, BORDER_WIDTH, COLOR_GRAY);
  graphix_rect(gx, 0, HEIGHT - BORDER_WIDTH, WIDTH, BORDER_WIDTH, COLOR_GRAY);
  graphix_rect(gx, 0, 0, BORDER_WIDTH, HEIGHT, COLOR_GRAY);
  graphix_rect(gx, WIDTH - BORDER_WIDTH, 0, BORDER_WIDTH, HEIGHT, COLOR_GRAY);

  for (int i = 0; i < SEPARATORS; i++)
    graphix_rect(gx, 0, BORDER_WIDTH + PART_HEIGHT * OFFSET + PIXEL_SPACING + SEPARATOR_PADDING
        + i * (SEPARATOR_HEIGHT + PART_HEIGHT * OFFSET + PIXEL_SPACING) + SEPARATOR_PADDING,
	WIDTH, SEPARATOR_HEIGHT - 2 * SEPARATOR_PADDING, COLOR_GRAY);
  return gx;
}

//******************************************************************************

void display_graphix_set(graphix gx, int x, int y, bool value)
{
  assert(gx);
  assert(x >= 0);
  assert(x < graphix_width(gx));
  assert(y >= 0);
  assert(y < graphix_height(gx));
  int xx = BORDER_WIDTH + PIXEL_SPACING + x * OFFSET + BORDER_PADDING;
  int ss = y / PART_HEIGHT;
  int yy = BORDER_WIDTH + PIXEL_SPACING + ss * (SEPARATOR_HEIGHT + PIXEL_SPACING)
      + SEPARATOR_PADDING + y * OFFSET;
  graphix_rect(gx, xx, yy, 2, 2, value ? COLOR_GREEN : COLOR_BLACK);
}

//******************************************************************************

void display_graphix_clear(graphix gx)
{
  assert(gx);
  for (int i = 0; i < PARTS; i++)
    graphix_rect(gx, BORDER_WIDTH + PIXEL_SPACING + BORDER_PADDING,
	BORDER_WIDTH + PIXEL_SPACING + SEPARATOR_PADDING
	+ i * (SEPARATOR_HEIGHT + PIXEL_SPACING + 8 * OFFSET),
	SP_WIDTH * OFFSET - PIXEL_SPACING,
	PART_HEIGHT * OFFSET - PIXEL_SPACING, COLOR_BLACK);
}

//******************************************************************************
