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

void draw_pixel_art(SDL_Renderer * renderer, const Pixel_Art * pixel_art){
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

void fill_image(Pixel_Art * pixel_art, SDL_Color color){
    for(int y = 0; y < pixel_art->height; y++){
        for(int x = 0; x < pixel_art->width; x++){
            pixel_art->image[x + y*pixel_art->width] = color;
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

void change_image_color(SDL_Point * pos, Pixel_Art * pixel_art, SDL_Color color){
    int x = pos->x - pixel_art->rect.x;
    int y = pos->y - pixel_art->rect.y;

    int xc = x / pixel_art->tile_size;
    int yc = y / pixel_art->tile_size;

    pixel_art->image[xc + yc*pixel_art->width] = color;
}

void save(FILE *output, Pixel_Art * pixel_art){
    SDL_Color color;
    fprintf(output, "P6\n%i %i 255\n", pixel_art->width, pixel_art->height);
    for(int y=0; y<pixel_art->height; y++){
        for(int x=0; x<pixel_art->width; x++){
            color = pixel_art->image[x + y*pixel_art->width];
            fputc(color.r, output);
            fputc(color.g, output);
            fputc(color.b, output);
        }
    }
}