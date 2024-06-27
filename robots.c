//******************************************************************************

#include "robots_auto.h"
#include "robots_bsd.h"
#include "robots.h"
#include "port.h"
#include "term.h"
#include "display_text.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

//******************************************************************************

#define ROBOTS_LEVEL 4
#define ROBOTS_OFFSET 4
#define ROBOTS_MAX 32

#define ROBOTS_KEY_AUTO 'a'
#define ROBOTS_KEY_BSD 'd'

#define SCORE_ROBOT 10
#define SCORE_WAIT 1
#define SCORE_TELE -2

typedef struct
{
  char screen[SCREEN_ROWS][SCREEN_COLS];

  int pos_row;
  int pos_col;

  int robots;
  int junks;

  int level;
  int score;
} robots;

#define ROBOTS_PORT 8080

static int param_delay = 0;

#define ROBOTS_IN_KEY 0
#define ROBOTS_IN_AUTO 1
#define ROBOTS_IN_BSD 2
#define ROBOTS_IN_GUIDED 3
#define ROBOTS_IN_PORT 4

static int param_in = 0;

#define ROBOTS_OUT_GX DISPLAY_SELECT_GX
#define ROBOTS_OUT_SP DISPLAY_SELECT_SP

#define ROBOTS_OUT_DISP (ROBOTS_OUT_GX | ROBOTS_OUT_SP)
#define ROBOTS_OUT_LOG 4
#define ROBOTS_OUT_TERM 8

#define ROBOTS_WAIT_LEVEL 750
#define ROBOTS_WAIT_WAIT 50
#define ROBOTS_WAIT_MOVE 75

static int param_out = 0;

//******************************************************************************
// >>> init / level

void robots_init(robots *state)
{
  memset(state->screen, ROBOTS_EMPTY, SCREEN_ROWS * SCREEN_COLS);
  state->pos_row = 0;
  state->pos_col = 0;
  state->robots = 0;
  state->junks = 0;
  state->level = 0;
  state->score = 0;

  srand(time(NULL));
}

void robots_level(robots *state)
{
  memset(state->screen, ROBOTS_EMPTY, SCREEN_ROWS * SCREEN_COLS);

  state->level++;
  state->robots = ROBOTS_OFFSET + state->level * ROBOTS_LEVEL;
  if (state->robots > ROBOTS_MAX)
    state->robots = ROBOTS_MAX;
  state->junks = 0;

  state->pos_row = rand() % SCREEN_ROWS;
  state->pos_col = rand() % SCREEN_COLS;
  state->screen[state->pos_row][state->pos_col] = ROBOTS_PLAYER;

  for (int i = 0; i < state->robots; i++)
  {
    int row = rand() % SCREEN_ROWS;
    int col = rand() % SCREEN_COLS;
    if (state->screen[row][col] == ROBOTS_EMPTY)
      state->screen[row][col] = ROBOTS_ROBOT;
    else
      i--;
  }

  rand();
}

// <<<
//******************************************************************************
// >>> move

static int robots_move_player(robots *state, int key)
{
  state->screen[state->pos_row][state->pos_col] = ROBOTS_EMPTY;

  if (key == ROBOTS_KEY_TELE)
  {
    state->pos_row = rand() % SCREEN_ROWS;
    state->pos_col = rand() % SCREEN_COLS;
    state->score += SCORE_TELE;
    if (state->score < 0)
      state->score = 0;
  }

  if (state->pos_row > 0
      && (key == ROBOTS_KEY_NE || key == ROBOTS_KEY_N || key == ROBOTS_KEY_NW))
    state->pos_row--;

  if (state->pos_row < SCREEN_ROWS - 1
      && (key == ROBOTS_KEY_SE || key == ROBOTS_KEY_S || key == ROBOTS_KEY_SW))
    state->pos_row++;

  if (state->pos_col > 0
      && (key == ROBOTS_KEY_NW || key == ROBOTS_KEY_W || key == ROBOTS_KEY_SW))
    state->pos_col--;

  if (state->pos_col < SCREEN_COLS - 1
      && (key == ROBOTS_KEY_NE || key == ROBOTS_KEY_E || key == ROBOTS_KEY_SE))
    state->pos_col++;

  switch (state->screen[state->pos_row][state->pos_col])
  {
  case ROBOTS_EMPTY:
    state->screen[state->pos_row][state->pos_col] = ROBOTS_PLAYER;
    break;
  case ROBOTS_JUNK:
  case ROBOTS_ROBOT:
    state->screen[state->pos_row][state->pos_col] = ROBOTS_DEAD;
    return 1;
    break;
  default:
    assert(0);
  }
  return 0;
}

static int robots_move_robot(robots *state, int row, int col)
{
  if (state->screen[row][col] != ROBOTS_ROBOT)
    return 0;

  state->screen[row][col] = ROBOTS_EMPTY;
  if (col > state->pos_col)
    col--;
  else if (col < state->pos_col)
    col++;
  if (row > state->pos_row)
    row--;
  else if (row < state->pos_row)
    row++;
  switch (state->screen[row][col])
  {
  case ROBOTS_EMPTY:
    state->screen[row][col] = ROBOTS_ROBOT;
    break;
  case ROBOTS_ROBOT:
    state->screen[row][col] = ROBOTS_JUNK;
    state->robots -= 2;
    state->junks++;
    state->score += 2 * SCORE_ROBOT;
    break;
  case ROBOTS_JUNK:
    state->robots--;
    state->score += SCORE_ROBOT;
    break;
  case ROBOTS_PLAYER:
  case ROBOTS_DEAD:
    state->screen[row][col] = ROBOTS_DEAD;
    return 1;
  default:
    assert(0);
  }
  return 0;
}

int robots_move(robots *state, int key)
{
  int dead = robots_move_player(state, key);

  for (int col = state->pos_col - 1; col >= 0; col--)
    dead += robots_move_robot(state, state->pos_row, col);
  for (int col = state->pos_col + 1; col < SCREEN_COLS; col++)
    dead += robots_move_robot(state, state->pos_row, col);
  for (int row = state->pos_row - 1; row >= 0; row--)
    for (int col = 0; col < SCREEN_COLS; col++)
      dead += robots_move_robot(state, row, col);
  for (int row = state->pos_row + 1; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
      dead += robots_move_robot(state, row, col);

  return dead;
}

// <<<
//******************************************************************************
// >>> print

void robots_print_log(robots *state)
{
  putchar('+');
  for (int col = 0; col < SCREEN_COLS; col++)
    putchar('-');
  putchar('+');
  putchar('\n');

  for (int row = 0; row < SCREEN_ROWS; row++)
  {
    putchar('|');
    fwrite(state->screen[row], SCREEN_COLS, 1, stdout);
    putchar('|');
    putchar('\n');
  }

  putchar('+');
  for (int col = 0; col < SCREEN_COLS; col++)
    putchar('-');
  putchar('+');
  putchar('\n');
  printf("score: %d\n", state->score);
  putchar('\n');
}

static void robots_print_port(robots *state)
{
  port_write("+", 1);
  for (int col = 0; col < SCREEN_COLS; col++)
    port_write("-", 1);
  port_write("+\n", 2);

  for (int row = 0; row < SCREEN_ROWS; row++)
  {
    port_write("|", 1);
    port_write(state->screen[row], SCREEN_COLS);
    port_write("|\n", 2);
  }

  port_write("+", 1);
  for (int col = 0; col < SCREEN_COLS; col++)
    port_write("-", 1);
  port_write("+\n", 2);
  
  char str[32];
  int n = sprintf(str, "%05d\n", state->score);
  port_write(str, n);
}

void robots_print_disp(robots *state)
{
  for (int row = 0; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
      display_chr(row, col, state->screen[row][col],
	  state->screen[row][col] == ROBOTS_PLAYER
	  || state->screen[row][col] == ROBOTS_DEAD);
  display_flush();
}

void robots_print_term(robots *state)
{
  term_home();

  term_color(TERM_GREEN);
  putchar('+');
  for (int col = 0; col < SCREEN_COLS; col++)
    putchar('-');
  putchar('+');
  term_color(TERM_WHITE);
  putchar('\n');

  for (int row = 0; row < SCREEN_ROWS; row++)
  {
    term_color(TERM_GREEN);
    int color_prev = TERM_GREEN;
    putchar('|');
    for (int col = 0; col < SCREEN_COLS; col++)
    {
      int color_next;
      switch (state->screen[row][col])
      {
      case ROBOTS_EMPTY:
	color_next = color_prev;
	break;
      case ROBOTS_ROBOT:
	color_next = TERM_RED;
	break;
      case ROBOTS_JUNK:
	color_next = TERM_GREEN;
	break;
      case ROBOTS_PLAYER:
      case ROBOTS_DEAD:
	color_next = TERM_YELLOW;
	break;
      default:
	assert(0);
      }
      if (color_next != color_prev)
      {
	term_color(color_next);
	color_prev = color_next;
      }
      putchar(state->screen[row][col]);
    }

    if (color_prev != TERM_GREEN)
      term_color(TERM_GREEN);
    putchar('|');

    static const char *str[] =
    {
      "BSD robots",
      NULL,
      "y k u",
      " \\|/",
      "h- -l",
      " /|\\",
      "b j n",
      NULL,
      "w: wait",
      "q: quit",
      NULL,
      "+: robot",
      "*: junk heap",
      "O: you",
    };
    if (row < 14 && str[row])
      printf("  %s", str[row]);

    if (row == 15)
      printf("  robots: %d", state->robots);

    if (row == 16)
      printf("  junk heaps: %d", state->junks);

    if (row == 18)
    {
      term_color(TERM_YELLOW);
      printf("  level: %d", state->level);
    }

    if (row == 19)
    {
      term_color(TERM_YELLOW);
      printf("  score: %d", state->score);
    }

    term_color(TERM_WHITE);
    putchar('\n');
  }

  term_color(TERM_GREEN);
  putchar('+');
  for (int col = 0; col < SCREEN_COLS; col++)
    putchar('-');
  putchar('+');
  term_color(TERM_WHITE);
  putchar('\n');
}

void robots_print(robots *state)
{
  if (param_out & ROBOTS_OUT_DISP)
    robots_print_disp(state);
  if (param_out & ROBOTS_OUT_TERM)
    robots_print_term(state);
  if (param_out & ROBOTS_OUT_LOG)
    robots_print_log(state);
}

// <<<
//******************************************************************************
// >>> key

static int robots_key_term(__attribute__((unused))robots *state)
{
  return getchar();
}

static int robots_key_auto(robots *state)
{
  if (param_delay)
    usleep(ROBOTS_WAIT_MOVE * 1000);
  return robots_strategy_auto(state->screen);
  return 0;
}

static int robots_key_bsd(robots *state)
{
  if (param_delay)
    usleep(ROBOTS_WAIT_MOVE * 1000);
  return robots_strategy_bsd(state->screen);
  return 0;
}

static int robots_key_guided(robots *state)
{
  int key_auto = robots_strategy_auto(state->screen);
  int key_bsd = robots_strategy_bsd(state->screen);
  if (key_auto == key_bsd)
  {
    if (param_delay)
      usleep(ROBOTS_WAIT_MOVE * 1000);
    return key_auto;
  }
  return getchar();
}

static int robots_key_port(robots *state)
{
  robots_print_port(state);
  for (;;)
  {
    int key = port_read_char();
    if (key == ROBOTS_KEY_N || key == ROBOTS_KEY_NW || key == ROBOTS_KEY_W
	|| key == ROBOTS_KEY_SW || key == ROBOTS_KEY_S || key == ROBOTS_KEY_SE
	|| key == ROBOTS_KEY_E || key == ROBOTS_KEY_NE || key == ROBOTS_KEY_X
	|| key == ROBOTS_KEY_TELE || key == ROBOTS_KEY_WAIT)
      return key;
  }
}

static int robots_key(robots *state)
{
  switch (param_in)
  {
  case ROBOTS_IN_KEY:
    return robots_key_term(state);
  case ROBOTS_IN_AUTO:
    return robots_key_auto(state);
  case ROBOTS_IN_BSD:
    return robots_key_bsd(state);
  case ROBOTS_IN_GUIDED:
    return robots_key_guided(state);
  case ROBOTS_IN_PORT:
    return robots_key_port(state);
  default:
    assert(0);
  }
}

// <<<
//******************************************************************************
// >>> run

static int robots_wait(robots *state)
{
  for (;;)
  {
    int dead = robots_move(state, ROBOTS_KEY_X);
    if (dead)
      return dead;
    state->score += SCORE_WAIT;
    if (state->robots == 0)
      return 0;

    if (param_delay)
    {
      robots_print(state);
      usleep(ROBOTS_WAIT_WAIT * 1000);
    }
  }
}

static int robots_run(void)
{
  robots rob;
  robots_init(&rob);
  robots_level(&rob);

  for (int dead = 0; !dead;)
  {
    if (rob.robots == 0)
    {
      if (param_delay)
      {
	robots_print(&rob);
	usleep(ROBOTS_WAIT_LEVEL * 1000);
      }
      robots_level(&rob);
    }
    robots_print(&rob);

    int key = robots_key(&rob);
    if (key == ROBOTS_KEY_QUIT || key == EOF)
      return rob.score;

    if (key == ROBOTS_KEY_AUTO)
      key = robots_key_auto(&rob);
    else if (key == ROBOTS_KEY_BSD)
      key = robots_key_bsd(&rob);

    if (key == ROBOTS_KEY_N || key == ROBOTS_KEY_NW || key == ROBOTS_KEY_W
	|| key == ROBOTS_KEY_SW || key == ROBOTS_KEY_S || key == ROBOTS_KEY_SE
	|| key == ROBOTS_KEY_E || key == ROBOTS_KEY_NE || key == ROBOTS_KEY_X
	|| key == ROBOTS_KEY_TELE)
      dead = robots_move(&rob, key);
    else if (key == ROBOTS_KEY_WAIT)
      dead = robots_wait(&rob);
  }
  robots_print(&rob);
  if (param_in == ROBOTS_IN_PORT)
    robots_print_port(&rob);
  return rob.score;
}

// <<<
//******************************************************************************

int main(int ac, const char *av[])
{
  for (int ai = 1; ai < ac; ai++)
  {
    assert(av[ai][0] == '-');
    switch (av[ai][1])
    {
    case 'l':
      param_out |= ROBOTS_OUT_LOG;
      break;
    case 't':
      param_out |= ROBOTS_OUT_TERM;
      break;
    case 'x':
      param_out |= ROBOTS_OUT_GX;
      break;
    case 's':
      param_out |= ROBOTS_OUT_SP;
      break;
    case 'd':
      param_delay = 1;
      break;
    case 'a':
      param_in = ROBOTS_IN_AUTO;
      break;
    case 'b':
      param_in = ROBOTS_IN_BSD;
      break;
    case 'g':
      param_in = ROBOTS_IN_GUIDED;
      break;
    case 'p':
      param_in = ROBOTS_IN_PORT;
      break;
    default:
      assert(0);
    }
  }

  term_raw();

  if (param_out & ROBOTS_OUT_TERM)
    term_clear();

  if (param_out & ROBOTS_OUT_DISP)
    display_create(param_out & ROBOTS_OUT_DISP);

  if (param_in == ROBOTS_IN_PORT)
    port_create(ROBOTS_PORT);

  int score = robots_run();

  term_restore();

  if (param_out & ROBOTS_OUT_DISP)
    display_free();

  if (param_in == ROBOTS_IN_PORT)
    port_close();

  printf("score: %d\n", score);

  return EXIT_SUCCESS;
}

//******************************************************************************
