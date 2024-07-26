//******************************************************************************

#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define WIDTH DISPLAY_WIDTH // 448
#define HEIGHT DISPLAY_HEIGHT_VIRT // 236
#define MAX_RADIUS 506
#define CENTER_RADIUS 270

#define PERIODS 14
#define COLOR_LENGTH 900
#define COLOR_PERIOD (2 * COLOR_LENGTH)
#define TUNNEL_RADIUS 1000
#define FOCAL_LENGTH 500

#define FRAMES 10
#define DELAY 60

//******************************************************************************
// >>> ring(x, y, r, s, val)

typedef struct
{
  int16_t x;
  int16_t y;
} coord_t;

static coord_t coord[WIDTH * HEIGHT];
static int rtoi[MAX_RADIUS + 1];

static void init()
{
  int n = 0;
  for (int r = 0; r < MAX_RADIUS; r++)
  {
    rtoi[r] = n;
    for (int x = 0; x < WIDTH; x++)
      for (int y = 0; y < HEIGHT; y++)
      {
	int s = (int)(sqrtf((float)x * (float)x + (float)y * (float)y) + 0.5);
	if (r == s)
	{
	  coord[n].x = x;
	  coord[n].y = y;
	  n++;
	}
      }
  }
  assert(n == (WIDTH * HEIGHT));
  rtoi[MAX_RADIUS] = n;
}

static void ring(int x, int y, int r, int s, bool value)
{
  assert(r <= s);
  assert(s <= MAX_RADIUS);

  for (int i = rtoi[r], j = rtoi[s]; i < j; i++)
  {
    int xx = coord[i].x;
    int yy = coord[i].y;

    if (x + xx < WIDTH && y + yy < HEIGHT)
      display_set_virt(x + xx, y + yy, value);
    if (x - xx >= 0 && y + yy < HEIGHT)
      display_set_virt(x - xx, y + yy, value);
    if (x + xx < WIDTH && y - yy >= 0)
      display_set_virt(x + xx, y - yy, value);
    if (x - xx >= 0 && y - yy >= 0)
      display_set_virt(x - xx, y - yy, value);
  }
}

// <<<
//******************************************************************************

int main(int ac, char *av[])
{
  int display_select = 0;
  for (int ai = 1; ai < ac; ai++)
  {
    if (av[ai][1] == 's')
      display_select |= DISPLAY_SELECT_SP;
    else if (av[ai][1] == 'x')
      display_select |= DISPLAY_SELECT_GX;
    else
      assert(0);
  }

  init();
  int d0 = TUNNEL_RADIUS * FOCAL_LENGTH / CENTER_RADIUS - FOCAL_LENGTH;

  display_create(display_select);

  for (int frame = 0; !display_button(); frame = (frame + 1) % FRAMES)
  {
    //display_clear();

    int offset = frame * COLOR_PERIOD / FRAMES;

    for (int i = PERIODS - 1; i >= 0; i--)
    {
      int d1 = d0 + i * COLOR_PERIOD - offset;
      int d2 = d1 + COLOR_LENGTH;
      int d3 = d2 + COLOR_LENGTH;
      if (d1 < d0)
	d1 = d0;
      if (d2 < d0)
	d2 = d0;
      assert(d3 > d0);

      int r1 = (TUNNEL_RADIUS - d1 / 35) * FOCAL_LENGTH / (d1 + FOCAL_LENGTH);
      int r2 = (TUNNEL_RADIUS - d2 / 35) * FOCAL_LENGTH / (d2 + FOCAL_LENGTH);
      int r3 = (TUNNEL_RADIUS - d3 / 35) * FOCAL_LENGTH / (d3 + FOCAL_LENGTH);

      int curve_x = 0;
      int curve_y = 0;

      int xx1 = curve_x * WIDTH * (d1 - d0) / (PERIODS * COLOR_PERIOD)
        * (d1 - d0) / (PERIODS * COLOR_PERIOD) / 15;
      int xx2 = curve_x * WIDTH * (d2 - d0) / (PERIODS * COLOR_PERIOD)
        * (d2 - d0) / (PERIODS * COLOR_PERIOD) / 15;
      int x1 = xx1 * FOCAL_LENGTH / (d1 + FOCAL_LENGTH);
      int x2 = xx2 * FOCAL_LENGTH / (d2 + FOCAL_LENGTH);

      int yy1 = curve_y * WIDTH * (d1 - d0) / (PERIODS * COLOR_PERIOD)
        * (d1 - d0) / (PERIODS * COLOR_PERIOD) / 15;
      int yy2 = curve_y * WIDTH * (d2 - d0) / (PERIODS * COLOR_PERIOD)
        * (d2 - d0) / (PERIODS * COLOR_PERIOD) / 15;
      int y1 = yy1 * FOCAL_LENGTH / (d1 + FOCAL_LENGTH);
      int y2 = yy2 * FOCAL_LENGTH / (d2 + FOCAL_LENGTH);

      ring(WIDTH / 2 + x2, HEIGHT / 2 + y2, r3, r2, false);
      ring(WIDTH / 2 + x1, HEIGHT / 2 + y1, r2, r1, true);
    }

    display_flush();
    usleep(DELAY * 1000);
  }

  display_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
