//******************************************************************************

#include "display_graphix.h"
#include "graphix.h"
#include "servicepoint.h"

#include <stdlib.h>
#include <stdbool.h>

//******************************************************************************

int main(void)
{
  graphix gx = display_graphix_create();

  for (int i = 0; i < 8; i++)
    display_graphix_set(gx, i, i, true);
  for (int i = 1; i <= 8; i++)
    display_graphix_set(gx, SP_DISPLAY_WIDTH - i, SP_DISPLAY_HEIGHT - i, true);
  graphix_flush(gx);
  graphix_wait(gx);

  for (sp_size y = 0; y < SP_DISPLAY_HEIGHT; y++)
    for (sp_size x = 0; x < SP_DISPLAY_WIDTH; x++)
      display_graphix_set(gx, x, y, true);
  graphix_flush(gx);
  graphix_wait(gx);

  display_graphix_clear(gx);
  graphix_flush(gx);
  graphix_wait(gx);

  graphix_free(gx);
  return EXIT_SUCCESS;
}

//******************************************************************************
