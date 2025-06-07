//******************************************************************************

// ffmpeg -i $f -f image2pipe -vcodec ppm - | ppms2sp -x

#include <ppm.h>

#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define GAMMA 1.6f
#define SKIP 15

#define FRAME_TIME 41

int gamma_(int x)
{
  return (int)(powf((float)x / 255.0f, GAMMA) * 255.0f);
}

uint64_t time_ms(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (uint64_t)ts.tv_sec * 1000L + (uint64_t)ts.tv_nsec / 1000000L;
}

void delay(int ms)
{
  usleep(ms * 1000);
}

//******************************************************************************

int main(int ac, char *av[])
{
  int gamma__[256];
  for (int x = 0; x < 256; x++)
    gamma__[x] = gamma_(x);

  ppm_init(&ac, av);

  int display_select = 0;
  int frame_time = 0;
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
      assert(isdigit(av[ai][1]));
      frame_time = atoi(&av[ai][1]);
      printf("frame_time %d\n", frame_time);
      break;
    }
  }

  display_create(display_select);

  int cols, rows;
  pixval maxval;
  int format;
  ppm_readppminit(stdin, &cols, &rows, &maxval, &format);

  assert(cols == DISPLAY_WIDTH);
  assert(rows == DISPLAY_HEIGHT_VIRT);

  pixel **pixels = ppm_allocarray(cols, rows);
  assert(pixels);

  uint64_t t0 = time_ms();

  while (!feof(stdin))
  {
    for (int row = 0; row < rows; ++row)
      ppm_readppmrow(stdin, pixels[row], cols, maxval, format);

    int this[cols];
    int next[cols];
    memset(next, 0, sizeof(this));

    for (int row = 0; row < rows; ++row)
    {
      memcpy(this, next, sizeof(next));
      memset(next, 0, sizeof(next));

      for (int col = 0; col < cols; ++col)
      {
        // 0.299 r + 0.587 g + 0.114 b -> 0..255
        int value = (76 * gamma__[pixels[row][col].r]
            + 150 * gamma__[pixels[row][col].g]
            + 29 * gamma__[pixels[row][col].b]) / maxval;

        // floyd-steinberg dithering
        this[col] += value;
        value = this[col] >= 128 ? 255 : 0;
        int error = this[col] - value;
        if (col - 1 >= 0)
          next[col - 1] += error * 3 / 16;
        next[col] += error * 5 / 16;
        if (col + 1 < cols)
        {
          this[col + 1] += error * 7 / 16;
          next[col + 1] += error * 1 / 16;
        }

        display_set_virt(col, row, value);
      }
    }

    display_flush();

    if (SKIP)
    {
      char buf[SKIP];
      int n = fread(buf, 1, SKIP, stdin);
      if (n < SKIP)
        break;
    }

    t0 += frame_time;
    uint64_t t = time_ms();
    int dt = (int)(t0 - t);
    if (dt > 0)
    {
      //printf("delay %d\n", dt);
      delay(dt);
    }
    else
    {
      //printf("skip %d\n", -dt);
      t0 = t;
    }
  }

  ppm_freearray(pixels, rows);

  display_free();

  return 0;
}

//******************************************************************************