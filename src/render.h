#ifndef __RENDER_H__
#define __RENDER_H__

#include "colors.h"

#define WIDTH 1400
#define HEIGHT 900

#define P_WIDTH 400
#define OFFSET 80

int fill_background(SDL_Renderer * renderer, SDL_Color color);

void draw_spectrum(SDL_Renderer * renderer, float saturation);

void draw_selected_colors(SDL_Renderer * renderer, SDL_Color color, SDL_Color color2);

void draw_selected_color_sat(SDL_Renderer * renderer, HSV_Color color);

#endif // __RENDER_H__