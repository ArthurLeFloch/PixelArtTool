#include "utils.h"

int fill_background(SDL_Renderer * renderer, SDL_Color color){
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

void draw_spectrum(SDL_Renderer * renderer, float saturation){
    for(float v = 0.0f; v < 1.0; v += 1.0/200.0f){
        for(float h = 0.0f; h < 1.0; h+=1.0/P_WIDTH){
            HSV_Color hsv = {h, saturation, v};
            SDL_Color color = hsv_to_rgb(hsv);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, (WIDTH-P_WIDTH) + roundf(P_WIDTH*h), (HEIGHT-200) + roundf(200*(1-v)));
        }
    }
}

void draw_selected_color(SDL_Renderer * renderer, SDL_Color color){
    SDL_Rect rect = {WIDTH-P_WIDTH, HEIGHT - 200 - 60 - 60, P_WIDTH, 40};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_selected_color_sat(SDL_Renderer * renderer, HSV_Color color){
    HSV_Color hsv;
    SDL_Color new_color;
    int x;
    for(float saturation = 0; saturation < 1.0f; saturation += 1.0f/P_WIDTH){
        x = round((WIDTH - P_WIDTH) + saturation * P_WIDTH);
        hsv = (HSV_Color){color.h, saturation, color.v};
        new_color = hsv_to_rgb(hsv);

        SDL_SetRenderDrawColor(renderer, new_color.r, new_color.g, new_color.b, new_color.a);
        SDL_RenderDrawLine(renderer, x, (HEIGHT - 200 - 20), x, (HEIGHT - 200 - 60));
    }
}

void draw_pixel_art(SDL_Renderer * renderer, const SDL_Color image[IM_HEIGHT][IM_WIDTH], int tile_size, int x_offset, int y_offset){
    SDL_Color color;
    SDL_Rect rect;
    for(int y = 0; y < IM_HEIGHT; y++){
        for(int x = 0; x < IM_WIDTH; x++){
            color = image[y][x];
            rect = (SDL_Rect){x_offset + x*tile_size, y_offset + y*tile_size, tile_size, tile_size};

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void fill_image(SDL_Color image[IM_HEIGHT][IM_WIDTH], SDL_Color color){
    for(int y = 0; y < IM_HEIGHT; y++){
        for(int x = 0; x < IM_WIDTH; x++){
            image[y][x] = color;
        }
    }
}

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

void change_image_color(SDL_Point * pos, SDL_Rect * image_rect, SDL_Color image[IM_HEIGHT][IM_WIDTH], int tile_size, SDL_Color color){
    int x = pos->x - image_rect->x;
    int y = pos->y - image_rect->y;

    int xc = x / tile_size;
    int yc = y / tile_size;

    image[yc][xc] = color;
}