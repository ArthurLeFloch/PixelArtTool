#ifndef __UTILS_H__
#define __UTILS_H__

#include "palette.h"

#ifndef IM_WIDTH
    #define IM_WIDTH 32
#endif

#ifndef IM_HEIGHT
    #define IM_HEIGHT 32
#endif

#define WIDTH 1400
#define HEIGHT 900

#define P_WIDTH 400
#define OFFSET 80

typedef struct {
    int width, height;
} Dimension;

int fill_background(SDL_Renderer * renderer, SDL_Color color);

void draw_spectrum(SDL_Renderer * renderer, float luminosity);

void draw_selected_color(SDL_Renderer * renderer, SDL_Color color);

void draw_selected_color_sat(SDL_Renderer * renderer, HSV_Color color);

void draw_pixel_art(SDL_Renderer * renderer, const SDL_Color image[IM_HEIGHT][IM_WIDTH], int tile_size, int x_offset, int y_offset);

void fill_image(SDL_Color image[IM_HEIGHT][IM_WIDTH], SDL_Color color);

HSV_Color get_palette_color(SDL_Point * pos, SDL_Rect * palette);

SDL_Color get_sat_color(SDL_Point * pos, SDL_Rect * sat, HSV_Color color);

void change_image_color(SDL_Point * pos, SDL_Rect * image_rect, SDL_Color image[IM_HEIGHT][IM_WIDTH], int tile_size, SDL_Color color);

#endif // __UTILS_H__