// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023 Visual Computing WH <vc@w-hs.de>

#include <gip_gfx/gip_gfx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>

static SDL_Window* win = NULL;
static SDL_Renderer* ren = NULL;
static SDL_Texture* texture = NULL;
static SDL_Texture* draw_texture = NULL;
static SDL_Surface* window_surface = NULL;
static gip_gfx_image_t window_image = {
    .width = 0, .height = 0, .bytes_per_row = 0, .pixels = NULL};

typedef struct {
  SDL_Surface* sdl_surface;
  gip_gfx_image_t* gip_gfx_image;
} sdl_surface_gip_gfx_image_t;

static struct {
  sdl_surface_gip_gfx_image_t* data;
  size_t size;
} surface_image_map = {.data = NULL, .size = 0};

typedef struct {
    unsigned short width;
    unsigned short height;
    unsigned short color_depth;
    unsigned short grayscale;
    unsigned short header_size;
} image_header_t;

static bool gip_gfx_loop_running = true;

static void (*gip_gfx_on_click_fn)(int x, int y,
                                   gip_gfx_mouse_button_t button) = NULL;

static gip_gfx_on_custom_event_t gip_gfx_on_custom_event_fn = NULL;

SDL_Renderer* gip_gfx_get_renderer() { return ren; }
SDL_Window* gip_gfx_get_window() { return win; }

int gip_gfx_init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  return 0;
}

int gip_gfx_create_window(const char* title, int width, int height) {
  return gip_gfx_create_scaled_window(title, width, height, 1.0f);
}

int gip_gfx_create_scaled_window(const char* title, int width, int height,
                                 float scale) {
  if (SDL_WasInit(0) == 0) {
    gip_gfx_init();
  }

  win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         scale * width, scale * height, SDL_WINDOW_SHOWN  | SDL_WINDOW_RESIZABLE);
  if (win == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL) {
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24,
                              SDL_TEXTUREACCESS_STREAMING, width, height);
  if (texture == NULL) {
    printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
    return 1;
  }

  draw_texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_TARGET, width, height);
  if (draw_texture == NULL) {
    printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_SetTextureBlendMode(draw_texture, SDL_BLENDMODE_BLEND);

  return 0;
}

void gip_gfx_begin_draw() {
  SDL_SetRenderTarget(ren, draw_texture);
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
  SDL_RenderClear(ren);
}

void gip_gfx_end_draw() { SDL_SetRenderTarget(ren, NULL); }

void gip_gfx_draw_line(int start_x, int start_y, int end_x, int end_y,
                       uint8_t r, uint8_t g, uint8_t b) {
  SDL_SetRenderDrawColor(ren, r, g, b, 255);
  SDL_RenderDrawLine(ren, start_x, start_y, end_x, end_y);
}

void gip_gfx_set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  SDL_SetRenderDrawColor(ren, r, g, b, 255);
  SDL_RenderDrawPoint(ren, x, y);
}

  void gip_gfx_fill_rect(int start_x, int start_y, int width, int height,
                        uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_Rect rect = {.x = start_x, .y = start_y, .w = width, .h = height};
    SDL_RenderFillRect(ren, &rect);
  }

void gip_gfx_render_loop(void) {
  while (gip_gfx_loop_running) {
    gip_gfx_handle_events();
    gip_gfx_render();
  }

  SDL_Quit();
}

gip_gfx_image_t* gip_gfx_load_bmp(const char* filename) {
  // TODO: Handle error: out of memory
  ++surface_image_map.size;
  surface_image_map.data =
      realloc(surface_image_map.data,
              surface_image_map.size * sizeof(sdl_surface_gip_gfx_image_t));

  sdl_surface_gip_gfx_image_t* const pair =
      surface_image_map.data + (surface_image_map.size - 1);

  pair->sdl_surface = SDL_LoadBMP(filename);
  if (pair->sdl_surface == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    return NULL;
  }

  pair->gip_gfx_image = malloc(sizeof(gip_gfx_image_t));
  pair->gip_gfx_image->width = pair->sdl_surface->w;
  pair->gip_gfx_image->height = pair->sdl_surface->h;
  pair->gip_gfx_image->bytes_per_row = pair->sdl_surface->pitch;
  pair->gip_gfx_image->pixels = (uint8_t*)pair->sdl_surface->pixels;

  return pair->gip_gfx_image;
}

gip_gfx_image_t* gip_gfx_load_custom() {
    FILE* file = fopen("/Users/divo/Development/w-hs/gip-03/assets/EA1_6_1000.data", "rb");
    if (!file) {
        printf("Error opening file: %s\n", "/Users/divo/Development/w-hs/gip-03/assets/EA1_6_1000.data");
        return NULL;
    }

    image_header_t header;
    fseek(file, 0x800, SEEK_SET);
    fread(&header.width, sizeof(unsigned short), 1, file);
    fread(&header.height, sizeof(unsigned short), 1, file);
    fread(&header.color_depth, sizeof(unsigned short), 1, file);
    fread(&header.grayscale, sizeof(unsigned short), 1, file);
    fread(&header.header_size, sizeof(unsigned short), 1, file);

    ++surface_image_map.size;
    surface_image_map.data = realloc(surface_image_map.data,
                                     surface_image_map.size * sizeof(sdl_surface_gip_gfx_image_t));

    sdl_surface_gip_gfx_image_t* pair =
        surface_image_map.data + (surface_image_map.size - 1);

    int pitch = header.width * 2; // 2 bytes per pixel
    pair->sdl_surface = SDL_CreateRGBSurface(0, header.width, header.height, 16,
                                             0x0000F800, 0x000007E0, 0x0000001F, 0);
    if (pair->sdl_surface == NULL) {
        printf("SDL_CreateRGBSurface Error: %s\n", SDL_GetError());
        fclose(file);
        return NULL;
    }

    fseek(file, 0x800, SEEK_SET); // Move to the start of pixel data
    fread(pair->sdl_surface->pixels, 2, header.width * header.height, file);

    fclose(file);

    pair->gip_gfx_image = malloc(sizeof(gip_gfx_image_t));
    pair->gip_gfx_image->width = header.width;
    pair->gip_gfx_image->height = header.height;
    pair->gip_gfx_image->bytes_per_row = pitch;
    pair->gip_gfx_image->pixels = (uint8_t*)pair->sdl_surface->pixels;

    return pair->gip_gfx_image;
}

void gip_gfx_free_image(gip_gfx_image_t* image) {
  // TODO: remove pair from map
  for (size_t i = 0; i < surface_image_map.size; ++i) {
    if (surface_image_map.data[i].gip_gfx_image == image) {
      free(surface_image_map.data[i].gip_gfx_image);
      surface_image_map.data[i].gip_gfx_image = NULL;
      SDL_FreeSurface(surface_image_map.data[i].sdl_surface);
      surface_image_map.data[i].sdl_surface = NULL;
      break;
    }
  }
}

gip_gfx_image_t* gip_gfx_access_window_image() {
  SDL_LockTextureToSurface(texture, NULL, &window_surface);
  window_image.width = window_surface->w;
  window_image.height = window_surface->h;
  window_image.bytes_per_row = window_surface->pitch;
  window_image.pixels = window_surface->pixels;
  return &window_image;
}

void gip_gfx_update_window_image() { SDL_UnlockTexture(texture); }

SDL_Surface* _find_surface_from_image(gip_gfx_image_t* img) {
  // TODO: Find a better, harmonized way to handle surfaces, images
  if (img == &window_image) {
    return window_surface;
  }
  for (int i = 0; i < surface_image_map.size; ++i) {
    if (surface_image_map.data[i].gip_gfx_image == img) {
      return surface_image_map.data[i].sdl_surface;
    }
  }
  return NULL;
}

void gip_gfx_copy_rect(gip_gfx_image_t* src_img, gip_gfx_rect_t src_rect,
                       gip_gfx_image_t* dst_img, gip_gfx_pos2d_t dst_pos) {
  SDL_Surface* src = _find_surface_from_image(src_img);
  SDL_Surface* dst = _find_surface_from_image(dst_img);
  const SDL_Rect sdl_src_rect = {.x = src_rect.x,
                                 .y = src_rect.y,
                                 .w = src_rect.width,
                                 .h = src_rect.height};
  SDL_Rect sdl_dst_rect = {.x = dst_pos.x,
                           .y = dst_pos.y,
                           .w = src_rect.width,
                           .h = src_rect.height};
  SDL_BlitSurface(src, &sdl_src_rect, dst, &sdl_dst_rect);
}

bool gip_gfx_running() { return gip_gfx_loop_running; }

void gip_gfx_handle_events() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if(gip_gfx_on_custom_event_fn != NULL) {
      gip_gfx_on_custom_event_fn(&e);
    }

    // If user closes the window
    if (e.type == SDL_QUIT) {
      gip_gfx_loop_running = false;
    } else if (e.type == SDL_MOUSEBUTTONUP) {
      if (e.button.clicks == 1 && gip_gfx_on_click_fn != NULL) {
        gip_gfx_mouse_button_t button = GIP_GFX_MOUSE_BUTTON_LEFT;
        switch (e.button.button) {
          case SDL_BUTTON_RIGHT:
            button = GIP_GFX_MOUSE_BUTTON_RIGHT;
          case SDL_BUTTON_LEFT:
            (*gip_gfx_on_click_fn)(e.button.x, e.button.y, button);
          default:
            break;
        }
      }
    }
  }
}

void gip_gfx_render() {
  // render on screen
  // SDL_RenderClear(ren);
  SDL_RenderCopy(ren, texture, NULL, NULL);
  if (draw_texture != NULL) {
    SDL_RenderCopy(ren, draw_texture, NULL, NULL);
  }
  SDL_RenderPresent(ren);
}

void gip_gfx_set_on_click_fn(
    void (*on_click_fn)(int x, int y, gip_gfx_mouse_button_t button)) {
  gip_gfx_on_click_fn = on_click_fn;
}

void gip_gfx_set_custom_event(gip_gfx_on_custom_event_t event) {
  gip_gfx_on_custom_event_fn = event;
}

