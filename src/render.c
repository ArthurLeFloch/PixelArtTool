#include "render.h"

typedef struct Icon {
	enum tool tool;
	SDL_Rect rect;
	SDL_Texture *texture;
} Icon;

Icon pen, bucket, pipette, saving;

enum button_state { SELECTED = 0, CLICKED, HOVERED, NO_STATE };

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color red	= { 255, 100, 100, 255 };

SDL_Color selected = { 255, 255, 255, 255 };
SDL_Color clicked  = { 100, 255, 100, 255 };
SDL_Color hovered  = { 100, 100, 255, 255 };
SDL_Color classic  = { 100, 100, 100, 255 };

void draw_outline(SDL_Renderer *renderer, const SDL_Rect *rect, SDL_Color color) {
	SDL_Rect rect1 = { rect->x - 4, rect->y - 4, rect->w + 8, rect->h + 8 };
	SDL_Rect rect2 = { rect->x - 1, rect->y - 1, rect->w + 2, rect->h + 2 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect1);
	SDL_RenderDrawRect(renderer, &rect2);

	rect1 = (SDL_Rect){ rect->x - 2, rect->y - 2, rect->w + 4, rect->h + 4 };
	rect2 = (SDL_Rect){ rect->x - 3, rect->y - 3, rect->w + 6, rect->h + 6 };

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawRect(renderer, &rect1);
	SDL_RenderDrawRect(renderer, &rect2);
}

int fill_background(SDL_Renderer *renderer, SDL_Color color) {
	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
		return -1;
	if (SDL_RenderClear(renderer) < 0)
		return -1;
	return 0;
}

void draw_spectrum(SDL_Renderer *renderer, SDL_Rect *output, HSV_Color selected) {
	draw_outline(renderer, output, white);
	for (int s = 0; s < output->h; s++) {
		for (int h = 0; h < output->w; h++) {
			HSV_Color hsv	= { h / (float) output->w, s / (float) output->h, 0.8f };
			SDL_Color color = hsv_to_rgb(hsv);
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawPoint(renderer, output->x + h, output->y + 200 - s - 1);
		}
	}
	SDL_Rect rect = { output->x + roundf(output->w * selected.h) - 10, output->y + roundf(200 * (1 - selected.s)) - 10,
					  20, 20 };
	draw_outline(renderer, &rect, white);
}

void draw_selected_colors(SDL_Renderer *renderer, SDL_Rect *primary_r, SDL_Color primary, SDL_Rect *secondary_r,
						  SDL_Color secondary) {
	draw_outline(renderer, primary_r, white);
	SDL_SetRenderDrawColor(renderer, primary.r, primary.g, primary.b, primary.a);
	SDL_RenderFillRect(renderer, primary_r);

	draw_outline(renderer, secondary_r, red);
	SDL_SetRenderDrawColor(renderer, secondary.r, secondary.g, secondary.b, secondary.a);
	SDL_RenderFillRect(renderer, secondary_r);
}

void draw_selected_color_sat(SDL_Renderer *renderer, SDL_Rect *output, HSV_Color color) {
	draw_outline(renderer, output, white);
	HSV_Color hsv;
	SDL_Color new_color;
	int high = output->y, low = output->y + output->h - 1;
	for (int lum = 0; lum < output->w; lum++) {
		int x	  = round(output->x + lum);
		hsv		  = (HSV_Color){ color.h, color.s, lum / (float) output->w };
		new_color = hsv_to_rgb(hsv);

		SDL_SetRenderDrawColor(renderer, new_color.r, new_color.g, new_color.b, new_color.a);
		SDL_RenderDrawLine(renderer, x, high, x, low);
	}
}

void draw_pixel_art(SDL_Renderer *renderer, const PixelArt *pixel_art) {
	draw_outline(renderer, &pixel_art->rect, white);
	SDL_Color color;
	SDL_Rect rect;
	int tile_size = pixel_art->tile_size;
	for (int y = 0; y < pixel_art->height; y++) {
		for (int x = 0; x < pixel_art->width; x++) {
			color = pixel_art->image[x + y * pixel_art->width];
			rect  = (SDL_Rect){ pixel_art->x + x * tile_size, pixel_art->y + y * tile_size, tile_size, tile_size };

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

SDL_Texture *load_image(SDL_Renderer *renderer, const char *path) {
	SDL_Surface *loaded_surface = SDL_LoadBMP(path);
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 255, 255, 255));

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	SDL_FreeSurface(loaded_surface);
	return texture;
}

void load_assets(SDL_Renderer *renderer) {
	pen.texture = load_image(renderer, "assets/pen.bmp");
	int x		= (WIDTH - P_WIDTH) + 20;
	int y		= 20;
	pen.rect	= (SDL_Rect){ x, y, 60, 60 };
	pen.tool	= PEN;

	bucket.texture = load_image(renderer, "assets/bucket.bmp");
	x			   = (WIDTH - P_WIDTH) + 20;
	y			   = 100;
	bucket.rect	   = (SDL_Rect){ x, y, 60, 60 };
	bucket.tool	   = BUCKET;

	pipette.texture = load_image(renderer, "assets/pipette.bmp");
	x				= (WIDTH - P_WIDTH) + 20;
	y				= 180;
	pipette.rect	= (SDL_Rect){ x, y, 60, 60 };
	pipette.tool	= PIPETTE;

	saving.texture = load_image(renderer, "assets/save.bmp");
	x			   = WIDTH - 60 - 20;
	y			   = 20;
	saving.rect	   = (SDL_Rect){ x, y, 60, 60 };
	saving.tool	   = NONE;
}

void free_assets() {
	SDL_DestroyTexture(pen.texture);
	SDL_DestroyTexture(bucket.texture);
	SDL_DestroyTexture(pipette.texture);
	SDL_DestroyTexture(saving.texture);
}

void update_icon(SDL_Renderer *renderer, Icon *icon, enum button_state state) {
	SDL_Rect background = { icon->rect.x - 4, icon->rect.y - 4, icon->rect.w + 8, icon->rect.h + 8 };
	SDL_Color outline	= classic;
	int alpha			= 200;
	if (state == NO_STATE)
		alpha = 80;
	if (state == SELECTED)
		outline = white;
	else if (state == CLICKED)
		outline = selected;
	else if (state == HOVERED)
		outline = hovered;

	draw_outline(renderer, &background, outline);
	SDL_SetRenderDrawColor(renderer, 120, 120, 180, alpha);
	SDL_RenderFillRect(renderer, &background);

	SDL_RenderCopy(renderer, icon->texture, NULL, &icon->rect);
}

int update_button(SDL_Renderer *renderer, SDL_Point *pos, Icon *icon, enum tool *tool, int click, int clicked) {
	enum button_state state = NO_STATE;
	int result				= 0;
	if (*tool == icon->tool)
		state = SELECTED;
	else if (SDL_PointInRect(pos, &icon->rect)) {
		if (clicked) {
			if (icon->tool != NONE)
				*tool = icon->tool;
			state = CLICKED;
		} else if (click)
			state = CLICKED;
		else
			state = HOVERED;

		if (clicked)
			result = 1;
	}
	update_icon(renderer, icon, state);
	return result;
}

int update_buttons(SDL_Renderer *renderer, SDL_Point *pos, enum tool *tool, int click, int clicked) {
	// Returns 1 if save is needed
	update_button(renderer, pos, &pen, tool, click, clicked);
	update_button(renderer, pos, &bucket, tool, click, clicked);
	update_button(renderer, pos, &pipette, tool, click, clicked);
	int result = update_button(renderer, pos, &saving, tool, click, clicked);
	return result;
}

void blit_text(SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Point pos, SDL_Color color) {
	SDL_Surface *text_surface = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Texture *texture	  = SDL_CreateTextureFromSurface(renderer, text_surface);

	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	SDL_Rect txt_rect = { pos.x, pos.y, width, height };

	SDL_RenderCopy(renderer, texture, NULL, &txt_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(texture);
}

void render_status_bar(SDL_Renderer *renderer, TTF_Font *font, PixelArt *pixel_art, SDL_Point pos, SDL_Color bg_color) {
	SDL_Rect rect = { 0, HEIGHT - 20, WIDTH - P_WIDTH, 20 };

	int x = pos.x - pixel_art->rect.x;
	int y = pos.y - pixel_art->rect.y;

	int xc = x / pixel_art->tile_size;
	int yc = y / pixel_art->tile_size;

	if (xc >= pixel_art->width)
		xc = pixel_art->width - 1;
	if (yc >= pixel_art->height)
		yc = pixel_art->height - 1;

	SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
	SDL_RenderFillRect(renderer, &rect);

	char pos_txt[30];
	sprintf(pos_txt, "Position : (%d, %d)", xc, yc);

	SDL_Point place = { 20, HEIGHT - 20 };
	blit_text(renderer, font, pos_txt, place, white);
}

void render_panel_background(SDL_Renderer *renderer, SDL_Color bg_color) {
	SDL_Rect panel = { WIDTH - P_WIDTH, 0, P_WIDTH, HEIGHT };
	SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
	SDL_RenderFillRect(renderer, &panel);
}