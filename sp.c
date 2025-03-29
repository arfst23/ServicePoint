//******************************************************************************

#include "sp.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

#define SP_ADDR "172.23.42.29"
#define CMD_BITMAPLINEAR 0x0012
#define SUBCMD_BITMAP_NORMAL 0x0

typedef struct __attribute__((__packed__))
{
  uint16_t command;
  uint16_t offset;
  uint16_t length;
  uint16_t subcommand;
  uint16_t reserved;
  uint8_t bits[SP_HEIGHT][SP_TILES_HORIZ];
} packet_t;

static int sock = -1;
static struct sockaddr_in sin;
static packet_t packet;

//******************************************************************************

void sp_create()
{
  assert(sock < 0);

  sin.sin_family = AF_INET;
  sin.sin_port = htons(2342);
  sin.sin_addr.s_addr = inet_addr(SP_ADDR);

  sock = socket(sin.sin_family, SOCK_DGRAM, IPPROTO_UDP);
  assert(sock >= 0);

  packet.command = htons(CMD_BITMAPLINEAR);
  packet.offset = 0;
  packet.length = htons(SP_HEIGHT * SP_TILES_HORIZ);
  packet.subcommand = htons(SUBCMD_BITMAP_NORMAL);
  packet.reserved = 0;
  memset(&packet.bits[0][0], 0, SP_HEIGHT * SP_TILES_HORIZ);
}

//******************************************************************************

void sp_free()
{
  assert(sock >= 0);
  close(sock);
  sock = -1;
}

//******************************************************************************

void sp_send()
{
  assert(sock >= 0);
  int rc = sendto(sock, &packet, sizeof(packet_t), 0, (struct sockaddr *)&sin, sizeof(sin));
  assert(rc >= 0);
}

//******************************************************************************

void sp_clear()
{
  assert(sock >= 0);
  memset(&packet.bits[0][0], 0, SP_HEIGHT * SP_TILES_HORIZ);
}

//******************************************************************************

void sp_set(int y, int x, bool val)
{
  assert(y >= 0 && y < SP_HEIGHT);
  assert(x >= 0 && x < SP_WIDTH);
  assert(sock >= 0);
  int col = x >> 3;
  int bit = 7 - (x & 0b111);
  
  if (val)
    packet.bits[y][col] |= 1 << bit;
  else
    packet.bits[y][col] &= ~(1 << bit);
}

//******************************************************************************

uint8_t *get(int y, int col)
{
  assert(y >= 0 && y < SP_HEIGHT);
  assert(col >= 0 && col < SP_TILES_HORIZ);
  assert(sock >= 0);
  return &packet.bits[y][col];
}

//******************************************************************************
