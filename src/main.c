#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <unistd.h>

#include "utils.h"

int main(int argc, char * argv[]){
	int status = EXIT_FAILURE;

	SDL_Color background_color = {10, 14, 18, 255};
    HSV_Color selected_hv = {0, 0, 0};
    SDL_Color selected_color = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};

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
    int width = 15, height = 15;
    int tile_size = fmin((float)(WIDTH - P_WIDTH - OFFSET) / (float)width, (float)(HEIGHT - OFFSET) / (float)height);
    int x_offset = ((WIDTH - P_WIDTH) - width * tile_size) / 2;
    int y_offset = (HEIGHT - height * tile_size) / 2;
    SDL_Rect rect = {x_offset, y_offset, width*tile_size, height*tile_size};

    Pixel_Art pixel_art = {width, height, x_offset, y_offset, tile_size, rect, NULL};
    pixel_art.image = malloc(sizeof(SDL_Color) * height * width);
    fill_image(&pixel_art, white);

    #pragma endregion

    SDL_Rect panel_palette_rect = {WIDTH-P_WIDTH, (HEIGHT - 200), P_WIDTH, 200};
    SDL_Rect panel_saturation_rect = {WIDTH-P_WIDTH, (HEIGHT - 200 - 60), P_WIDTH, 40};

    #pragma region MAIN LOOP
    SDL_Point mouse_pos;
	int running = 1;
	while(running){
		fill_background(renderer, background_color);

		Uint32 mouse_state = SDL_GetMouseState(&mouseX, &mouseY);
        mouse_pos = (SDL_Point){mouseX, mouseY};

        if(SDL_PointInRect(&mouse_pos, &panel_palette_rect) && (mouse_state & SDL_BUTTON_LMASK)){
            selected_hv = get_palette_color(&mouse_pos, &panel_palette_rect);
        }
        if(SDL_PointInRect(&mouse_pos, &panel_saturation_rect) && (mouse_state & SDL_BUTTON_LMASK)){
            selected_color = get_sat_color(&mouse_pos, &panel_saturation_rect, selected_hv);
        }
        if(SDL_PointInRect(&mouse_pos, &pixel_art.rect)){
            if(mouse_state & SDL_BUTTON_RMASK)
                change_image_color(&mouse_pos, &pixel_art, white);
            if(mouse_state & SDL_BUTTON_LMASK)
                change_image_color(&mouse_pos, &pixel_art, selected_color);
        }
        
		while(SDL_PollEvent(&event)){
			switch(event.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                /*case SDL_MOUSEBUTTONDOWN:
                    break;*/
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_SPACE:
                            fill_image(&pixel_art, white);
                            break;
                    }
            }
		}
    	
		draw_spectrum(renderer, 1);
        draw_selected_color_sat(renderer, selected_hv);
        draw_selected_color(renderer, selected_color);
        draw_pixel_art(renderer, &pixel_art);
		
    	SDL_RenderPresent(renderer);
	}

	status = EXIT_SUCCESS;
    #pragma endregion

Quit:
    free(pixel_art.image);
    if(renderer != NULL){
        SDL_DestroyRenderer(renderer);
    }
    if(window != NULL){
        SDL_DestroyWindow(window);
    }
    
    SDL_Quit();
    return status;
}
