// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2023--2024 Tom Vierjahn <tom.vierjahn@acm.org>
// SPDX-FileCopyrightText: 2023--2024 Visual Computing WH <vc@w-hs.de>

#include <gip_gfx/gip_gfx.h>
#include <stdio.h>
#include <stdlib.h>

void on_click(int x, int y, gip_gfx_mouse_button_t button) {
  if (button == GIP_GFX_MOUSE_BUTTON_LEFT) {
    printf("Left click ");
  } else if (button == GIP_GFX_MOUSE_BUTTON_RIGHT) {
    printf("Right click ");
  }
  printf("at (%d,%d).\n ", x, y);
}

int main() {
  if (gip_gfx_create_window("Hello Rectangle", 800, 600) != 0) {
    printf("Error creating window.\n");
    return 1;
  }

  gip_gfx_set_on_click_fn(on_click);

  while (gip_gfx_running()) {
    gip_gfx_handle_events();
    gip_gfx_render();
  }

  return EXIT_SUCCESS;
}
