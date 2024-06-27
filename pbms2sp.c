//******************************************************************************
 
#include "display.h"

#define HAVE_BOOL 1
#include <pbm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define SEPARATORS (DISPLAY_TILES_VERT - 1)

#define MIN_DELAY 40
#define MAX_DELAY 150
#define DELAY 70

//******************************************************************************
 
int main(int ac, const char *av[])
{
  int display_select = 0;
  bit on_value = PBM_WHITE;
  int delay = DELAY;

  int ai;
  for (ai = 1; ai < ac; ai++)
  {
    if (av[ai][0] != '-')
      break;
    if (av[ai][1] == 'r')
      on_value = PBM_BLACK;
    else if (av[ai][1] == 's')
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

  int frames = ac - ai;
  char bits[frames][DISPLAY_HEIGHT][DISPLAY_WIDTH];
  memset(bits, 0, frames * DISPLAY_HEIGHT * DISPLAY_WIDTH * sizeof(char));

  for (int frame = 0; frame < frames; frame++)
  {
    int bitmap_width, bitmap_height;
    FILE *fh = fopen(av[ai + frame], "r");
    assert(fh);
    bit** bitmap = pbm_readpbm(fh, &bitmap_width, &bitmap_height);
    assert(bitmap);
    fclose(fh);

    float ratio_x = (float)bitmap_width / (float)DISPLAY_WIDTH;
    float ratio_y = (float)bitmap_height / (float)DISPLAY_HEIGHT_VIRT;
    float ratio = ratio_x > ratio_y ? ratio_x : ratio_y;

    int scaled_width = (int)((float)bitmap_width / ratio + 0.5);
    int scaled_height = (int)((float)bitmap_height / ratio + 0.5);

    int offset_x = (DISPLAY_WIDTH - scaled_width) / 2;
    int offset_y = (DISPLAY_HEIGHT_VIRT - scaled_height) / 2;

    for (int display_y = offset_y; display_y < DISPLAY_HEIGHT - offset_y; display_y++)
      for (int display_x = offset_x; display_x < DISPLAY_WIDTH - offset_x; display_x++)
      {
	int virtual_y = display_y + (display_y / DISPLAY_TILE_SIZE) * DISPLAY_SEPARATOR_WIDTH;
	int bitmap_x = (int)((float)(display_x - offset_x) * ratio + 0.5);
	int bitmap_y = (int)((float)(virtual_y - offset_y) * ratio + 0.5);

	bits[frame][display_y][display_x] = bitmap[bitmap_y][bitmap_x] == on_value ? 1 : 0;
      }
    pbm_freearray(bitmap, bitmap_height);
  }

  display_create(display_select);
  
  for (int frame = 0; ; frame = (frame + 1) % frames)
  {
    for (int display_y = 0; display_y < DISPLAY_HEIGHT; display_y++)
      for (int display_x = 0; display_x < DISPLAY_WIDTH; display_x++)
	display_set(display_x, display_y, bits[frame][display_y][display_x]);
    display_flush();

    int button = display_button();
    if (button == 3) // right
      break;
    if (button == 1) // left
      display_wait();
    else if (button == 2) // center
      delay = DELAY;
    else if (button == 4) // up
    {
      int new = (int)((float)delay * 0.91);
      if (new >= MIN_DELAY)
	delay = new;
    }
    else if (button == 5) // down
    {
      int new = (int)((float)delay * 1.1);
      if (new <= MAX_DELAY)
	delay = new;
    }
    usleep(delay * 1000);
  }

  display_free();
  return EXIT_SUCCESS;
}
 
//******************************************************************************
