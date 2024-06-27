//******************************************************************************

#pragma once

#include "graphix.h"
#include <stdbool.h>

//******************************************************************************

graphix display_graphix_create(void);
void display_graphix_set(graphix gx, int x, int y, bool value);
void display_graphix_clear(graphix gx);

//******************************************************************************
