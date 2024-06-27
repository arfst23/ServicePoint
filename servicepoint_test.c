//******************************************************************************

#include "servicepoint.h"

#include <stdlib.h>
#include <assert.h>

//******************************************************************************

int main(void)
{
  sp_display display = sp_display_create();

  sp_bitmap bitmap = sp_bitmap_create(SP_DISPLAY_WIDTH, SP_DISPLAY_HEIGHT);
  sp_bitmap_fill(bitmap, false);

  for (int i = 0; i < 8; i++)
    sp_bitmap_set(bitmap, i, i, true);
  for (int i = 1; i <= 8; i++)
    sp_bitmap_set(bitmap, SP_DISPLAY_WIDTH - i, SP_DISPLAY_HEIGHT - i, true);
  sp_display_bitmap_set(display, bitmap, 0, 0);

  sp_display_free(display);
  return EXIT_SUCCESS;
}

//******************************************************************************
