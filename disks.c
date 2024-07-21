//******************************************************************************

#include "display.h"

#include <stdio.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

#define WIDTH (DISPLAY_WIDTH - DISPLAY_TILE_SIZE) // 440
#define HEIGHT (DISPLAY_HEIGHT_VIRT - DISPLAY_TILE_SIZE) // 228
#define ROTATION 12 // ~4 * pi
#define SIZE DISPLAY_TILE_SIZE // 8
#define ROW_DIST (DISPLAY_TILE_SIZE + DISPLAY_SEPARATOR_WIDTH)
#define Y_START (-ROW_DIST)

#define DELAY 35
#define MIN_DELAY 20

#define MIN_NEW 16
#define MAX_NEW 50
#define JOBS 1000

//******************************************************************************
// >>> disk

uint8_t disks[][ROTATION][SIZE] =
{
  {
    {
      0b00111100,
      0b01100110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01110110,
      0b11111011,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111011,
      0b11111101,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b11111101,
      0b11111101,
      0b11111111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b11111111,
      0b11111101,
      0b11111011,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111011,
      0b01110110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01100110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11011111,
      0b01101110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b11111111,
      0b10111111,
      0b11011111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11111111,
      0b10111111,
      0b10111111,
      0b11111111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01111110,
      0b11011111,
      0b10111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
    },
    {
      0b00111100,
      0b01101110,
      0b11011111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
    },
  },

  {
    {
      0b00111100,
      0b01000010,
      0b10010001,
      0b10000001,
      0b10000001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10001001,
      0b10000001,
      0b10000001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000101,
      0b10000001,
      0b10000001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000101,
      0b10000001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000001,
      0b10000101,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000001,
      0b10000001,
      0b10000101,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000001,
      0b10000001,
      0b10001001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000001,
      0b10000001,
      0b10010001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000001,
      0b10000001,
      0b10100001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10000001,
      0b10100001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10000001,
      0b10100001,
      0b10000001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
    {
      0b00111100,
      0b01000010,
      0b10100001,
      0b10000001,
      0b10000001,
      0b10000001,
      0b01000010,
      0b00111100,
    },
  },
};

void disk(int x, int y, int z, bool c)
{
  assert(x > -SIZE);
  assert(x < WIDTH + SIZE);
  assert(y >= -ROW_DIST);
  assert(y < HEIGHT + SIZE);
  assert(z >= 0);
  assert(z < ROTATION);

  for (int xx = 0; xx < SIZE; xx++)
    for (int yy = 0; yy < SIZE; yy++)
    {
      bool value = (disks[c][z][yy] << xx) & (0b10000000);
      if (x + xx >= 0 && x + xx < WIDTH + SIZE && y + yy >= 0 && y + yy < HEIGHT + SIZE)
	display_set_virt(x + xx, y + yy, value);
    }
}

// <<<
//******************************************************************************
// >>> jobs

typedef struct
{
  int x;
  int dx;
  int y;
  int z;
  int t;
  bool color;
  bool valid;
} job_t;

static job_t jobs[JOBS];

// <<<
//******************************************************************************
// >>> empty add

static bool is_empty(bool side)
{
  for (int j = 0; j < JOBS; j++)
    if (jobs[j].valid)
    {
      if (side) // left
      {
	if (jobs[j].y == 0 && jobs[j].x < 4 * SIZE)
	  return false;
      }
      else // right
      {
	if (jobs[j].y == 0 && jobs[j].x >= WIDTH - 4 * SIZE)
	  return false;
      }
    }
  return true;
}

static void add_job(bool side, bool fast, bool color)
{
  int j;
  for (j = 0; j < JOBS; j++)
    if (!jobs[j].valid)
      break;
  assert(j < JOBS);

  if (side) // left
  {
    jobs[j].x = -SIZE;
    jobs[j].dx = fast ? 2 : 1;
  }
  else // right
  {
    jobs[j].x = WIDTH + SIZE;
    jobs[j].dx = fast ? -2 : -1;
  }
  jobs[j].y = Y_START;
  jobs[j].z = 0;
  jobs[j].t = 0;
  jobs[j].color = color;
  jobs[j].valid = true;
}

// <<<
//******************************************************************************
// >>> run_jobs

static int collide_job(int j)
{
  for (int i = 0; i < JOBS; i++)
    if (i != j
      && jobs[i].valid
      && jobs[i].t == 0
      && jobs[j].t == 0
      && jobs[i].y == jobs[j].y
      && jobs[i].dx != jobs[j].dx
      && abs(jobs[i].x - jobs[j].x) < SIZE + 2)
      return i;
  return -1;
}

static bool fits_job(int j)
{
  for (int i = 0; i < JOBS; i++)
    if (i != j
      && jobs[i].valid
      && jobs[i].t == 0
      && jobs[j].t == 0
      && jobs[i].y == jobs[j].y
      && abs(jobs[i].x - jobs[j].x) < SIZE + 2)
      return false;
  return true;
}

static void run_job(int j)
{
  jobs[j].x += jobs[j].dx;
  jobs[j].z += jobs[j].dx;
  if ((jobs[j].dx > 0 && jobs[j].x >= WIDTH)
    || (jobs[j].dx < 0 && jobs[j].x < 0))
  {
    jobs[j].dx = -jobs[j].dx;
    jobs[j].x += jobs[j].dx;
    jobs[j].z += jobs[j].dx;
  }
  if (jobs[j].z < 0)
    jobs[j].z += ROTATION;
  if (jobs[j].z >= ROTATION)
    jobs[j].z -= ROTATION;

  if (jobs[j].t)
  {
    jobs[j].y += abs(jobs[j].dx);
    if (jobs[j].y >= HEIGHT + SIZE)
    {
      jobs[j].valid = false;
      return;
    }
    jobs[j].t--;
    if (jobs[j].t == 0 && !fits_job(j))
      jobs[j].t = ROW_DIST;
  }

  int i = collide_job(j);
  if (i >= 0)
  {
    if (jobs[i].color == jobs[j].color)
    {
      if (abs(jobs[j].x + jobs[j].dx - jobs[i].x - jobs[i].dx) < abs(jobs[j].x - jobs[i].x))
      {
	int dx = jobs[j].dx;
	jobs[j].dx = jobs[i].dx;
	jobs[i].dx = dx;
      }
    }
    else
    {
      jobs[i].t = ROW_DIST;
      jobs[j].t = ROW_DIST;
    }
  }
}

static void run_jobs()
{
  for (int j = 0; j < JOBS; j++)
    if (jobs[j].valid)
      run_job(j);
}

// <<<
//******************************************************************************
// >>> display_jobs

static void display_job(int j)
{
  disk(jobs[j].x, jobs[j].y, jobs[j].z, jobs[j].color);
}

static void display_jobs()
{
  for (int j = 0; j < JOBS; j++)
    if (jobs[j].valid)
      display_job(j);
}

// <<<
//******************************************************************************

static int count()
{
  int c = 0;
  for (int j = 0; j < JOBS; j++)
    if (jobs[j].valid && jobs[j].y == Y_START)
      c += jobs[j].color ? 1 : -1;
  return c;
}

int main(int ac, char *av[])
{
  int display_select = 0;
  int delay = DELAY;
  for (int ai = 1; ai < ac; ai++)
  {
    if (av[ai][1] == 's')
      display_select |= DISPLAY_SELECT_SP;
    else if (av[ai][1] == 'x')
      display_select |= DISPLAY_SELECT_GX;
    else if (isdigit(av[ai][1]))
    {
      delay = atoi(&av[ai][1]);
      assert(delay >= MIN_DELAY);
    }
    else
      assert(0);
  }

  srand(time(NULL));
  display_create(display_select);

  for (int t = 0; !display_button(); t--)
  {
    if (!t)
    {
      int c = count();
      bool side = rand() & 1;
      bool color = c > 3 ? false : c < -3 ? true : rand() & 1; // equal dist first row
      bool fast = rand() % 4 == 0;
      
      if (is_empty(side))
      {
	add_job(side, fast, color);
	t = MIN_NEW + rand() % (MAX_NEW - MIN_NEW);
      }
      else if (is_empty(!side))
      {
	add_job(!side, fast, color);
	t = MIN_NEW + rand() % (MAX_NEW - MIN_NEW);
      }
      else
      {
	t = MIN_NEW;
      }
    }

    run_jobs();

    display_clear();
    display_jobs();
    display_flush();
    usleep(delay * 1000);
  }

  display_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
