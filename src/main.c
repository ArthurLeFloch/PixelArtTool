#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "render.h"
#include "utils.h"
#include "pixel_art.h"

int main(int argc, char * argv[]){
	int status = EXIT_FAILURE;

	#pragma region OPTIONS
	int width=15, height=15;
	char *filename = "output.ppm";
	int opt;

	while((opt = getopt(argc, argv, "s:o:")) != -1){
		switch(opt){
			case 's':
				if(sscanf(optarg, "%dx%d", &width, &height) != 2){
					fprintf(stderr, "Invalid size format\n");
					exit(EXIT_FAILURE);
				}
				printf("Width: %d \t Height: %d\n", width, height);
				break;
			case 'o':
				filename = optarg;
				break;
			default:
				fprintf(stderr, "Usage: %s -s <width>x<height> -o <filename> \n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	printf("Output file: %s\n", filename);
	FILE *output = NULL;

	#pragma endregion

	SDL_Color background_color = {20, 28, 36, 255};
	HSV_Color selected_hs = {0, 0, 0};
	SDL_Color primary_c = {0, 0, 0, 255};
	SDL_Color white = {255, 255, 255, 255};
	SDL_Color secondary_c = {255, 255, 255, 255};

	#pragma region SDL INIT
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;    

	SDL_Event event;
	int mouseX, mouseY;
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "Error using SDL_Init : %s\n", SDL_GetError());
		goto Quit;
	}

	window = SDL_CreateWindow("Pixel Art Tool", 40, 60, WIDTH, HEIGHT, 0);
	if(window == NULL){
		fprintf(stderr, "Error with SDL_CreateWindow : %s\n", SDL_GetError());
		goto Quit;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if(renderer == NULL){
		fprintf(stderr, "Error with SDL_CreateRenderer : %s\n", SDL_GetError());
		goto Quit;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	#pragma endregion

	#pragma region PIXEL ART INIT
	int tile_size = fmin((float)(WIDTH - P_WIDTH - OFFSET - 20) / (float)width, (float)(HEIGHT - OFFSET) / (float)height);
	int x_offset = ((WIDTH - P_WIDTH - 20) - width * tile_size) / 2;
	int y_offset = (HEIGHT - height * tile_size) / 2;
	SDL_Rect rect = {x_offset, y_offset, width*tile_size, height*tile_size};

	Pixel_Art pixel_art = {width, height, x_offset, y_offset, tile_size, rect, NULL};
	pixel_art.image = malloc(sizeof(SDL_Color) * height * width);
	fill_image(&pixel_art, white);

	#pragma endregion

	SDL_Rect palette_r = {(WIDTH - P_WIDTH) - 10, (HEIGHT - 200) - 10, P_WIDTH, 200};
	SDL_Rect luminosity_r = {(WIDTH - P_WIDTH) - 10, (HEIGHT - 200 - 60) - 10, P_WIDTH, 40};
	SDL_Rect primary_r = {(WIDTH - P_WIDTH) - 10, (HEIGHT - 200 - 60 - 60) - 10, P_WIDTH/2 - 10, 40};
	SDL_Rect secondary_r = {(WIDTH - P_WIDTH/2 + 10) - 10, (HEIGHT - 200 - 60 - 60) - 10, P_WIDTH/2 - 10, 40};

	#pragma region MAIN LOOP
	load_assets(renderer);
	enum tool tool = PEN;
	SDL_Point mouse_pos;
	int running = 1;
	while(running){
		fill_background(renderer, background_color);

		Uint32 mouse_state = SDL_GetMouseState(&mouseX, &mouseY);
		mouse_pos = (SDL_Point){mouseX, mouseY};

		if(SDL_PointInRect(&mouse_pos, &palette_r) && (mouse_state & SDL_BUTTON_LMASK)){
			selected_hs = get_palette_color(&mouse_pos, &palette_r);
		}

		if(SDL_PointInRect(&mouse_pos, &luminosity_r)){
			if(mouse_state & SDL_BUTTON_LMASK)
				primary_c = get_lum_color(&mouse_pos, &luminosity_r, selected_hs);
			if(mouse_state & SDL_BUTTON_RMASK)
				secondary_c = get_lum_color(&mouse_pos, &luminosity_r, selected_hs);
		}

		if(tool == PEN && SDL_PointInRect(&mouse_pos, &pixel_art.rect)){
			if(mouse_state & SDL_BUTTON_LMASK)
				change_image_color(&mouse_pos, &pixel_art, primary_c);
			if(mouse_state & SDL_BUTTON_RMASK)
				change_image_color(&mouse_pos, &pixel_art, secondary_c);
		}
		
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT){
						if(SDL_PointInRect(&mouse_pos, &pixel_art.rect)){
							if(tool == BUCKET)
								fill_from_pos(&pixel_art, primary_c, mouse_pos);
							if(tool == PIPETTE)
								primary_c = get_color_at(&mouse_pos, &pixel_art);
						}
						set_selected_tool(&mouse_pos, &tool);
					} else if(event.button.button == SDL_BUTTON_RIGHT){
						if(SDL_PointInRect(&mouse_pos, &pixel_art.rect)){
							if(tool == BUCKET)
								fill_from_pos(&pixel_art, secondary_c, mouse_pos);
							if(tool == PIPETTE)
								secondary_c = get_color_at(&mouse_pos, &pixel_art);
						}
					}
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							fill_image(&pixel_art, white);
							break;
						case SDLK_s:
							printf("Saving pixel art to %s...\n", filename);
							output = fopen(filename, "w");
							if(output == NULL){
								perror("Error opening file");
								exit(EXIT_FAILURE);
							}
							save(output, &pixel_art);
							printf("Saved pixel art successfully !\n");
							break;
					}
			}
		}

		render_icons(renderer, tool);
		
		draw_spectrum(renderer, &palette_r, selected_hs);
		draw_selected_color_sat(renderer, &luminosity_r, selected_hs);
		draw_selected_colors(renderer, &primary_r, primary_c, &secondary_r, secondary_c);
		draw_pixel_art(renderer, &pixel_art);
		
		SDL_RenderPresent(renderer);
	}

	status = EXIT_SUCCESS;
	#pragma endregion

Quit:
	free_assets();
	free(pixel_art.image);
	if(output != NULL)
		fclose(output);
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if(window != NULL)
		SDL_DestroyWindow(window);
	
	SDL_Quit();
	return status;
}
