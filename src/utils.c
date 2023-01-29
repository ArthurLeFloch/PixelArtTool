#include "utils.h"

HSV_Color get_palette_color(SDL_Point * pos, SDL_Rect * palette){
    int x = pos->x - palette->x;
    int y = pos->y - palette->y;

    float saturation = 0;
    float hue = x / (float)palette->w;
    float value = (1.0f - y / (float)palette->h);
    
    return (HSV_Color){hue, saturation, value};
}

SDL_Color get_sat_color(SDL_Point * pos, SDL_Rect * sat, HSV_Color color){
    int x = pos->x - sat->x;

    float saturation = x / (float)sat->w;
    
    HSV_Color result = {color.h, saturation, color.v};
    return hsv_to_rgb(result);
}