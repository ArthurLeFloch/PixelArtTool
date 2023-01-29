#include "colors.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void min_and_max(SDL_Color color, int *min, int *max){
    int r = color.r/255.0, g = color.g, b = color.b;

    if(r <= g && r <= b)
        *min = r;
    else if(g <= r && g <= b)
        *min = g;
    else
        *min = b;
    
    if(r >= g && r >= b)
        *max = r;
    else if(g >= r && g >= b)
        *max = g;
    else
        *max = b;
}

SDL_Color hsv_to_rgb(HSV_Color hsv) {
    float h = hsv.h;
    float s = hsv.s;
    float v = hsv.v;
    int i;
    float f, p, q, t;
    int R, G, B;
    if(s == 0){
        R = G = B = (int)(v * 255);
        return (SDL_Color){R, G, B, 255};
    }
    h *= 6;
    i = floor(h);
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch(i) {
        case 0:
            R = (int)(v * 255);
            G = (int)(t * 255);
            B = (int)(p * 255);
            break;
        case 1:
            R = (int)(q * 255);
            G = (int)(v * 255);
            B = (int)(p * 255);
            break;
        case 2:
            R = (int)(p * 255);
            G = (int)(v * 255);
            B = (int)(t * 255);
            break;
        case 3:
            R = (int)(p * 255);
            G = (int)(q * 255);
            B = (int)(v * 255);
            break;
        case 4:
            R = (int)(t * 255);
            G = (int)(p * 255);
            B = (int)(v * 255);
            break;
        default:
            R = (int)(v * 255);
            G = (int)(p * 255);
            B = (int)(q * 255);
            break;
    }
    return (SDL_Color){R, G, B, 255};
}

HSV_Color rgb_to_hsv(SDL_Color color) {
    float r = color.r / 255.0;
    float g = color.g / 255.0;
    float b = color.b / 255.0;
    float cmax = fmax(r, fmax(g, b));
    float cmin = fmin(r, fmin(g, b));
    float delta = cmax - cmin;

    float H, S, V;
    V = cmax;
    if (cmax != 0) {
        S = delta / cmax;
    } else {
        S = 0;
        H = -1;
        return (HSV_Color){H, S, V};
    }
    if(r == cmax)
        H = (g - b) / delta;
    else if (g == cmax)
        H = 2 + (b - r) / delta;
    else
        H = 4 + (r - g) / delta;
    H *= 60;
    if(H < 0)
        H += 360;
    H /= 360;

    return (HSV_Color){H, S, V};
}

void print_hsv(HSV_Color color){
    printf("Hue : %f \tSaturation : %f \tValue : %f\n", color.h, color.s, color.v);
}

void print_rgb(SDL_Color color){
    printf("Red : %d \tGreen : %d \tBlue : %d\n", color.r, color.g, color.b);
}