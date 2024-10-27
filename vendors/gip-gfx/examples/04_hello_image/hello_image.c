// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023--2024 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023--2024 Visual Computing WH <vc@w-hs.de>

#include <gip_gfx/gip_gfx.h>
#include <stdio.h>
#include <stdlib.h>

void print_usage() {
  printf("gip_gfx image drawing example\n\n");
  printf("  usage: hello_image IMG\n\n");
  printf("  IMG  Path to the image file to be used.\n");
  printf("       It can be found in the img subdirectory of this example:\n");
  printf("         GIP_GFX_SRC/examples/04_hello_image/img/image.bmp\n");
  printf("         or\n");
  printf(
      "         "
      "GIP_GFX_INST/share/gip_gfx/examples/04_hello_image/img/image.bmp\n");
  printf("       with\n");
  printf("         GIP_GFX denoting the source directory and \n");
  printf("         GIP_GFX_INST the install directory\n");
  printf("       of gip_gfx.\n\n");
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    print_usage();
    return EXIT_FAILURE;
  }

  if (gip_gfx_create_window("Hello Image", 800, 600) != 0) {
    printf("Error creating window.\n");
    return EXIT_FAILURE;
  }

  gip_gfx_image_t* img = gip_gfx_load_bmp(argv[1]);
  if (img == NULL) {
    printf("Error loading image.\n");
    return EXIT_FAILURE;
  }

  gip_gfx_image_t* window_image = gip_gfx_access_window_image();

  gip_gfx_rect_t src_rect = {.x = 64, .y = 0, .width = 64, .height = 64};
  gip_gfx_pos2d_t dst_pos = {.x = 368, .y = 268};
  gip_gfx_copy_rect(img, src_rect, window_image, dst_pos);

  gip_gfx_update_window_image();

  gip_gfx_render_loop();

  gip_gfx_free_image(img);

  return EXIT_SUCCESS;
}
