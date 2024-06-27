//******************************************************************************

#include "graphix.h"

#include <X11/Xlib.h>
#include <stdlib.h>
#include <assert.h>

struct graphix
{
  int width;
  int height;
  int colors;
  Display *display;
  Window window;
  GC gc[1];
};

//******************************************************************************
// https://tronche.com/gui/x/xlib/events/processing-overview.html

graphix graphix_create(const char *title, int width, int height,
    int colors, const char *color[])
{
  assert(width > 0);
  assert(height > 0);
  assert(colors > 0);
  assert(colors < 256);

  graphix gx = malloc(sizeof(struct graphix) + (colors - 1) * sizeof(GC));
  assert(gx);
  gx->width = width;
  gx->height = height;
  gx->colors = colors;

  gx->display = XOpenDisplay(NULL);
  assert(gx->display);
  XSync(gx->display, False);
  int screen = DefaultScreen(gx->display);
  gx->window = XCreateSimpleWindow
  (
    gx->display, RootWindow(gx->display, screen),
    0, 0, // x, y
    (unsigned int)width, (unsigned int)height, // w, h
    1, // border
    BlackPixel(gx->display, screen), BlackPixel(gx->display, screen)
  );
  XStoreName(gx->display, gx->window, title);
  XSelectInput(gx->display, gx->window, ExposureMask | StructureNotifyMask);
  XMapWindow(gx->display, gx->window);
  XFlush(gx->display);

  for (int events = 0; events < 4;)
  {
    XEvent event;
    XNextEvent(gx->display, &event);
    if (event.type == Expose)
      events++;
    if (event.type == MapNotify)
      events++;
  }
  XSelectInput(gx->display, gx->window, ButtonPressMask);

  Colormap colormap = DefaultColormap(gx->display, screen);
  for (int i = 0; i < colors; i++)
  {
    XColor xcolor;
    Status status = XAllocNamedColor(gx->display, colormap, color[i], &xcolor, &xcolor);
    assert(status);
    XGCValues values;
    gx->gc[i] = XCreateGC(gx->display, gx->window, 0L, &values);
    assert(gx->gc[i]);
    XSetForeground(gx->display, gx->gc[i], xcolor.pixel);
    XSetBackground(gx->display, gx->gc[i],
      BlackPixel(gx->display, screen));
    XSetLineAttributes(gx->display, gx->gc[i], 1L, LineSolid, CapButt, JoinBevel);
    XSetFillStyle(gx->display, gx->gc[i], FillSolid);
  }
  return gx;
}

void graphix_free(graphix gx)
{
  assert(gx);
  XCloseDisplay(gx->display);
  free(gx);
}

int graphix_width(const graphix gx)
{
  assert(gx);
  return gx->width;
}

int graphix_height(const graphix gx)
{
  assert(gx);
  return gx->height;
}

//******************************************************************************

void graphix_point(graphix gx, int x, int y, int color)
{
  assert(gx);
  assert(x >= 0);
  assert(x < gx->width);
  assert(y >= 0);
  assert(y < gx->height);
  assert(color >= 0);
  assert(color < gx->colors);
  XDrawPoint(gx->display, gx->window, gx->gc[color], x, y);
}

void graphix_line(graphix gx, int x, int y, int xx, int yy, int color)
{
  assert(gx);
  assert(x >= 0);
  assert(x < gx->width);
  assert(y >= 0);
  assert(y < gx->height);
  assert(xx >= 0);
  assert(xx < gx->width);
  assert(yy >= 0);
  assert(yy < gx->height);
  assert(color >= 0);
  assert(color < gx->colors);
  XDrawLine(gx->display, gx->window, gx->gc[color], x, y, xx, yy);
}

void graphix_rect(graphix gx, int x, int y, int width, int height, int color)
{
  assert(gx);
  assert(x >= 0);
  assert(x < gx->width);
  assert(y >= 0);
  assert(y < gx->height);
  assert(width >= 0);
  assert(x + width <= gx->width);
  assert(height >= 0);
  assert(y + height <= gx->height);
  assert(color >= 0);
  assert(color < gx->colors);
  XFillRectangle(gx->display, gx->window, gx->gc[color],
      x, y, (unsigned int)width, (unsigned int)height);
}

void graphix_flush(graphix gx)
{
  assert(gx);
  XFlush(gx->display);
}

//******************************************************************************

int graphix_button(graphix gx)
{
  assert(gx);
  XEvent event;
  if (XCheckMaskEvent(gx->display, ButtonPressMask, &event))
    return ((XButtonEvent*)&event)->button;
  return 0;
}

int graphix_wait(graphix gx)
{
  assert(gx);
  XEvent event;
  XMaskEvent(gx->display, ButtonPressMask, &event);
  return ((XButtonEvent*)&event)->button;
}

//******************************************************************************
