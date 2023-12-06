#pragma once

#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

typedef struct {
	Matrix* img_data;
	int label;
} Img;

void shuffle(Img **imgs, size_t n);
void img_free(Img* img);
Img* img_copy(Img *img);
void imgs_free(Img** imgs, int n);
void img_print(Img* img);
Img** csv_to_imgs(char* file_string, int number_of_imgs);