#ifndef __RENDER_H__
#define __RENDER_H__

#include "colors.h"
#include "pixel_art.h"

#define WIDTH 1400
#define HEIGHT 900

#define P_WIDTH 400
#define OFFSET 80

int fill_background(SDL_Renderer * renderer, SDL_Color color);

void draw_spectrum(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color selected);

void draw_selected_colors(SDL_Renderer * renderer, SDL_Rect * primary_r, SDL_Color primary, SDL_Rect * secondary_r, SDL_Color secondary);

void draw_selected_color_sat(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color color);

void draw_pixel_art(SDL_Renderer * renderer, const Pixel_Art * pixel_art);

#endif // __RENDER_H__