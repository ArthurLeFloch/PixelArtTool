#include "pixel_art.h"
#include "colors.h"

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

int is_point_inside(const Pixel_Art * pixel_art, SDL_Point pos){
	return (pos.x >= 0 && pos.x < pixel_art->width) && (pos.y >= 0 && pos.y < pixel_art->height);
}

void fill_from_pos_rec(Pixel_Art * pixel_art, SDL_Color color, SDL_Color new_color, SDL_Point pos){
	int index = pos.x + pos.y*pixel_art->width;
	SDL_Color current_color = pixel_art->image[index];
	if(!are_color_equal(current_color, color))
		return;
	pixel_art->image[index] = new_color;
	
	SDL_Point new = {pos.x, pos.y - 1};
	if(is_point_inside(pixel_art, new))
		fill_from_pos_rec(pixel_art, color, new_color, new);
	new = (SDL_Point){pos.x - 1, pos.y};
	if(is_point_inside(pixel_art, new))
		fill_from_pos_rec(pixel_art, color, new_color, new);
	new = (SDL_Point){pos.x, pos.y + 1};
	if(is_point_inside(pixel_art, new))
		fill_from_pos_rec(pixel_art, color, new_color, new);
	new = (SDL_Point){pos.x + 1, pos.y};
	if(is_point_inside(pixel_art, new))
		fill_from_pos_rec(pixel_art, color, new_color, new);

}

void fill_from_pos(Pixel_Art * pixel_art, SDL_Color new_color, SDL_Point pos){
	int x = pos.x - pixel_art->rect.x;
	int y = pos.y - pixel_art->rect.y;

	int xc = x / pixel_art->tile_size;
	int yc = y / pixel_art->tile_size;

	SDL_Color color = pixel_art->image[xc + yc*pixel_art->width];
	if(are_color_equal(new_color, color))
		return ;
	SDL_Point new_point = {xc, yc};

	fill_from_pos_rec(pixel_art, color, new_color, new_point);
}