//******************************************************************************

#include "display_text.h"
#include "display.h"

#include <stdlib.h>
#include <assert.h>
 
//******************************************************************************
 
int main(int ac, const char *av[])
{
  int display_select
      = ac == 1 ? DISPLAY_SELECT_GX | DISPLAY_SELECT_SP
      : av[1][0] == 'g' ? DISPLAY_SELECT_GX
      : av[1][0] == 's' ? DISPLAY_SELECT_SP
      : 0;
  display_create(display_select);

  display_str(0, 0, "Have a lot of fun!");
  display_chr(1, 0, '*', true);

  display_wait();
  display_free();
}
 
//******************************************************************************
