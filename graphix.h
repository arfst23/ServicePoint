//******************************************************************************

#pragma once

struct graphix;
typedef struct graphix* graphix;

//******************************************************************************

graphix graphix_create(const char *title, int width, int height,
    int colors, const char *color[]);
void graphix_free(graphix gx);
int graphix_width(const graphix gx);
int graphix_height(const graphix gx);
void graphix_point(graphix gx, int x, int y, int color);
void graphix_line(graphix gx, int x, int y, int xx, int yy, int color);
void graphix_rect(graphix gx, int x, int y, int xx, int yy, int color);
void graphix_flush(graphix gx);
int graphix_button(graphix gx);
int graphix_wait(graphix gx);

//******************************************************************************
