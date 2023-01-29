#include "render.h"

int fill_background(SDL_Renderer * renderer, SDL_Color color){
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

void draw_spectrum(SDL_Renderer * renderer, HSV_Color selected, float saturation){
    for(float v = 0.0f; v < 1.0; v += 1.0/200.0f){
        for(float h = 0.0f; h < 1.0; h+=1.0/P_WIDTH){
            HSV_Color hsv = {h, saturation, v};
            SDL_Color color = hsv_to_rgb(hsv);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, (WIDTH-P_WIDTH) + roundf(P_WIDTH*h), (HEIGHT-200) + roundf(200*(1-v)));
        }
    }
    SDL_Rect rect = {(WIDTH-P_WIDTH) + roundf(P_WIDTH*selected.h) - 9, (HEIGHT-200) + roundf(200*(1-selected.v)) - 9, 18, 18};
    SDL_Rect rect2 = {(WIDTH-P_WIDTH) + roundf(P_WIDTH*selected.h) - 8, (HEIGHT-200) + roundf(200*(1-selected.v)) - 8, 16, 16};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect2);

    rect = (SDL_Rect){(WIDTH-P_WIDTH) + roundf(P_WIDTH*selected.h) - 10, (HEIGHT-200) + roundf(200*(1-selected.v)) - 10, 20, 20};
    rect2 = (SDL_Rect){(WIDTH-P_WIDTH) + roundf(P_WIDTH*selected.h) - 7, (HEIGHT-200) + roundf(200*(1-selected.v)) - 7, 14, 14};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect2);
}

void draw_selected_colors(SDL_Renderer * renderer, SDL_Color color, SDL_Color color2){
    SDL_Rect rect = {WIDTH - P_WIDTH, HEIGHT - 200 - 60 - 60, P_WIDTH/2 - 10, 40};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    rect = (SDL_Rect){WIDTH - P_WIDTH/2 + 10, HEIGHT - 200 - 60 - 60, P_WIDTH/2 - 10, 40};
    SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_selected_color_sat(SDL_Renderer * renderer, HSV_Color color){
    HSV_Color hsv;
    SDL_Color new_color;
    int high = (HEIGHT - 200 - 60), low = (HEIGHT - 200 - 20);
    for(float saturation = 0; saturation < 1.0f; saturation += 1.0f/P_WIDTH){
        int x = round((WIDTH - P_WIDTH) + saturation * P_WIDTH);
        hsv = (HSV_Color){color.h, saturation, color.v};
        new_color = hsv_to_rgb(hsv);

        SDL_SetRenderDrawColor(renderer, new_color.r, new_color.g, new_color.b, new_color.a);
        SDL_RenderDrawLine(renderer, x, high, x, low);
    }
}