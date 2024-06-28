//******************************************************************************

#include "robots_bsd.h"
#include "robots.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TO_X 0
#define TO_N 1
#define TO_NW 2
#define TO_W 3
#define TO_SW 4
#define TO_S 5
#define TO_SE 6
#define TO_E 7
#define TO_NE 8

//******************************************************************************
// >>> bsd code

#define ROBOTS_MAX 32

typedef struct
{
  int y, x;
} position;

position player;
position robot[ROBOTS_MAX];
position junk[ROBOTS_MAX];

int robots;
int junks;

const char* valid;

//******************************************************************************

int sign(int n)
{
  if (n < 0)
    return -1;
  else if (n > 0)
    return 1;
  else
    return 0;
}

#define ABS(a) (((a)>0)?(a):-(a))
#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)<(b))?(b):(a))

// return "move" number distance of the two coordinates
static int distance(int x1, int y1, int x2, int y2)
{
  return MAX(ABS(ABS(x1) - ABS(x2)), ABS(ABS(y1) - ABS(y2)));
}

//******************************************************************************

// return x coordinate moves
static int x_inc(int dir)
{
  switch(dir)
  {
  case ROBOTS_KEY_NE:
  case ROBOTS_KEY_E:
  case ROBOTS_KEY_SE:
    return 1;
  case ROBOTS_KEY_N:
  case ROBOTS_KEY_X:
  case ROBOTS_KEY_S:
    return 0;
  case ROBOTS_KEY_NW:
  case ROBOTS_KEY_W:
  case ROBOTS_KEY_SW:
    return -1;
  default:
    assert(0);
  }
}

// return y coordinate moves
static int y_inc(int dir)
{
  switch(dir)
  {
  case ROBOTS_KEY_SW:
  case ROBOTS_KEY_S:
  case ROBOTS_KEY_SE:
    return 1;
  case ROBOTS_KEY_W:
  case ROBOTS_KEY_X:
  case ROBOTS_KEY_E:
    return 0;
  case ROBOTS_KEY_NW:
  case ROBOTS_KEY_N:
  case ROBOTS_KEY_NE:
    return -1;
  default:
  assert(0);
  }
}

//******************************************************************************

// find possible moves
static const char* find_moves()
{
  static char ans[10];
  char *p = ans;

  if (valid[TO_X])
    *p++ = ROBOTS_KEY_X;
  if (valid[TO_W])
    *p++ = ROBOTS_KEY_W;
  if (valid[TO_S])
    *p++ = ROBOTS_KEY_S;
  if (valid[TO_N])
    *p++ = ROBOTS_KEY_N;
  if (valid[TO_E])
    *p++ = ROBOTS_KEY_E;
  if (valid[TO_NW])
    *p++ = ROBOTS_KEY_NW;
  if (valid[TO_NE])
    *p++ = ROBOTS_KEY_NW;
  if (valid[TO_SW])
    *p++ = ROBOTS_KEY_SW;
  if (valid[TO_SE])
    *p++ = ROBOTS_KEY_SE;
  if (p == ans)
    *p++ = ROBOTS_KEY_TELE;
  *p = '\0';

  return ans;
}

//******************************************************************************

// return the robot closest to us and put in dist its distance
static position* closest_robot(int *dist)
{
  position *minrob = NULL;

  int mindist = 1000000;
  for (position *rob = robot; rob < &robot[robots]; rob++)
  {
    int tdist = distance(player.x, player.y, rob->x, rob->y);
    if (tdist < mindist)
    {
      minrob = rob;
      mindist = tdist;
    }
  }

  *dist = mindist;
  return minrob;
}
			
// return the heap closest to us and put in dist its distance
static position *closest_junk(int *dist)
{
  position *minhp = NULL;

  int mindist = 1000000;
  for (position *hp = junk; hp < &junk[ROBOTS_MAX]; hp++)
  {
    if (hp->x == 0 && hp->y == 0)
      break;
    int tdist = distance(player.x, player.y, hp->x, hp->y);
    if (tdist < mindist)
    {
      minhp = hp;
      mindist = tdist;
    }
  }

  *dist = mindist;
  return minhp;
}

//******************************************************************************

// move as close to the given direction as possible
static int move_towards(int dx, int dy)
{
  char valid_moves[10], best_move;
  strcpy(valid_moves, find_moves());

  best_move = valid_moves[0]; 
  if (best_move != ROBOTS_KEY_TELE)
  {
    int mv_x = x_inc(best_move);
    int mv_y = y_inc(best_move);
    int move_judge = ABS(mv_x - dx) + ABS(mv_y - dy);
    for (char *ptr = &valid_moves[1]; *ptr != '\0'; ptr++)
    {
      mv_x = x_inc(*ptr);
      mv_y = y_inc(*ptr);
      int cur_judge = ABS(mv_x - dx) + ABS(mv_y - dy);
      if (cur_judge < move_judge)
      {
	move_judge = cur_judge;
	best_move = *ptr;
      }
    }
  }
  return best_move;
}

// move away form the robot given
static int move_away(position *rob)
{
  int dx = sign(player.x - rob->x);
  int dy = sign(player.y - rob->y);
  return move_towards(dx, dy);
}

//******************************************************************************

// move the closest heap between us and the closest robot
static int move_between(position *rob, position *hp)
{
  int dx;
  int dy;

  // equation of the line between us and the closest robot
  if (player.x == rob->x)
  {
    // me and the robot are aligned in x 
    // change my x so I get closer to the heap
    // and my y far from the robot
    dx = -sign(player.x - hp->x);
    dy = sign(player.y - rob->y);
  }
  else if (player.y == rob->y)
  {
    // me and the robot are aligned in y 
    // change my y so I get closer to the heap
    // and my x far from the robot
    dx = sign(player.x - rob->x);
    dy = -sign(player.y - hp->y);
  }
  else
  {
    float slope = (player.y - rob->y) / (player.x - rob->x);
    float cons = slope * rob->y;
    if (ABS(player.x - rob->x) > ABS(player.y - rob->y))
    {
      // we are closest to the robot in x 
      // move away from the robot in x and
      // close to the junk in y
      dx = sign(player.x - rob->x);
      dy = sign(((slope * ((float) hp->x)) + cons) - ((float) hp->y));
    }
    else
    {
      dx = sign(((slope * ((float) hp->x)) + cons) - ((float) hp->y));
      dy = sign(player.y - rob->y);
    }
  }

  return move_towards(dx, dy);
}

//******************************************************************************

// return true if the heap is between us and the robot
static int between(position *rob, position *hp)
{
  // I = @
  if (hp->x > rob->x && player.x < rob->x)
    return 0;
  // @ = I
  if (hp->x < rob->x && player.x > rob->x)
    return 0;
  // @
  // =
  // I
  if (hp->y < rob->y && player.y > rob->y)
    return 0;
  // I
  // =
  // @
  if (hp->y > rob->y && player.y < rob->y)
    return 0;
  return 1;
}

//******************************************************************************

// find and do the best move if flag else get the first move
static int automove() 
{
  int robot_dist;
  position *robot_close = closest_robot(&robot_dist);
  if (robot_dist > 1)
    return ROBOTS_KEY_X;
  if (!junks) 
    // no junk heaps just run away
    return move_away(robot_close);

  int heap_dist;
  position *heap_close = closest_junk(&heap_dist);
  int robot_heap = distance(robot_close->x, robot_close->y, 
  heap_close->x, heap_close->y);	

  if (robot_heap <= heap_dist && !between(robot_close, heap_close))
    // robot is closest to us from the heap. Run away!
    return move_away(robot_close);
	
  return move_between(robot_close, heap_close);
}

// <<<
//******************************************************************************
// >>> validity

static const char* validity(const char board[SCREEN_ROWS][SCREEN_COLS], int row, int col)
{
  static char valid[9];

  valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W] = valid[TO_SW]
    = valid[TO_S] = valid[TO_SE] = valid[TO_E] = valid[TO_NE] = 1;

  // borders
  if (row == 0)
    valid[TO_N] = valid[TO_NW] = valid[TO_NE] = 0;
  if (row == SCREEN_ROWS - 1)
    valid[TO_SW] = valid[TO_S] = valid[TO_SE] = 0;
  if (col == 0)
    valid[TO_NW] = valid[TO_W] = valid[TO_SW] = 0;
  if (col == SCREEN_COLS - 1)
    valid[TO_SE] = valid[TO_E] = valid[TO_NE] = 0;

  // 8 primary neighbors
  if (row > 0)
  {
    if (board[row - 1][col] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W]
	= valid[TO_E] = valid[TO_NE] = 0;
    else if (board[row - 1][col] == ROBOTS_JUNK)
      valid[TO_N] = 0;
  }

  if (row > 0 && col > 0)
  {
    if (board[row - 1][col - 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W] = 0;
    else if (board[row - 1][col - 1] == ROBOTS_JUNK)
      valid[TO_NW] = 0;
  }

  if (col > 0)
  {
    if (board[row][col - 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_NW] = valid[TO_W]
	= valid[TO_SW] = valid[TO_S] = 0;
    else if (board[row][col - 1] == ROBOTS_JUNK)
      valid[TO_W] = 0;
  }

  if (row < SCREEN_ROWS - 1 && col > 0)
  {
    if (board[row + 1][col - 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_W] = valid[TO_SW] = valid[TO_S] = 0;
    else if (board[row + 1][col - 1] == ROBOTS_JUNK)
      valid[TO_SW] = 0;
  }

  if (row < SCREEN_ROWS - 1)
  {
    if (board[row + 1][col] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_W] = valid[TO_SW] = valid[TO_S]
	= valid[TO_SE] = valid[TO_E] = 0;
    else if (board[row + 1][col] == ROBOTS_JUNK)
      valid[TO_S] = 0;
  }

  if (row < SCREEN_ROWS - 1 && col < SCREEN_COLS - 1)
  {
    if (board[row + 1][col + 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_S] = valid[TO_SE] = valid[TO_E] = 0;
    else if (board[row + 1][col + 1] == ROBOTS_JUNK)
      valid[TO_SE] = 0;
  }

  if (col < SCREEN_COLS - 1)
  {
    if (board[row][col + 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_S] = valid[TO_SE]
	= valid[TO_E] = valid[TO_NE] = 0;
    else if (board[row][col + 1] == ROBOTS_JUNK)
      valid[TO_E] = 0;
  }

  if (row > 0 && col < SCREEN_COLS - 1)
  {
    if (board[row - 1][col + 1] == ROBOTS_ROBOT)
      valid[TO_X] = valid[TO_N] = valid[TO_E] = valid[TO_NE] = 0;
    else if (board[row - 1][col + 1] == ROBOTS_JUNK)
      valid[TO_NE] = 0;
  }

  // 16 secondary neightbors
  if (row > 1
    && board[row - 2][col] == ROBOTS_ROBOT)
    valid[TO_N] = valid[TO_NW] = valid[TO_NE] = 0;

  if (row > 1 && col > 0
    && board[row - 2][col - 1] == ROBOTS_ROBOT)
    valid[TO_N] = valid[TO_NW] = 0;

  if (row > 1 && col > 1
    && board[row - 2][col - 2] == ROBOTS_ROBOT)
    valid[TO_NW] = 0;

  if (row > 0 && col > 1
    && board[row - 1][col - 2] == ROBOTS_ROBOT)
    valid[TO_NW] = valid[TO_W] = 0;

  if (col > 1
    && board[row][col - 2] == ROBOTS_ROBOT)
    valid[TO_NW] = valid[TO_W] = valid[TO_SW] = 0;

  if (row < SCREEN_ROWS - 1 && col > 1
    && board[row + 1][col - 2] == ROBOTS_ROBOT)
    valid[TO_W] = valid[TO_SW] = 0;

  if (row < SCREEN_ROWS - 2 && col > 1
    && board[row + 2][col - 2] == ROBOTS_ROBOT)
    valid[TO_SW] = 0;

  if (row < SCREEN_ROWS - 2 && col > 0
    && board[row + 2][col - 1] == ROBOTS_ROBOT)
    valid[TO_SW] = valid[TO_S] = 0;

  if (row < SCREEN_ROWS - 2
    && board[row + 2][col] == ROBOTS_ROBOT)
    valid[TO_SW] = valid[TO_S] = valid[TO_SE] = 0;

  if (row < SCREEN_ROWS - 2 && col < SCREEN_COLS - 1
    && board[row + 2][col + 1] == ROBOTS_ROBOT)
    valid[TO_S] = valid[TO_SE] = 0;

  if (row < SCREEN_ROWS - 2 && col < SCREEN_COLS - 2
    && board[row + 2][col + 2] == ROBOTS_ROBOT)
    valid[TO_SE] = 0;

  if (row < SCREEN_ROWS - 1 && col < SCREEN_COLS - 2
    && board[row + 1][col + 2] == ROBOTS_ROBOT)
    valid[TO_SE] = valid[TO_E] = 0;

  if (col < SCREEN_COLS - 2
    && board[row][col + 2] == ROBOTS_ROBOT)
    valid[TO_SE] = valid[TO_E] = valid[TO_NE] = 0;

  if (row > 0 && col < SCREEN_COLS - 2
    && board[row - 1][col + 2] == ROBOTS_ROBOT)
    valid[TO_E] = valid[TO_NE] = 0;

  if (row > 1 && col < SCREEN_COLS - 2
    && board[row - 2][col + 2] == ROBOTS_ROBOT)
    valid[TO_NE] = 0;

  if (row > 1 && col < SCREEN_COLS - 1
    && board[row - 2][col + 1] == ROBOTS_ROBOT)
    valid[TO_N] = valid[TO_NE] = 0;

  return valid;
}

// <<<
//******************************************************************************
// >>> strategy

int robots_strategy_bsd(const char board[SCREEN_ROWS][SCREEN_COLS])
{
  robots = 0;
  junks = 0;

  for (int row = 0; row < SCREEN_ROWS; row++)
    for (int col = 0; col < SCREEN_COLS; col++)
    {
      if (board[row][col] == ROBOTS_PLAYER)
      {
	player.x = col;
	player.y = row;
      }
      else if (board[row][col] == ROBOTS_ROBOT)
      {
	robot[robots].x = col;
	robot[robots].y = row;
	robots++;
      }
      else if (board[row][col] == ROBOTS_JUNK)
      {
	junk[junks].x = col;
	junk[junks].y = row;
	junks++;
      }
    }

  valid = validity(board, player.y, player.x);

  return automove();
}

// <<<
//******************************************************************************
