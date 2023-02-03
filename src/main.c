#include "pixel_art.h"
#include "render.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int status = EXIT_FAILURE;

#pragma region OPTIONS
	int width = 0, height = 0;
	char *default_destination = "output.ppm";
	char *destination		  = "output.ppm";
	char *source			  = NULL;
	int opt;

	while ((opt = getopt(argc, argv, "s:o:f:")) != -1) {
		switch (opt) {
			case 's':
				if (sscanf(optarg, "%dx%d", &width, &height) != 2) {
					fprintf(stderr, "Invalid size format\n");
					exit(EXIT_FAILURE);
				}
				printf("Width: %d \t Height: %d\n", width, height);
				break;
			case 'o':
				destination = optarg;
				break;
			case 'f':
				source = optarg;
				printf("Loading %s...\n", optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s -s <width>x<height> -f <source> -o <destination> \n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if (width == 0 && height == 0 && source == NULL) {
		printf("No source file and dimensions, setup with 15x15\n");
		width  = 15;
		height = 15;
	}

	if ((width != 0 || height != 0) && source != NULL) {
		printf("Warning : width and height cannot be defined when choosing source file\n");
		width  = 0;
		height = 0;
	}

	if (strcmp(destination, default_destination) == 0 && source != NULL) {
		destination = source;
		printf("Saved file %s will overwrite existing file %s\n", destination, source);
	}

	FILE *input = NULL;
	if (source != NULL)
		input = fopen(source, "r");
	if (source != NULL && input == NULL) {
		fprintf(stderr, "Source file %s does not exists\n", source);
		exit(EXIT_FAILURE);
	}

	printf("Output file: %s\n", destination);
	FILE *output = NULL;

#pragma endregion

	TTF_Init();
	TTF_Font *FONT	   = TTF_OpenFont("fonts/MonoglycerideDemiBold.ttf", 18);
	TTF_Font *BAR_FONT = TTF_OpenFont("fonts/MonoglycerideDemiBold.ttf", 15);

	SDL_Color background_color = { 20, 28, 36, 255 };
	SDL_Color p_background	   = { 40, 56, 72, 255 };
	SDL_Color b_color		   = { 40, 40, 150, 255 };

	HSV_Color selected_hs = { 0, 0, 0 };
	SDL_Color primary_c	  = { 0, 0, 0, 255 };
	SDL_Color white		  = { 255, 255, 255, 255 };
	SDL_Color secondary_c = { 255, 255, 255, 255 };

#pragma region SDL INIT
	SDL_Window *window	   = NULL;
	SDL_Renderer *renderer = NULL;

	SDL_Event event;
	int mouseX, mouseY;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error using SDL_Init : %s\n", SDL_GetError());
		goto Quit;
	}

	window = SDL_CreateWindow("Pixel Art Tool", 40, 60, WIDTH, HEIGHT, 0);
	if (window == NULL) {
		fprintf(stderr, "Error with SDL_CreateWindow : %s\n", SDL_GetError());
		goto Quit;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error with SDL_CreateRenderer : %s\n", SDL_GetError());
		goto Quit;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

#pragma endregion

#pragma region PIXEL ART INIT
	PixelArt pixel_art = {};
	if (input != NULL) {
		get_dimension(input, &pixel_art);
		printf("Detected size %dx%d in file %s\n", pixel_art.width, pixel_art.height, source);
		width  = pixel_art.width;
		height = pixel_art.height;
	} else {
		pixel_art.width	 = width;
		pixel_art.height = height;
	}

	int tile_size =
		fmin((float) (WIDTH - P_WIDTH - OFFSET - 20) / (float) width, (float) (HEIGHT - OFFSET) / (float) height);
	int x		  = ((WIDTH - P_WIDTH - 20) - width * tile_size) / 2;
	int y		  = (HEIGHT - height * tile_size) / 2 - 10;
	SDL_Rect rect = { x, y, width * tile_size, height * tile_size };

	pixel_art.x			= x;
	pixel_art.y			= y;
	pixel_art.tile_size = tile_size;
	pixel_art.rect		= rect;

	pixel_art.image = malloc(sizeof(SDL_Color) * height * width);
	if (input == NULL)
		fill_image(&pixel_art, white);
	else
		fill_from_file(input, &pixel_art);

#pragma endregion

	SDL_Rect palette_r	  = { (WIDTH - P_WIDTH) + 20, (HEIGHT - 200) - 20, P_WIDTH - 40, 200 };
	SDL_Rect luminosity_r = { (WIDTH - P_WIDTH) + 20, (HEIGHT - 200 - 60) - 20, P_WIDTH - 40, 40 };
	SDL_Rect primary_r	  = { (WIDTH - P_WIDTH) + 20, (HEIGHT - 200 - 60 - 60) - 20, (P_WIDTH - 40) / 2 - 10, 40 };
	SDL_Rect secondary_r  = { (WIDTH - P_WIDTH / 2 + 10), (HEIGHT - 200 - 60 - 60) - 20, (P_WIDTH - 40) / 2 - 10, 40 };

#pragma region MAIN LOOP
	load_assets(renderer);
	enum tool tool = PEN;
	SDL_Point mouse_pos;
	int click	   = 0;
	int clicked_up = 0;

	int running = 1;
	while (running) {
		long int start = get_time_micro();
		fill_background(renderer, background_color);
		render_panel_background(renderer, p_background);

		Uint32 mouse_state = SDL_GetMouseState(&mouseX, &mouseY);
		mouse_pos		   = (SDL_Point){ mouseX, mouseY };

		if (mouse_state & SDL_BUTTON_LMASK) {
			click = 1;
		}

		if (SDL_PointInRect(&mouse_pos, &palette_r) && (mouse_state & SDL_BUTTON_LMASK)) {
			selected_hs = get_palette_color(&mouse_pos, &palette_r);
		}

		if (SDL_PointInRect(&mouse_pos, &luminosity_r)) {
			if (mouse_state & SDL_BUTTON_LMASK)
				primary_c = get_lum_color(&mouse_pos, &luminosity_r, selected_hs);
			if (mouse_state & SDL_BUTTON_RMASK)
				secondary_c = get_lum_color(&mouse_pos, &luminosity_r, selected_hs);
		}

		if (tool == PEN && SDL_PointInRect(&mouse_pos, &pixel_art.rect)) {
			if (mouse_state & SDL_BUTTON_LMASK)
				change_image_color(&mouse_pos, &pixel_art, primary_c);
			if (mouse_state & SDL_BUTTON_RMASK)
				change_image_color(&mouse_pos, &pixel_art, secondary_c);
		}

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						if (SDL_PointInRect(&mouse_pos, &pixel_art.rect)) {
							if (tool == BUCKET)
								fill_from_pos(&pixel_art, primary_c, mouse_pos);
							if (tool == PIPETTE)
								primary_c = get_color_at(&mouse_pos, &pixel_art);
						}
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						if (SDL_PointInRect(&mouse_pos, &pixel_art.rect)) {
							if (tool == BUCKET)
								fill_from_pos(&pixel_art, secondary_c, mouse_pos);
							if (tool == PIPETTE)
								secondary_c = get_color_at(&mouse_pos, &pixel_art);
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {
						clicked_up = 1;
					}
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							fill_image(&pixel_art, white);
							break;
						case SDLK_w:
							secondary_c = (SDL_Color){ 255, 255, 255, 255 };
							break;
					}
			}
		}

		int result = update_buttons(renderer, &mouse_pos, &tool, click, clicked_up);
		if (result == 1) {
			printf("Saving pixel art to %s...\n", destination);
			output = fopen(destination, "w");
			if (output == NULL) {
				perror("Error opening file");
				exit(EXIT_FAILURE);
			}
			save(output, &pixel_art);
			printf("Saved pixel art successfully !\n");
		}

		draw_spectrum(renderer, &palette_r, selected_hs);
		draw_selected_color_sat(renderer, &luminosity_r, selected_hs);
		draw_selected_colors(renderer, &primary_r, primary_c, &secondary_r, secondary_c);
		draw_pixel_art(renderer, &pixel_art);

		render_status_bar(renderer, BAR_FONT, &pixel_art, mouse_pos, b_color);
		int fps = get_fps(start);
		char fps_text[7];
		sprintf(fps_text, "%d", fps);

		blit_text(renderer, FONT, fps_text, (SDL_Point){ 20, 20 }, white);

		SDL_RenderPresent(renderer);
		click	   = 0;
		clicked_up = 0;
	}

	status = EXIT_SUCCESS;
#pragma endregion

Quit:
	free_assets();
	free(pixel_art.image);
	if (input != NULL)
		fclose(input);
	if (output != NULL)
		fclose(output);
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (window != NULL)
		SDL_DestroyWindow(window);

	SDL_Quit();
	return status;
}
