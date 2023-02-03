#ifndef __PIXELART_H__
#define __PIXELART_H__

#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct {
	int width;
	int height;
	int x;
	int y;
	int tile_size;
	SDL_Rect rect;
	SDL_Color *image;
} PixelArt;

void fill_image(PixelArt *pixel_art, SDL_Color color);

void change_image_color(SDL_Point *pos, PixelArt *pixel_art, SDL_Color color);

SDL_Color get_color_at(SDL_Point *pos, const PixelArt *pixel_art);

void save(FILE *output, PixelArt *pixel_art);

void fill_from_pos(PixelArt *pixel_art, SDL_Color new_color, SDL_Point pos);

void get_dimension(FILE *input, PixelArt *pixel_art);

void fill_from_file(FILE *input, PixelArt *pixel_art);

#endif // __PIXELART_H__