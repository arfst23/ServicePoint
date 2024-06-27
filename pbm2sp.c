//******************************************************************************
 
#include "display.h"

#define HAVE_BOOL 1
#include <pbm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SEPARATORS (DISPLAY_TILES_VERT - 1)

//******************************************************************************
 
int main(int ac, const char *av[])
{
  int display_select = 0;
  bit on_value = PBM_WHITE;
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
    case 'r':
      on_value = PBM_BLACK;
      break;
    default:
      assert(0);
    }
  }

  int bitmap_width, bitmap_height;
  bit** bitmap = pbm_readpbm(stdin, &bitmap_width, &bitmap_height);
  assert(bitmap);

  float ratio_x = (float)bitmap_width / (float)DISPLAY_WIDTH;
  float ratio_y = (float)bitmap_height / (float)DISPLAY_HEIGHT_VIRT;
  float ratio = ratio_x > ratio_y ? ratio_x : ratio_y;

  int scaled_width = (int)((float)bitmap_width / ratio + 0.5);
  int scaled_height = (int)((float)bitmap_height / ratio + 0.5);

  int offset_x = (DISPLAY_WIDTH - scaled_width) / 2;
  int offset_y = (DISPLAY_HEIGHT_VIRT - scaled_height) / 2;

  display_create(display_select);
  
  for (int display_y = offset_y; display_y < DISPLAY_HEIGHT - offset_y; display_y++)
    for (int display_x = offset_x; display_x < DISPLAY_WIDTH - offset_x; display_x++)
    {
      int virtual_y = display_y + (display_y / DISPLAY_TILE_SIZE) * DISPLAY_SEPARATOR_WIDTH;
      int bitmap_x = (int)((float)(display_x - offset_x) * ratio + 0.5);
      int bitmap_y = (int)((float)(virtual_y - offset_y) * ratio + 0.5);

      display_set(display_x, display_y, bitmap[bitmap_y][bitmap_x] == on_value);
    }
  display_flush();

  pbm_freearray(bitmap, bitmap_height);

  display_wait();
  display_free();
  return EXIT_SUCCESS;
}
 
//******************************************************************************
