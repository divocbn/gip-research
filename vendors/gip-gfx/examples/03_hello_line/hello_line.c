// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023--2024 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023--2024 Visual Computing WH <vc@w-hs.de>

#include <gip_gfx/gip_gfx.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  if (gip_gfx_create_window("Hello Line", 800, 600) != 0) {
    printf("Error creating window.\n");
    return 1;
  }

  gip_gfx_begin_draw();

  gip_gfx_draw_line(100, 200, 120, 275, 113, 177, 39);

  gip_gfx_end_draw();

  gip_gfx_render_loop();

  return EXIT_SUCCESS;
}
