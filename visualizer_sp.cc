//******************************************************************************

#include "visualizer.h"
#include "graphix.h"
#include <unistd.h>

//******************************************************************************

enum Color
{
  black,
  grey,
  yellow,
  red,
  green,
  blue,
};

Visualizer::Visualizer()
  : delay(defdelay)
{
  int width = 2 * offset + hscale * (2 * Redcode::width - 1);
  int height = 2 * offset + vscale * (2 * Redcode::height - 1);

  std::vector<const char*> colors =
  {
    "black",
    "grey40",
    "yellow",
    "red2",
    "green3",
    "blue1",
  };

  graphix = new Graphix(width, height, colors);
}

Visualizer::~Visualizer()
{
  delete graphix;
}

void Visualizer::init()
{
  for (int row = 0; row < Redcode::height; row++)
    for (int col = 0; col < Redcode::width; col++)
    {
      int x = offset + 2 * hscale * col;
      int y = offset + 2 * vscale * row;
      graphix->rect(grey, x, y, hscale, vscale);
    }
  graphix->flush();
}

void Visualizer::set(int address, int uid)
{
  assert(address >= 0 && address < Redcode::size);
  assert(uid >= 0 && uid < Redcode::users);
  int row = address / Redcode::width;
  int col = address % Redcode::width;
  int x = offset + 2 * hscale * col;
  int y = offset + 2 * vscale * row;
  graphix->rect(red + uid, x, y, hscale, vscale);
}

void Visualizer::setexec(int address)
{
  assert(address >= 0 && address < Redcode::size);
  int row = address / Redcode::width;
  int col = address % Redcode::width;
  int x = offset + 2 * hscale * col;
  int y = offset + 2 * vscale * row;
  graphix->rect(yellow, x, y, hscale, vscale);
}

bool Visualizer::check()
{
  graphix->flush();
  while (int button = graphix->button())
  {
    switch (button)
    {
    case 1: // left
      if (wait())
	return true;
      break;

    case 3: // right
      return true;

    case 4: // up
      if (delay > mindelay)
	delay >>= 1;
      break;
      
    case 5: // down
      if (delay < maxdelay)
	delay <<= 1;
      break;

    case 8: // back
      delay = maxdelay;
      break;

    case 9: // forward
      delay = defdelay;
      break;
    }
  }
  if (delay)
  {
    usleep(delay);
    usleep(delay);
    usleep(delay);
    usleep(delay);
    usleep(delay);
  }
  return false;
}

bool Visualizer::wait()
{
  graphix->flush();
  for (;;)
  {
    int button = graphix->wait();
    if (button == 1) // left
      return false;
    if (button == 3) // right
      return true;
  }
}

//******************************************************************************
