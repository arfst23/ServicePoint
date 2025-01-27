//******************************************************************************

#include "display_text.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define ROWS DISPLAY_TILES_VERT // 20
#define COLS DISPLAY_TILES_HORIZ // 56
#define SIZE DISPLAY_TILE_SIZE // 8

#define DELAY 35

#define delay(ms) usleep(ms * 1000)

#define LENGTH 14

//******************************************************************************
// >>> move_*_str

void move_out_str(int row, int col, const char *str)
{
  static const int off[14] = { 1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 7, 8 };
  int cols = strlen(str);
  assert(col + cols <= COLS);
  for (int t = 0; t < cols + 14 - 1; t++)
  {
    for (int i = 0; i < 14; i++)
    {
      int c = t - i;
      if (c >= 0 && c < cols)
        display_chr_offset(row, col + c, str[c], off[i]);
    }
    display_flush();
    delay(DELAY);
  } 
}

void move_in_str(int row, int col, const char *str)
{
  static const int off[14] = { 7, 7, 7, 6, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0 };
  int cols = strlen(str);
  assert(col + cols <= COLS);
  for (int t = 0; t < cols + 14 - 1; t++)
  {
    for (int i = 0; i < 14; i++)
    {
      int c = t - i;
      if (c >= 0 && c < cols)
        display_chr_offset(row, col + c, str[c], -off[i]);
    }
    display_flush();
    delay(DELAY);
  } 
}

void move_up_str(int row, int col, const char *str)
{
  static const int off0[18] = { 8, 8, 8, 8, 8, 8, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0 };
  static const int off1[18] = { 1, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8 };
  int cols = strlen(str);
  assert(col + cols <= COLS);

  for (int t = 0; t < cols + 18 - 1; t++)
  {
    for (int i = 0; i < 18; i++)
    {
      int c = t - i;
      if (c >= 0 && c < cols)
      {
        display_chr_offset(row, col + c, str[c], -off0[i]);
        display_chr_offset(row + 1, col + c, str[c], off1[i]);
      }
    }
    display_flush();
    delay(DELAY);
  } 
}

void move_2up_str(int row, int col, const char *str)
{
  static const int off0[22] = { -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -6, -5, -4, -3, -2, -2, -1, -1, 0 };
  static const int off1[22] = { -8, -8, -8, -8, -8, -8, -7, -6, -4, -2,  0,  2,  4,  6,  7,  8,  8,  8,  8,  8,  8, 8 };
  static const int off2[22] = {  1,  1,  2,  2,  3,  4,  5,  6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 8 };

  int cols = strlen(str);
  assert(col + cols <= COLS);

  for (int t = 0; t < cols + 22 - 1; t++)
  {
    for (int i = 0; i < 22; i++)
    {
      int c = t - i;
      if (c >= 0 && c < cols)
      {
        display_chr_offset(row, col + c, str[c], off0[i]);
        display_chr_offset(row + 1, col + c, str[c], off1[i]);
        display_chr_offset(row + 2, col + c, str[c], off2[i]);
      }
    }
    display_flush();
    delay(DELAY);
  } 
}

void move_3up_str(int row, int col, const char *str)
{
  static const int off0[25] = { -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -6, -5, -4, -3, -2, -2, -1, -1,  0 };
  static const int off1[25] = { -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -6, -3,  0,  2,  4,  6 , 8,  8,  8,  8,  8,  8,  8 };
  static const int off2[25] = { -8, -8, -8, -8, -8, -8, -7, -6, -4, -2,  0,  3,  6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 } ;
  static const int off3[25] = {  1,  1,  2,  2,  3,  4,  5,  6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 };

  int cols = strlen(str);
  assert(col + cols <= COLS);

  for (int t = 0; t < cols + 25 - 1; t++)
  {
    for (int i = 0; i < 25; i++)
    {
      int c = t - i;
      if (c >= 0 && c < cols)
      {
        display_chr_offset(row, col + c, str[c], off0[i]);
        display_chr_offset(row + 1, col + c, str[c], off1[i]);
        display_chr_offset(row + 2, col + c, str[c], off2[i]);
        display_chr_offset(row + 3, col + c, str[c], off3[i]);
      }
    }
    display_flush();
    delay(DELAY);
  } 
}

// <<<
//******************************************************************************

char src[ROWS][COLS + 2];

void fetch(const char *cmd)
{
  FILE *f = popen(cmd, "r");

  for (int row = 0; row < ROWS; row++)
  {
    char *str = fgets(&src[row][0], COLS + 1, f);
    if (str)
    {
      int len = strlen(str);
      if (str[--len] == '\n')
        str[len] = '\0';
    }
    else
    {
      src[row][0] = '\0';
    }
  }

  pclose(f);
}

char dst[ROWS][COLS + 2];

void diff()
{
  for (int row = 0; row < ROWS; row++)
  {
    if (!strcmp(src[row], dst[row]))
    {
      ;
      //printf("keep row=%d\n", row);
    }
    else if (row + 1 < ROWS && !strcmp(src[row], dst[row + 1]))
    {
      //printf("up 1 row=%d %s %s\n", row, src[row], dst[row+1]);
      if (dst[row][0])
        move_out_str(row, 0, dst[row]);
      move_up_str(row, 0, src[row]);
      strcpy(&dst[row][0], &src[row][0]);
      dst[row + 1][0] = '\0';
    }
    else if (row + 2 < ROWS && !strcmp(src[row], dst[row + 2]))
    {
      //printf("up 2 row=%d %d\n", row, row + 2);
      if (dst[row][0])
        move_out_str(row, 0, dst[row]);
      if (dst[row + 1][0])
        move_out_str(row + 1, 0, dst[row + 1]);
      move_2up_str(row, 0, src[row]);
      strcpy(&dst[row][0], &src[row][0]);
      dst[row + 1][0] = '\0';
      dst[row + 2][0] = '\0';
    }
    else if (row + 3 < ROWS && !strcmp(src[row], dst[row + 3]))
    {
      //printf("up 3 row=%d %d\n", row, row + 3);
      if (dst[row][0])
        move_out_str(row, 0, dst[row]);
      if (dst[row + 1][0])
        move_out_str(row + 1, 0, dst[row + 1]);
      if (dst[row + 2][0])
        move_out_str(row + 2, 0, dst[row + 2]);
      move_3up_str(row, 0, src[row]);
      strcpy(&dst[row][0], &src[row][0]);
      dst[row + 1][0] = '\0';
      dst[row + 2][0] = '\0';
      dst[row + 3][0] = '\0';
    }
    else
    {
      //printf("swap row=%d %s\n", row, dst[row]);
      if (dst[row][0])
        move_out_str(row, 0, dst[row]);
      move_in_str(row, 0, src[row]);
    }
    strcpy(&dst[row][0], &src[row][0]);
  }
}

//******************************************************************************

int main(int ac, char *av[])
{
  int display_select = 0;
  for (int ai = 1; ai < ac; ai++)
  {
    assert(av[ai][0] == '-');
    if (av[ai][1] == 's')
      display_select |= DISPLAY_SELECT_SP;
    else if (av[ai][1] == 'x')
      display_select |= DISPLAY_SELECT_GX;
    else
      assert(0);
  }

  display_create(display_select);

  memset(dst, 0, sizeof(dst));

#if 0
  display_str(0, 0, "AAA");
  display_str(1, 0, "BBB");
  display_str(2, 0, "CCC");
  display_str(4, 0, "DDD");
  display_str(5, 0, "EEE");
  display_str(8, 0, "FFF");
  display_str(9, 0, "GGG");
  display_str(13, 0, "HHH");
  display_str(14, 0, "III");
  display_flush();
  delay(2000);

  move_out_str(1, 0, "BBB");
  move_in_str(1, 0, "BBB");
  move_up_str(3, 0, "DDD");
  move_2up_str(6, 0, "FFF");
  move_2up_str(10, 0, "HHH");

  delay(2000);
#endif

#if 1
  while (!display_button())
  {
    fetch("query");
    diff();

    delay(5000);
  }
#endif
  
  display_free();

  return EXIT_SUCCESS;
}

//******************************************************************************
