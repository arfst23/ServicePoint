//******************************************************************************

#include "sp.h"

#include <stdlib.h>
#include <assert.h>

//******************************************************************************

int main(void)
{
  sp_Connection *connection = sp_connection_open("172.23.42.29:2342");
  assert(connection);

  sp_PixelGrid *pixels = sp_pixel_grid_new(sp_PIXEL_WIDTH, sp_PIXEL_HEIGHT);
  sp_pixel_grid_fill(pixels, false);

  for (size_t i = 0; i < 8; i++)
    sp_pixel_grid_set(pixels, i, i, true);
  for (size_t i = 1; i <= 8; i++)
    sp_pixel_grid_set(pixels, sp_PIXEL_WIDTH - i, sp_PIXEL_HEIGHT - i, true);

  sp_Command *command = sp_command_bitmap_linear_win(0, 0, pixels, Uncompressed);
  sp_Packet *packet = sp_packet_from_command(command);
  assert(!sp_connection_send(connection, packet));

  sp_connection_dealloc(connection);
  return EXIT_SUCCESS;
}

//******************************************************************************
