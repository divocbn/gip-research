#include "image.h"

gip_image_t* gip_image_create() {
    gip_image_t* img = malloc(sizeof(gip_image_t));
    img->texture = NULL;
    img->width = 1;
    img->height = 1; 
    img->scale = 0.6f;
    return img;
}

void gip_image_destroy(gip_image_t* img) {
    if (img->texture) {
        SDL_DestroyTexture(img->texture);
    }
    free(img);
}

int gip_image_load(gip_image_t* img, const char* filename, SDL_Renderer* renderer) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 0;
    }

    // Read dimensions
    fread(&img->width, 2, 1, file);
    fread(&img->height, 2, 1, file);
    fread(&img->color_depth, 2, 1, file);
    fread(&img->grayscale, 2, 1, file);
    fread(&img->header_size, 2, 1, file);

    if (img->width == 0 || img->height == 0) {
        fprintf(stderr, "Invalid image dimensions: %d x %d\n", img->width, img->height);
        fclose(file);
        return 0;
    }

    fseek(file, img->header_size, SEEK_SET);

    int pixel_count = img->width * img->height * (img->color_depth / 2);
    uint16_t* pixel_data = malloc(pixel_count);
    if (!pixel_data) {
        fprintf(stderr, "Failed to allocate memory for pixel data\n");
        fclose(file);
        return 0;
    }

    fread(pixel_data, sizeof(uint16_t), pixel_count, file);
    fclose(file);

    uint32_t* rgba_data = malloc(pixel_count * sizeof(uint32_t));
    if (!rgba_data) {
        fprintf(stderr, "Failed to allocate memory for RGBA data\n");
        free(rgba_data);
        return 0;
    }

    if (img->grayscale) {
        for (int i = 0; i < pixel_count; i++) {
            uint8_t gray = pixel_data[i] >> 8;
                rgba_data[i] = (gray << 24) | (gray << 16) | (gray << 8) | 0xFF;
            }
    } 
    
    free(pixel_data);

    if (img->texture) {
        SDL_DestroyTexture(img->texture);
    }
    
    // ^
    // uh maybe check if texture is already created?
    img->texture = SDL_CreateTexture(renderer,
                                   SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   img->width, img->height);

    if (!img->texture) {
        fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
        free(rgba_data);
        return 0;
    }

    SDL_UpdateTexture(img->texture, NULL, rgba_data, img->width * sizeof(uint32_t));
    free(rgba_data);

    return 1;
}

void gip_image_render(gip_image_t* img, SDL_Renderer* renderer) {
    if (!img->texture) return;

    int scaled_width = (int)(img->width * img->scale);
    int scaled_height = (int)(img->height * img->scale);
    SDL_Rect dest_rect = {0, 0, scaled_width, scaled_height};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, img->texture, NULL, &dest_rect);
}

void gip_image_set_scale(gip_image_t* img, float scale) {
    const float MIN_SCALE = 0.1f;
    const float MAX_SCALE = 2.0f;
    img->scale = fmaxf(MIN_SCALE, fminf(MAX_SCALE, scale));
}

int gip_image_get_scaled_width(gip_image_t* img) {
    return (int)(img->width * img->scale);
}

int gip_image_get_scaled_height(gip_image_t* img) {
    return (int)(img->height * img->scale);
}
