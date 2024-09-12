//******************************************************************************

#define _SP_CONST
#include "servicepoint.h"
#include "sp.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define ADDRESS "172.23.42.29:2342"

#define SP_COMPRESSION_NONE (uint16_t)0
#define SP_COMPRESSION_ZLIB (uint16_t)26490
#define SP_COMPRESSION_BZIP2 (uint16_t)25210
#define SP_COMPRESSION_LZMA (uint16_t)27770
#define SP_COMPRESSION_ZSTD (uint16_t)31347
#define SP_COMPRESSION SP_COMPRESSION_LZMA

//******************************************************************************
// >>> display

sp_display sp_display_create(void)
{
  sp_display display = sp_connection_open(ADDRESS);
  assert(display);
  return display;
}

void sp_display_reset(const sp_display display)
{
  assert(display);
  struct SPCommand *command = sp_command_hard_reset();
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

void sp_display_clear(const sp_display display)
{
  assert(display);
  struct SPCommand *command = sp_command_clear();
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

void sp_display_brightness(const sp_display display, sp_brightness brightness)
{
  assert(display);
  assert(brightness < SP_BRIGHTNESS_LEVELS);
  struct SPCommand *command = sp_command_brightness((uint8_t)brightness);
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

void sp_display_fade(const sp_display display)
{
  assert(display);
  struct SPCommand *command = sp_command_fade_out();
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

void sp_display_free(sp_display display)
{
  assert(display);
  sp_connection_free(display);
}

// <<<
//******************************************************************************
// >>> bitvector

sp_bitvec sp_bitvec_create(sp_size size)
{
  assert(size < SP_DISPLAY_WIDTH);
  assert(size % SP_TILE_SIZE == 0);
  sp_bitvec bitvec = sp_bit_vec_new((size_t)size);
  assert(bitvec);
  return bitvec;
}

sp_bitvec sp_bitvec_copy(const sp_bitvec bitvec)
{
  assert(bitvec);
  sp_bitvec copy = sp_bit_vec_clone(bitvec);
  assert(copy);
  return copy;
}

void sp_bitvec_free(sp_bitvec bitvec)
{
  assert(bitvec);
  sp_bit_vec_free(bitvec);
}

sp_size sp_bitvec_size(const sp_bitvec bitvec)
{
  assert(bitvec);
  sp_size size = (sp_size)sp_bit_vec_len(bitvec);
  assert(size);
  return size;
}

void sp_bitvec_set(const sp_bitvec bitvec, sp_size i, bool value)
{
  assert(bitvec);
  assert(i < sp_bitvec_size(bitvec));
  sp_bit_vec_set(bitvec, (size_t)i, value);
}

bool sp_bitvec_get(const sp_bitvec bitvec, sp_size i)
{
  assert(bitvec);
  assert(i < sp_bitvec_size(bitvec));
  return sp_bit_vec_get(bitvec, i);
}

void sp_bitvec_fill(const sp_bitvec bitvec, bool value)
{
  assert(bitvec);
  sp_bit_vec_fill(bitvec, value);
}

//******************************************************************************

void sp_display_bitvec_set(const sp_display display, sp_bitvec bitvec,
    sp_size x, sp_size y)
{
  assert(display);
  assert(bitvec);
  assert(x % SP_TILE_SIZE == 0);
  assert(x + sp_bitvec_size(bitvec) <= SP_DISPLAY_WIDTH);
  assert(y < SP_DISPLAY_HEIGHT);
  sp_size offset = x / SP_TILE_SIZE + y * SP_TILES_VERT;
  struct SPCommand *command = sp_command_bitmap_linear(offset, bitvec, SP_COMPRESSION);
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

void sp_display_bitvec_and(const sp_display display, sp_bitvec bitvec,
    sp_size x, sp_size y)
{
  assert(display);
  assert(bitvec);
  assert(x % SP_TILE_SIZE == 0);
  assert(x + sp_bitvec_size(bitvec) <= SP_DISPLAY_WIDTH);
  assert(y < SP_DISPLAY_HEIGHT);
  sp_size offset = x / SP_TILE_SIZE + y * SP_TILES_VERT;
  struct SPCommand *command = sp_command_bitmap_linear_and(offset, bitvec, SP_COMPRESSION);
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

void sp_display_bitvec_or(const sp_display display, sp_bitvec bitvec,
    sp_size x, sp_size y)
{
  assert(display);
  assert(bitvec);
  assert(x % SP_TILE_SIZE == 0);
  assert(x + sp_bitvec_size(bitvec) <= SP_DISPLAY_WIDTH);
  assert(y < SP_DISPLAY_HEIGHT);
  sp_size offset = x / SP_TILE_SIZE + y * SP_TILES_VERT;
  struct SPCommand *command = sp_command_bitmap_linear_or(offset, bitvec, SP_COMPRESSION);
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

// <<<
//******************************************************************************
// >>> bitmap

sp_bitmap sp_bitmap_create(sp_size width, sp_size height)
{
  assert(width <= SP_DISPLAY_WIDTH);
  assert(width % SP_TILE_SIZE == 0);
  assert(height <= SP_DISPLAY_HEIGHT);
  sp_bitmap bitmap = sp_pixel_grid_new((size_t)width, (size_t)height);
  assert(bitmap);
  return bitmap;
}

sp_bitmap sp_bitmap_copy(const sp_bitmap bitmap)
{
  assert(bitmap);
  sp_bitmap copy = sp_pixel_grid_clone(bitmap);
  assert(copy);
  return copy;
}

void sp_bitmap_free(sp_bitmap bitmap)
{
  assert(bitmap);
  sp_pixel_grid_free(bitmap);
}

sp_size sp_bitmap_width(const sp_bitmap bitmap)
{
  assert(bitmap);
  return (sp_size)sp_pixel_grid_width(bitmap);
}

sp_size sp_bitmap_height(const sp_bitmap bitmap)
{
  assert(bitmap);
  return (sp_size)sp_pixel_grid_height(bitmap);
}

void sp_bitmap_set(const sp_bitmap bitmap, sp_size x, sp_size y, bool value)
{
  assert(bitmap);
  assert(x < sp_bitmap_width(bitmap));
  assert(y < sp_bitmap_height(bitmap));
  sp_pixel_grid_set(bitmap, (size_t)x, (size_t)y, value);
}

bool sp_bitmap_get(const sp_bitmap bitmap, sp_size x, sp_size y)
{
  assert(bitmap);
  assert(x < sp_bitmap_width(bitmap));
  assert(y < sp_bitmap_height(bitmap));
  return sp_pixel_grid_get(bitmap, (size_t)x, (size_t)y);
}

void sp_bitmap_fill(sp_bitmap bitmap, bool value)
{
  assert(bitmap);
  sp_pixel_grid_fill(bitmap, value);
}

//******************************************************************************

void sp_display_bitmap_set(const sp_display display, sp_bitmap bitmap,
    sp_size x, sp_size y)
{
  assert(display);
  assert(bitmap);
  assert(x % SP_TILE_SIZE == 0);
  assert(x + sp_bitmap_width(bitmap) <= SP_DISPLAY_WIDTH);
  assert(y + sp_bitmap_height(bitmap) <= SP_DISPLAY_HEIGHT);
  struct SPCommand *command = sp_command_bitmap_linear_win((size_t)x,
      (size_t)y, bitmap, SP_COMPRESSION);
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

// <<<
//******************************************************************************
// >>> brightnessmap

sp_brightnessmap sp_brightnessmap_create(sp_size width, sp_size height)
{
  sp_brightnessmap brightnessmap = sp_brightness_grid_new((size_t)width, (size_t)height);
  assert(brightnessmap);
  return brightnessmap;
}

sp_brightnessmap sp_brightnessmap_copy(sp_brightnessmap brightnessmap)
{
  assert(brightnessmap);
  sp_brightnessmap copy = sp_brightness_grid_clone(brightnessmap);
  assert(copy);
  return copy;
}

void sp_brightnessmap_free(sp_brightnessmap brightnessmap)
{
  assert(brightnessmap);
  sp_brightness_grid_free(brightnessmap);
}

sp_size sp_brightnessmap_width(sp_brightnessmap brightnessmap)
{
  assert(brightnessmap);
  return (sp_size)sp_brightness_grid_height(brightnessmap);
}

sp_size sp_brightnessmap_height(sp_brightnessmap brightnessmap)
{
  assert(brightnessmap);
  return (sp_size)sp_brightness_grid_width(brightnessmap);
}

void sp_brightnessmap_set(sp_brightnessmap brightnessmap, sp_size x, sp_size y,
    sp_brightness brightness)
{
  assert(brightnessmap);
  assert(x < sp_brightnessmap_width(brightnessmap));
  assert(y < sp_brightnessmap_height(brightnessmap));
  assert(brightness < SP_BRIGHTNESS_LEVELS);
  sp_brightness_grid_set(brightnessmap, (size_t)x, (size_t)y, (uint8_t)brightness);
}

sp_brightness sp_brightnessmap_get(sp_brightnessmap brightnessmap, sp_size x, sp_size y)
{
  assert(brightnessmap);
  assert(x < sp_brightnessmap_width(brightnessmap));
  assert(y < sp_brightnessmap_height(brightnessmap));
  return (sp_brightness)sp_brightness_grid_get(brightnessmap, (size_t)x, (size_t)y);
}

void sp_brightnessmap_fill(sp_brightnessmap brightnessmap, sp_brightness brightness)
{
  assert(brightnessmap);
  assert(brightness < SP_BRIGHTNESS_LEVELS);
  sp_brightness_grid_fill(brightnessmap, (uint8_t)brightness);
}

//******************************************************************************

void sp_display_brightnessmap_set(const sp_display display,
    sp_brightnessmap brightnessmap, sp_size x, sp_size y)
{
  assert(display);
  assert(brightnessmap);
  assert(x + sp_brightnessmap_width(brightnessmap) < SP_TILES_HORIZ);
  assert(y + sp_brightnessmap_height(brightnessmap) < SP_TILES_VERT);
  struct SPCommand *command = sp_command_char_brightness(x, y, brightnessmap);
  assert(command);
  bool sent = sp_connection_send_command(display, command);
  assert(sent);
}

// <<<
//******************************************************************************
