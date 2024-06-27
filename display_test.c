//******************************************************************************
 
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

  for (int i = 0; i < 36; i++)
    display_set_virt(i, i, true);
  for (int i = 1; i <= 36; i++)
    display_set_virt(DISPLAY_WIDTH - i, DISPLAY_HEIGHT_VIRT - i, true);
  display_flush();

  display_wait();
  display_free();
}
 
//******************************************************************************
