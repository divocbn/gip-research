#include "gui.h"
#include "image.h"
#include <gip_gfx/gip_gfx.h>
#include <SDL.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

/**
 * yea this is wicked ik 
 */
#define MAX_FILENAME 256
#define MAX_FRAMES 1000

static gip_image_t* current_image = NULL;

// TODO: file browser or somethin like that?
static char current_dir[MAX_FILENAME] = "/Users/divo/Downloads/gip1-24-data-files"; // Default directory

void gip_viewer_render_gui()
{
    if (current_image == NULL) {
        current_image = gip_image_create();
        gip_image_set_scale(current_image, 0.6f);
    } 
    // Sidebar window
    ImGuiWindowFlags sidebar_flags = ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoCollapse;

    ImVec2 sidebar_pos = {1030, 0}; // (1280-250= 1030) isnt there a way to align it right?
    ImVec2 sidebar_size = {250, 670};

    if (igBegin("Image Info", NULL, sidebar_flags)) 
    {
        igSetWindowPos_Vec2(sidebar_pos, ImGuiCond_Always);
        igSetWindowSize_Vec2(sidebar_size, ImGuiCond_Always);

        if (current_image && current_image->texture) {
            igText("Headers:");
            igSeparator();
            igText("Width: %d", current_image->width);
            igText("Height: %d", current_image->height);
            igText("Color Depth: %d", current_image->color_depth);
            igText("Grayscale: %s", current_image->grayscale ? "Yes" : "No");
            igText("Header Size: %d", current_image->header_size);
            
            igSeparator();
            float scale = current_image->scale;
            if (igSliderFloat("Zoom", &scale, 0.1f, 2.0f, "%.2f", ImGuiSliderFlags_None)) {
                gip_image_set_scale(current_image, scale);
            }
        } else {
            igText("No image loaded");
        }
    }
    igEnd();
 
    // Footer window
    ImGuiWindowFlags footer_flags = ImGuiWindowFlags_NoTitleBar |
                                  ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_AlwaysAutoResize |
                                  ImGuiWindowFlags_NoSavedSettings;

    ImVec2 footer_pos = {5, 670};
    ImVec2 footer_size = {(float)1280, 50};

    if (igBegin("Footer", NULL, footer_flags))
    {
        igSetWindowPos_Vec2(footer_pos, ImGuiCond_Always);
        igSetWindowSize_Vec2(footer_size, ImGuiCond_Always);

        static bool playing = true;
        static int current_frame = 1;
        
        if (igButton(playing ? "Pause" : "Play", (ImVec2){50, 20}))
        {
            playing = !playing;
        }

        igSameLine(0, 10);
        igSliderInt("Frame", &current_frame, 1, MAX_FRAMES, "%d", ImGuiSliderFlags_None);

        char filename[MAX_FILENAME];
        snprintf(filename, MAX_FILENAME, "%s/EA1_6_%04d.data", current_dir, current_frame);
        
        // image isnt loading for some reason???
        // printf("loading frame: %s\n", filename);
        
        struct stat buffer;
        if (stat(filename, &buffer) == 0) {
            if (!gip_image_load(current_image, filename, gip_gfx_get_renderer())) {
                fprintf(stderr, "Failed to load image: %s\n", filename);
                return;
            }
            
            // set scale before rendering
            // gip_image_set_scale(current_image, 0.5f);

            // check if texture exists before rendering
            if (current_image && current_image->texture) {
                gip_image_render(current_image, gip_gfx_get_renderer());
            }
            
            if (playing) {
                current_frame = (current_frame % MAX_FRAMES) + 1;
            }
        }
    }
    igEnd();
}
