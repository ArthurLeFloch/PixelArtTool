#ifndef __PIXELART_H__
#define __PIXELART_H__

#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct {
    const int width;
    const int height;
    const int x;
    const int y;
    const int tile_size;
    SDL_Rect rect;
    SDL_Color * image;
} Pixel_Art;

void fill_image(Pixel_Art * pixel_art, SDL_Color color);

void change_image_color(SDL_Point * pos, Pixel_Art * pixel_art, SDL_Color color);

SDL_Color get_color_at(SDL_Point * pos, const Pixel_Art * pixel_art);

void save(FILE *output, Pixel_Art * pixel_art);

#endif // __PIXELART_H__