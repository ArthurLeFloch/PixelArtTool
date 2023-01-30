#ifndef __RENDER_H__
#define __RENDER_H__

#include "colors.h"
#include "pixel_art.h"

#define WIDTH 1400
#define HEIGHT 900

#define P_WIDTH 400
#define OFFSET 80

enum tool{
    PEN = 0,
    BUCKET,
    PIPETTE
};

int fill_background(SDL_Renderer * renderer, SDL_Color color);

void draw_spectrum(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color selected);

void draw_selected_colors(SDL_Renderer * renderer, SDL_Rect * primary_r, SDL_Color primary, SDL_Rect * secondary_r, SDL_Color secondary);

void draw_selected_color_sat(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color color);

void draw_pixel_art(SDL_Renderer * renderer, const Pixel_Art * pixel_art);

void load_assets(SDL_Renderer * renderer);

void render_icons(SDL_Renderer * renderer, enum tool tool);

void free_assets();

void set_selected_tool(SDL_Point * pos, enum tool * tool);

#endif // __RENDER_H__