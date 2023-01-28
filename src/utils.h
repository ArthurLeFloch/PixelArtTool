#ifndef __UTILS_H__
#define __UTILS_H__

#include "palette.h"

#define WIDTH 1400
#define HEIGHT 900

#define P_WIDTH 400
#define OFFSET 80

typedef struct {
    const int width;
    const int height;
    const int x;
    const int y;
    const int tile_size;
    SDL_Rect rect;
    SDL_Color * image;
} Pixel_Art;

int fill_background(SDL_Renderer * renderer, SDL_Color color);

void draw_spectrum(SDL_Renderer * renderer, float luminosity);

void draw_selected_color(SDL_Renderer * renderer, SDL_Color color);

void draw_selected_color_sat(SDL_Renderer * renderer, HSV_Color color);

void draw_pixel_art(SDL_Renderer * renderer, const Pixel_Art * image);

void fill_image(Pixel_Art * image, SDL_Color color);

HSV_Color get_palette_color(SDL_Point * pos, SDL_Rect * palette);

SDL_Color get_sat_color(SDL_Point * pos, SDL_Rect * sat, HSV_Color color);

void change_image_color(SDL_Point * pos, Pixel_Art * image, SDL_Color color);

void save(FILE *output, Pixel_Art * pixel_art);

#endif // __UTILS_H__