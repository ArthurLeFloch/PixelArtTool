#ifndef __UTILS_H__
#define __UTILS_H__

#include "colors.h"

HSV_Color get_palette_color(SDL_Point * pos, SDL_Rect * palette);

SDL_Color get_lum_color(SDL_Point * pos, SDL_Rect * lum, HSV_Color color);

#endif // __UTILS_H__