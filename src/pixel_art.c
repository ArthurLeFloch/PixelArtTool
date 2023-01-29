#include "pixel_art.h"

void fill_image(Pixel_Art * pixel_art, SDL_Color color){
    for(int y = 0; y < pixel_art->height; y++){
        for(int x = 0; x < pixel_art->width; x++){
            pixel_art->image[x + y*pixel_art->width] = color;
        }
    }
}

void change_image_color(SDL_Point * pos, Pixel_Art * pixel_art, SDL_Color color){
    int x = pos->x - pixel_art->rect.x;
    int y = pos->y - pixel_art->rect.y;

    int xc = x / pixel_art->tile_size;
    int yc = y / pixel_art->tile_size;

    pixel_art->image[xc + yc*pixel_art->width] = color;
}

SDL_Color get_color_at(SDL_Point * pos, const Pixel_Art * pixel_art){
    int x = pos->x - pixel_art->rect.x;
    int y = pos->y - pixel_art->rect.y;

    int xc = x / pixel_art->tile_size;
    int yc = y / pixel_art->tile_size;

    return pixel_art->image[xc + yc*pixel_art->width];
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