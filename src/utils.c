#include "utils.h"

#include <sys/time.h>

HSV_Color get_palette_color(SDL_Point *pos, SDL_Rect *palette) {
	int x = pos->x - palette->x;
	int y = pos->y - palette->y;

	float value		 = 0;
	float hue		 = x / (float) palette->w;
	float saturation = (1.0f - y / (float) palette->h);

	return (HSV_Color){ hue, saturation, value };
}

SDL_Color get_lum_color(SDL_Point *pos, SDL_Rect *lum, HSV_Color color) {
	int x = pos->x - lum->x;

	float luminosity = x / (float) lum->w;

	HSV_Color result = { color.h, color.s, luminosity };
	return hsv_to_rgb(result);
}

long int get_time_micro() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int) 1e6 + currentTime.tv_usec;
}

int get_fps(long int start) {
	long int end = get_time_micro();
	return 1e6 / (end - start);
}