//******************************************************************************

#include "graphix.h"

#include <stdlib.h>
#include <assert.h>

//******************************************************************************

int main(void)
{
  const char *color[] = { "black", "red", "green" };
  graphix gx = graphix_create("Graphix", 200, 100, 3, color);

  assert(graphix_width(gx) == 200);
  assert(graphix_height(gx) == 100);

  graphix_rect(gx, 50, 25, 100, 50, 1);
  graphix_line(gx, 0, 0, 199, 99, 2);
  graphix_flush(gx);

  for (;;)
    if (graphix_wait(gx) == 1)
      break;

  graphix_free(gx);
  return EXIT_SUCCESS;
}

//******************************************************************************
