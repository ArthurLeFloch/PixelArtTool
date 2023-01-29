#include "utils.h"

HSV_Color get_palette_color(SDL_Point * pos, SDL_Rect * palette){
	int x = pos->x - palette->x;
	int y = pos->y - palette->y;

	float value = 0;
	float hue = x / (float)palette->w;
	float saturation = (1.0f - y / (float)palette->h);
	
	return (HSV_Color){hue, saturation, value};
}

SDL_Color get_lum_color(SDL_Point * pos, SDL_Rect * lum, HSV_Color color){
	int x = pos->x - lum->x;

	float luminosity = x / (float)lum->w;
	
	HSV_Color result = {color.h, color.s, luminosity};
	return hsv_to_rgb(result);
}