// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023--2024 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023--2024 Visual Computing WH <vc@w-hs.de>

#include <stdio.h>
#include <stdlib.h>

#include <gip_gfx/gip_gfx.h>

int main()
{
  if (gip_gfx_create_window("Hello Pixel", 800, 600) != 0)
  {
    printf("Error creating window.\n");
    return 1;
  }

  gip_gfx_begin_draw();

  int punktVektor = 100;
  for (int x = 0; x <= 100; x++)
      gip_gfx_set_pixel(punktVektor, punktVektor + x, 255, 0, 0);

  gip_gfx_end_draw();

  gip_gfx_render_loop();

  return EXIT_SUCCESS;
}
