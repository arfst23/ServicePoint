//******************************************************************************

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t sp_size;

#define SP_TILE_SIZE ((sp_size)8)
#define SP_TILES_HORIZ ((sp_size)56)
#define SP_TILES_VERT ((sp_size)20)
#define SP_DISPLAY_WIDTH (SP_TILES_HORIZ * SP_TILE_SIZE)
#define SP_DISPLAY_HEIGHT (SP_TILES_VERT * SP_TILE_SIZE)

typedef uint8_t sp_brightness;

#define SP_BRIGHTNESS_LEVELS (sp_brightness)12

//******************************************************************************

typedef struct sp_Connection* sp_display;

sp_display sp_display_create(void);
void sp_display_reset(const sp_display display);
void sp_display_clear(const sp_display display);
void sp_display_brightness(const sp_display display, sp_brightness brightness);
void sp_display_fade(const sp_display display);
void sp_display_free(sp_display display);

//******************************************************************************

typedef struct sp_BitVec* sp_bitvec;

sp_bitvec sp_bitvec_create(sp_size size);
sp_bitvec sp_bitvec_copy(const sp_bitvec bitvec);
void sp_bitvec_free(sp_bitvec bitvec);
sp_size sp_bitvec_size(const sp_bitvec bitvec);
void sp_bitvec_set(const sp_bitvec bitvec, sp_size i, bool value);
bool sp_bitvec_get(const sp_bitvec bitvec, sp_size i);
void sp_bitvec_fill(const sp_bitvec bitvec, bool value);

void sp_display_bitvec_set(const sp_display display, sp_bitvec bitvec, sp_size x, sp_size y);
void sp_display_bitvec_and(const sp_display display, sp_bitvec bitvec, sp_size x, sp_size y);
void sp_display_bitvec_or(const sp_display display, sp_bitvec bitvec, sp_size x, sp_size y);

//******************************************************************************

typedef struct sp_PixelGrid* sp_bitmap;

sp_bitmap sp_bitmap_create(sp_size width, sp_size height);
sp_bitmap sp_bitmap_copy(const sp_bitmap bitmap);
void sp_bitmap_free(sp_bitmap bitmap);
sp_size sp_bitmap_width(const sp_bitmap bitmap);
sp_size sp_bitmap_height(const sp_bitmap bitmap);
void sp_bitmap_set(const sp_bitmap bitmap, sp_size x, sp_size y, bool value);
bool sp_bitmap_get(const sp_bitmap bitmap, sp_size x, sp_size y);
void sp_bitmap_fill(sp_bitmap bitmap, bool value);

void sp_display_bitmap_set(const sp_display display, sp_bitmap bitmap, sp_size x, sp_size y);

//******************************************************************************

typedef struct sp_ByteGrid* sp_brightnessmap;

sp_brightnessmap sp_brightnessmap_create(sp_size width, sp_size height);
sp_brightnessmap sp_brightnessmap_copy(sp_brightnessmap brightnessmap);
void sp_brightnessmap_free(sp_brightnessmap brightnessmap);
sp_size sp_brightnessmap_width(sp_brightnessmap brightnessmap);
sp_size sp_brightnessmap_height(sp_brightnessmap brightnessmap);
void sp_brightnessmap_set(sp_brightnessmap brightnessmap,
    sp_size x, sp_size y, sp_brightness brightness);
sp_brightness sp_brightnessmap_get(sp_brightnessmap brightnessmap, sp_size x, sp_size y);
void sp_brightnessmap_fill(sp_brightnessmap brightnessmap, sp_brightness brightness);

void sp_display_brightnessmap_set(const sp_display display,
    sp_brightnessmap brightnessmap, sp_size x, sp_size y);

//******************************************************************************
