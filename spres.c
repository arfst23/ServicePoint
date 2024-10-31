//******************************************************************************

#include "servicepoint.h"

#include <stdlib.h>
#include <assert.h>

//******************************************************************************

int main(void)
{
  sp_connection connection = sp_connection_open(ADDRESS);
  assert(connection);
  sp_command command = sp_command_hard_reset();
  assert(command);
  bool sent = sp_connection_send_command(connection, command);
  assert(sent);
  sp_connection_free(connection);
  return EXIT_SUCCESS;
}

//******************************************************************************
