//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

//******************************************************************************

int main(void)
{
  struct sockaddr_in sin =
  {
    .sin_family = AF_INET,
    .sin_port = htons(2342),
    .sin_addr = { inet_addr("172.23.42.29") },
  };

  int sock = socket(sin.sin_family, SOCK_DGRAM, IPPROTO_UDP);
  assert(sock >= 0);

  static unsigned char cmd[] = "\x00\x02";
  int sent = sendto(sock, cmd, 2, 0, (struct sockaddr *)&sin, sizeof(sin));
  assert(sent >= 0);

  close(sock);
  exit(EXIT_SUCCESS);
}

//******************************************************************************
