//******************************************************************************

#include "port.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

int fd_listen = -1;
int fd_connect = -1;

//******************************************************************************

void port_create(int port)
{
  assert(fd_connect < 0);

  fd_listen = socket(AF_INET, SOCK_STREAM, 0);
  assert(fd_listen >= 0);

  struct sockaddr_in addr_server;
  memset(&addr_server, 0, sizeof(struct sockaddr_in));
  addr_server.sin_family = AF_INET;
  addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
  addr_server.sin_port = htons(port);
  int ret = bind(fd_listen, (struct sockaddr*)&addr_server, sizeof(addr_server));
  assert(ret >= 0);

  ret = listen(fd_listen, 1);
  assert(ret >= 0);

  fd_connect = accept(fd_listen, NULL, NULL);
  assert(fd_connect >= 0);
}

void port_close()
{
  assert(fd_connect >= 0);
  close(fd_connect);
  fd_connect = -1;
  close(fd_listen);
  fd_listen = -1;
}

//******************************************************************************

void port_write(const char *str, int len)
{
  assert(fd_connect >= 0);
  int n = write(fd_connect, str, len);
  assert(n == len);
}

int port_read_char()
{
  assert(fd_connect >= 0);
  char c = '\0';
  int n = read(fd_connect, &c, 1);
  if (n == 0)
    return EOF;
  return c;
}

//******************************************************************************
