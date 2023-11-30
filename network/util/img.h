#pragma once

#include "../matrix/matrix.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
	Matrix* img_data;
	int label;
} Img;

Img** csv_to_imgs(char* file_string, int number_of_imgs);
Img* png_to_img(char * file_string);
void img_print(Img* img);
void img_free(Img *img);
void imgs_free(Img **imgs, int n);
