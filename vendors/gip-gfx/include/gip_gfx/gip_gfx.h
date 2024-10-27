// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023 Visual Computing WH <vc@w-hs.de>

#ifndef SDL_WARMING_STRIPES_GIP_GFX_H_
#define SDL_WARMING_STRIPES_GIP_GFX_H_

#include <stdbool.h>
#include <stdint.h>

#include <SDL_render.h>

typedef struct {
  int width;
  int height;
  int bytes_per_row;
  uint8_t* pixels;
} gip_gfx_image_t;

typedef enum {
  GIP_GFX_MOUSE_BUTTON_LEFT,
  GIP_GFX_MOUSE_BUTTON_RIGHT
} gip_gfx_mouse_button_t;

typedef struct {
  int x;
  int y;
} gip_gfx_pos2d_t;

typedef struct {
  int x;
  int y;
  int width;
  int height;
} gip_gfx_rect_t;

int gip_gfx_init();
int gip_gfx_create_window(const char* title, int width, int height);
int gip_gfx_create_scaled_window(const char* title, int width, int height,
                                 float scale);

void gip_gfx_set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void gip_gfx_draw_line(int start_x, int start_y, int end_x, int end_y,
                       uint8_t r, uint8_t g, uint8_t b);
void gip_gfx_fill_rect(int start_x, int start_y, int width, int height,
                       uint8_t r, uint8_t g, uint8_t b);

void gip_gfx_begin_draw();
void gip_gfx_end_draw();

void gip_gfx_render_loop(void);

gip_gfx_image_t* gip_gfx_load_bmp(const char* filename);
void gip_gfx_free_image(gip_gfx_image_t* image);

gip_gfx_image_t* gip_gfx_access_window_image();
void gip_gfx_update_window_image();
void gip_gfx_copy_rect(gip_gfx_image_t* src_img, gip_gfx_rect_t src_rect,
                       gip_gfx_image_t* dst_img, gip_gfx_pos2d_t dst_pos);

void gip_gfx_set_on_click_fn(
    void (*on_click_fn)(int x, int y, gip_gfx_mouse_button_t button));
bool gip_gfx_running();
void gip_gfx_handle_events();
void gip_gfx_render();

SDL_Renderer* gip_gfx_get_renderer();

#endif  // SDL_WARMING_STRIPES_GIP_GFX_H_
