#ifndef GIP_IMAGE_H
#define GIP_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <gip_gfx/gip_gfx.h>

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
    float scale;
    uint16_t color_depth;
    uint16_t grayscale;
    uint16_t header_size;
} gip_image_t;

gip_image_t* gip_image_create();
void gip_image_destroy(gip_image_t* img);
int gip_image_load(gip_image_t* img, const char* filename, SDL_Renderer* renderer);
void gip_image_render(gip_image_t* img, SDL_Renderer* renderer);
void gip_image_set_scale(gip_image_t* img, float scale);
int gip_image_get_scaled_width(gip_image_t* img);
int gip_image_get_scaled_height(gip_image_t* img);

#endif /* GIP_IMAGE_H */
