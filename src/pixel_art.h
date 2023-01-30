#ifndef __PIXELART_H__
#define __PIXELART_H__

#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct {
	int width;
	int height;
	int x;
	int y;
	int tile_size;
	SDL_Rect rect;
	SDL_Color * image;
} Pixel_Art;

void fill_image(Pixel_Art * pixel_art, SDL_Color color);

void change_image_color(SDL_Point * pos, Pixel_Art * pixel_art, SDL_Color color);

SDL_Color get_color_at(SDL_Point * pos, const Pixel_Art * pixel_art);

void save(FILE *output, Pixel_Art * pixel_art);

void fill_from_pos(Pixel_Art * pixel_art, SDL_Color new_color, SDL_Point pos);

void get_dimension(FILE *input, Pixel_Art * pixel_art);

void fill_from_file(FILE *input, Pixel_Art * pixel_art);

#endif // __PIXELART_H__