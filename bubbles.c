//******************************************************************************

#include "display.h"

#include <stdio.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define WIDTH 448
#define HEIGHT 236
#define BORDER 30
#define MAX_DIST 574
#define EMPTY 25
#define QUEUE_LEN 100000
#define FRAMES_PER_BUBBLE 20
#define FRAMES_PER_RING 10
#define MAX_SAME 3
#define DELAY 25
#define MIN_DELAY 20

//******************************************************************************

typedef struct
{
  int16_t x;
  int16_t y;
  int next[3];
} pixel_t;

static pixel_t pixels[(WIDTH + BORDER) * (HEIGHT + BORDER)];
static int dist_first[MAX_DIST + 1];

static void init()
{
  memset(pixels, 0, WIDTH * HEIGHT * sizeof(pixel_t));
  memset(dist_first, 0, (MAX_DIST + 1) * sizeof(int));

  int n = 0;
  for (int d = 0; d < MAX_DIST; d++)
  {
    dist_first[d] = n;
    for (int x = 0; x < WIDTH + BORDER; x++)
      for (int y = 0; y < HEIGHT + BORDER; y++)
      {
	int dist = (int)(sqrtf((float)x * (float)x + (float)y * (float)y) + 0.5);
	if (dist == d)
	{
	  pixels[n].x = x;
	  pixels[n].y = y;
	  if (d)
	  {
	    int nearest = -1;
	    float best_dist = (float)(MAX_DIST + 1);
	    for (int i = dist_first[d - 1]; i < dist_first[d]; i++)
	    {
	      float dx = (float)(pixels[i].x - x);
	      float dy = (float)(pixels[i].y - y);
	      float di = sqrtf(dx * dx + dy * dy);
	      if (di < best_dist)
	      {
		best_dist = di;
		nearest = i;
	      }
	      assert(nearest >= 0);
	    }

	    if (pixels[nearest].next[0] == 0)
	      pixels[nearest].next[0] = n;
	    else if (pixels[nearest].next[1] == 0)
	      pixels[nearest].next[1] = n;
	    else if (pixels[nearest].next[2] == 0)
	      pixels[nearest].next[2] = n;
	    else
	      assert(0);
	  }
	  n++;
	}
      }
  }
  assert(n == (WIDTH + BORDER) * (HEIGHT + BORDER));
  dist_first[MAX_DIST] = n;
}

//*****************************************************************************

static bool screen[WIDTH + 2 * BORDER][HEIGHT + 2 * BORDER];

static void set(int x, int y, bool value)
{
  assert(x >= 0);
  assert(x < WIDTH + 2 * BORDER);
  assert(y >= 0);
  assert(y < HEIGHT + 2 * BORDER);
  screen[x][y] = value;

  x -= BORDER;
  y -= BORDER;
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    display_set_virt(x, y, value);
}

//*****************************************************************************

typedef struct
{
  int16_t x0; 
  int16_t y0; 
  int16_t xs; 
  int16_t ys; 
  int pixels_i;
  bool value;
} job_t;

static job_t queue[QUEUE_LEN];
static int head = 0;
static int tail = 0;

static void bubble_create(int x, int y, int8_t value)
{
  set(x, y, value);
  queue[head].x0 = x;
  queue[head].y0 = y;
  queue[head].xs = 1;
  queue[head].ys = 1;
  queue[head].pixels_i = 0;
  queue[head].value = value;
  head++;
  head %= QUEUE_LEN;
  assert(head != tail);

  set(x - 1, y, value);
  queue[head].x0 = x - 1;
  queue[head].y0 = y;
  queue[head].xs = -1;
  queue[head].ys = 1;
  queue[head].pixels_i = 0;
  queue[head].value = value;
  head++;
  head %= QUEUE_LEN;
  assert(head != tail);

  set(x, y - 1, value);
  queue[head].x0 = x;
  queue[head].y0 = y - 1;
  queue[head].xs = 1;
  queue[head].ys = -1;
  queue[head].pixels_i = 0;
  queue[head].value = value;
  head++;
  head %= QUEUE_LEN;
  assert(head != tail);

  set(x - 1, y - 1, value);
  queue[head].x0 = x - 1;
  queue[head].y0 = y - 1;
  queue[head].xs = -1;
  queue[head].ys = -1;
  queue[head].pixels_i = 0;
  queue[head].value = value;
  head++;
  head %= QUEUE_LEN;
  assert(head != tail);

  display_flush();
}

static void queue_run()
{
  int end = head;
  while (tail != end)
  {
    int16_t x0 = queue[tail].x0;
    int16_t y0 = queue[tail].y0;
    int16_t xs = queue[tail].xs;
    int16_t ys = queue[tail].ys;
    int i = queue[tail].pixels_i;
    bool value = queue[tail].value;
    for (int k = 0; k < 3; k++)
    {
      int j = pixels[i].next[k];
      if (!j)
	break;
      int16_t x = x0 + xs * pixels[j].x;
      int16_t y = y0 + ys * pixels[j].y;
      if (x >= 0 && x < WIDTH + 2 * BORDER && y >= 0 && y < HEIGHT + 2 * BORDER
	&& screen[x][y] != value)
      {
	set(x, y, value);
	queue[head].x0 = x0;
	queue[head].y0 = y0;
	queue[head].xs = xs;
	queue[head].ys = ys;
	queue[head].pixels_i = j;
	queue[head].value = value;
	head++;
	head %= QUEUE_LEN;
	assert(head != tail);
      }
    }
    tail++;
    tail %= QUEUE_LEN;
  }
}

//*****************************************************************************

static bool same(int x, int y)
{
  bool reference = screen[x][y];

  return ((x - EMPTY < 0 || screen[x - EMPTY][y] == reference)
    && (x + EMPTY >= WIDTH + 2 * BORDER || screen[x + EMPTY][y] == reference)
    && (y - EMPTY < 0 || screen[x][y - EMPTY] == reference)
    && (y + EMPTY >= HEIGHT + 2 * BORDER || screen[x][y + EMPTY]== reference)
    && (x - EMPTY < 0 || y - EMPTY < 0 || screen[x - EMPTY][y - EMPTY] == reference)
    && (x - EMPTY < 0 || y + EMPTY >= HEIGHT + 2 * BORDER || screen[x - EMPTY][y + EMPTY] == reference)
    && (x + EMPTY >= WIDTH + 2 * BORDER || y - EMPTY < 0 || screen[x + EMPTY][y - EMPTY] == reference)
    && (x + EMPTY >= WIDTH + 2 * BORDER || y + EMPTY >= HEIGHT + 2 * BORDER || screen[x + EMPTY][y + EMPTY] == reference));
}

static void filter()
{
  for (int x = 1; x < WIDTH + 2 * BORDER - 1; x++)
    for (int y = 1; y < HEIGHT + 2 * BORDER - 1; y++)
    {
      bool center = screen[x][y];
      int count = 0;
      if (screen[x - 1][y] != center)
	count++;
      if (screen[x + 1][y] != center)
	count++;
      if (screen[x][y - 1] != center)
	count++;
      if (screen[x][y + 1] != center)
	count++;
      if (count >= 3)
	set(x, y, !center);
    }
}

//******************************************************************************

static void animation_bubbles(int delay)
{
  for (int n = 0; display_button() == 0; n++)
  {
    if (!(n % FRAMES_PER_BUBBLE))
    {
      int x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
      int y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
      if (same(x, y))
	bubble_create(x, y, !screen[x][y]);
      else
      {
	int x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
	int y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
	if (same(x, y))
	  bubble_create(x, y, !screen[x][y]);
	else
	{
	  int x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
	  int y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
	  if (same(x, y))
	    bubble_create(x, y, !screen[x][y]);
	  else
	  {
	    int x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
	    int y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
	    if (same(x, y))
	      bubble_create(x, y, !screen[x][y]);
	  }
	}
      }
    }
    queue_run();
    filter();
    display_flush();
    usleep(delay * 1000);
  }
}

static void animation_rings(int delay)
{
  int x = -1;
  int y = -1;
  for (int n = 0; display_button() == 0; n++)
  {
    if (n % FRAMES_PER_BUBBLE == 0)
    {
      x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
      y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
      if (!screen[x][y] && same(x, y))
	bubble_create(x, y, true);
      else
      {
	x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
	y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
	if (!screen[x][y] && same(x, y))
	  bubble_create(x, y, true);
	else
	{
	  x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
	  y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
	  if (!screen[x][y] && same(x, y))
	    bubble_create(x, y, true);
	  else
	  {
	    x = rand() % (WIDTH + 2 * BORDER - 1) + 1;
	    y = rand() % (HEIGHT + 2 * BORDER - 1) + 1;
	    if (!screen[x][y] && same(x, y))
	      bubble_create(x, y, true);
	    else
	    {
	      x = -1;
	      y = -1;
	    }
	  }
	}
      }
    }
    else if (n % FRAMES_PER_BUBBLE == FRAMES_PER_RING)
    {
      if (x >= 0 && y >= 0 && screen[x][y])
	bubble_create(x, y, false);
    }
    queue_run();
    filter();
    display_flush();
    usleep(delay * 1000);
  }
}

int main(int ac, char *av[])
{
  bool rings = false;
  int display_select = 0;
  int delay = DELAY;
  for (int ai = 1; ai < ac; ai++)
  {
    if (av[ai][1] == 's')
      display_select |= DISPLAY_SELECT_SP;
    else if (av[ai][1] == 'x')
      display_select |= DISPLAY_SELECT_GX;
    else if (av[ai][1] == 'r')
      rings = true;
    else if (isdigit(av[ai][1]))
    {
      delay = atoi(&av[ai][1]);
      assert(delay >= MIN_DELAY);
    }
    else
      assert(0);
  }

  srand(time(NULL));
  init();
  memset(screen, 0, (WIDTH + 2 * BORDER) * (HEIGHT + 2 * BORDER) * sizeof(int8_t));
  display_create(display_select);

  if (rings)
    animation_rings(delay);
  else
    animation_bubbles(delay);

  display_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
