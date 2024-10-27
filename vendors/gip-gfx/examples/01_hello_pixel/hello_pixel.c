// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023--2024 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023--2024 Visual Computing WH <vc@w-hs.de>

#include <stdio.h>
#include <stdlib.h>

#include <gip_gfx/gip_gfx.h>

int main() {
  if (gip_gfx_create_window("Hello Pixel", 800, 600) != 0) {
    printf("Error creating window.\n");
    return 1;
  }

  gip_gfx_begin_draw();

  gip_gfx_set_pixel(100, 200, 255, 0, 0);
  gip_gfx_set_pixel(101, 200, 255, 0, 0);
  gip_gfx_set_pixel(100, 201, 255, 0, 0);
  gip_gfx_set_pixel(101, 201, 255, 0, 0);

  gip_gfx_set_pixel(102, 200, 0, 255, 0);
  gip_gfx_set_pixel(103, 200, 0, 255, 0);
  gip_gfx_set_pixel(102, 201, 0, 255, 0);
  gip_gfx_set_pixel(103, 201, 0, 255, 0);

  gip_gfx_set_pixel(100, 202, 0, 0, 255);
  gip_gfx_set_pixel(101, 202, 0, 0, 255);
  gip_gfx_set_pixel(100, 203, 0, 0, 255);
  gip_gfx_set_pixel(101, 203, 0, 0, 255);

  gip_gfx_set_pixel(102, 202, 255, 255, 255);
  gip_gfx_set_pixel(103, 202, 255, 255, 255);
  gip_gfx_set_pixel(102, 203, 255, 255, 255);
  gip_gfx_set_pixel(103, 203, 255, 255, 255);

  gip_gfx_end_draw();

  gip_gfx_render_loop();

  return EXIT_SUCCESS;
}
