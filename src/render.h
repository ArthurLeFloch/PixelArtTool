#ifndef __RENDER_H__
#define __RENDER_H__

#include "colors.h"
#include "pixel_art.h"

#ifndef WIDTH
    #define WIDTH 1400
#endif
#ifndef HEIGHT
    #define HEIGHT 900
#endif

#define P_WIDTH 400
#define OFFSET 80

enum tool{
    PEN = 0,
    BUCKET,
    PIPETTE,
    NONE,
};

int fill_background(SDL_Renderer * renderer, SDL_Color color);

void draw_spectrum(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color selected);

void draw_selected_colors(SDL_Renderer * renderer, SDL_Rect * primary_r, SDL_Color primary, SDL_Rect * secondary_r, SDL_Color secondary);

void draw_selected_color_sat(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color color);

void draw_pixel_art(SDL_Renderer * renderer, const PixelArt * pixel_art);

void load_assets(SDL_Renderer * renderer);

void free_assets();

int update_buttons(SDL_Renderer * renderer, SDL_Point * pos, enum tool * tool, int click, int clicked);

#endif // __RENDER_H__