//******************************************************************************

#include "servicepoint.h"

#include <stdlib.h>
#include <assert.h>

//******************************************************************************

int main(void)
{
  sp_display display = sp_display_create();
  sp_display_reset(display);
  sp_display_free(display);
  return EXIT_SUCCESS;
}

//******************************************************************************
