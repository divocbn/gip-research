
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#define CIMGUI_USE_SDL2
#include <cimgui_impl.h>

#include <gip_gfx/gip_gfx.h>

#include "gui.h"

#include <SDL.h>

int main(int argc, char **argv)
{
  gip_gfx_create_window("micro-ct viewer", 1280, 720);

  SDL_Renderer *renderer = gip_gfx_get_renderer();
  SDL_Window *window = gip_gfx_get_window();

  igCreateContext(NULL);

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);
  igStyleColorsDark(NULL);

  gip_gfx_set_custom_event(ImGui_ImplSDL2_ProcessEvent);

  while (gip_gfx_running())
  {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    igNewFrame();

    gip_gfx_begin_draw();
    gip_viewer_render_gui();

    igRender();

    ImGui_ImplSDLRenderer2_RenderDrawData(igGetDrawData(), renderer);

    gip_gfx_end_draw();

    gip_gfx_handle_events();
    gip_gfx_render();
  }
}