#ifndef __PALETTE_H__
#define __PALETTE_H_

#include <SDL2/SDL.h>

typedef struct{
    float h;
    float s;
    float v;
} HSV_Color;

HSV_Color rgb_to_hsv(SDL_Color color);

SDL_Color hsv_to_rgb(HSV_Color color);

void print_hsv(HSV_Color color);

void print_rgb(SDL_Color color);

#endif // __PALETTE_H__