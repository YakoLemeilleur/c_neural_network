#include "img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#define MAXCHAR 10000

Img** csv_to_imgs(char* file_string, int number_of_imgs) {
	FILE *fp;
	Img** imgs = malloc(number_of_imgs * sizeof(Img*));
	char row[MAXCHAR];
	fp = fopen(file_string, "r");

	// Read the first line 
	fgets(row, MAXCHAR, fp);
	int i = 0;
	while (feof(fp) != 1 && i < number_of_imgs) {
		imgs[i] = malloc(sizeof(Img));

		int j = 0;
		fgets(row, MAXCHAR, fp);
		char* token = strtok(row, ",");
		imgs[i]->img_data = matrix_create(28, 28);
		while (token != NULL) {
			if (j == 0) {
				imgs[i]->label = atoi(token);
			} else {
				imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] = atoi(token) > 125 ? 1 : 0;/// 256.0;
			}
			token = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
	return imgs;
}

SDL_Surface* load_image(const char* path)
{
	SDL_Surface * temp = IMG_Load(path);
	SDL_Surface * image = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);
	if((image == NULL) | (temp == NULL)) {
		printf("%s%s\n", temp,image);
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	}
	SDL_FreeSurface(temp);
	return image;
}



Img* png_to_img(char* file_string) {
	FILE * fp;
	Img* res = malloc(sizeof(Img));
	res->img_data = matrix_create(28,28);
	SDL_Surface* surface = load_image(file_string);
	Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;
	if(SDL_LockSurface(surface) != 0) {
		errx(EXIT_FAILURE,"%s",SDL_GetError());
	}
	for (int i = 0; i < surface->w; i++)
	{
		for (int j = 0; j < surface->h; j++)
		{
			Uint8 r, g, b;
			SDL_GetRGB(pixels[i*surface->w+j], surface->format, &r, &g, &b);
			res->img_data->entries[i][j] = r/255;
		}
	}
	SDL_UnlockSurface(surface);
	return res;
}

void img_print(Img* img) {
	matrix_print(img->img_data);
	printf("Img Label: %d\n", img->label);
}

void img_free(Img* img) {
	matrix_free(img->img_data);
	free(img);
	img = NULL;
}

void imgs_free(Img** imgs, int n) {
	for (int i = 0; i < n; i++) {
		img_free(imgs[i]);
	}
	free(imgs);
	imgs = NULL;
}
/*
int main() {
	Img* test = png_to_img("test.png");
	img_print(test);
	img_free(test);
}*/
