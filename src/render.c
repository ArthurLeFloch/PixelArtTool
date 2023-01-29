#include "render.h"

void draw_outline(SDL_Renderer * renderer, const SDL_Rect * rect){
    SDL_Rect rect1 = {rect->x - 4, rect->y - 4, rect->w + 8, rect->h + 8};
    SDL_Rect rect2 = {rect->x - 1, rect->y - 1, rect->w + 2, rect->h + 2};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect1);
    SDL_RenderDrawRect(renderer, &rect2);

    rect1 = (SDL_Rect){rect->x - 2, rect->y - 2, rect->w + 4, rect->h + 4};
    rect2 = (SDL_Rect){rect->x - 3, rect->y - 3, rect->w + 6, rect->h + 6};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect1);
    SDL_RenderDrawRect(renderer, &rect2);
}

int fill_background(SDL_Renderer * renderer, SDL_Color color){
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

void draw_spectrum(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color selected, float saturation){
    draw_outline(renderer, output);
    for(float v = 0.0f; v < 1.0; v += 1.0/output->h){
        for(float h = 0.0f; h < 1.0; h+=1.0/output->w){
            HSV_Color hsv = {h, saturation, v};
            SDL_Color color = hsv_to_rgb(hsv);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, output->x + roundf(output->w*h), output->y + roundf(200*(1-v)));
        }
    }
    SDL_Rect rect = {output->x + roundf(output->w*selected.h) - 10, output->y + roundf(200*(1-selected.v)) - 10, 20, 20};
    draw_outline(renderer, &rect);
}

void draw_selected_colors(SDL_Renderer * renderer, SDL_Rect * primary_r, SDL_Color primary, SDL_Rect * secondary_r, SDL_Color secondary){
    draw_outline(renderer, primary_r);
    SDL_SetRenderDrawColor(renderer, primary.r, primary.g, primary.b, primary.a);
    SDL_RenderFillRect(renderer, primary_r);

    draw_outline(renderer, secondary_r);
    SDL_SetRenderDrawColor(renderer, secondary.r, secondary.g, secondary.b, secondary.a);
    SDL_RenderFillRect(renderer, secondary_r);
}

void draw_selected_color_sat(SDL_Renderer * renderer, SDL_Rect * output, HSV_Color color){
    draw_outline(renderer, output);
    HSV_Color hsv;
    SDL_Color new_color;
    int high = output->y, low = output->y + output->h;
    for(float saturation = 0; saturation < 1.0f; saturation += 1.0f/output->w){
        int x = round(output->x + saturation * output->w);
        hsv = (HSV_Color){color.h, saturation, color.v};
        new_color = hsv_to_rgb(hsv);

        SDL_SetRenderDrawColor(renderer, new_color.r, new_color.g, new_color.b, new_color.a);
        SDL_RenderDrawLine(renderer, x, high, x, low);
    }
}

void draw_pixel_art(SDL_Renderer * renderer, const Pixel_Art * pixel_art){
    draw_outline(renderer, &pixel_art->rect);
    SDL_Color color;
    SDL_Rect rect;
    int tile_size = pixel_art->tile_size;
    for(int y = 0; y < pixel_art->height; y++){
        for(int x = 0; x < pixel_art->width; x++){
            color = pixel_art->image[x + y*pixel_art->width];
            rect = (SDL_Rect){pixel_art->x + x*tile_size, pixel_art->y + y*tile_size, tile_size, tile_size};

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}