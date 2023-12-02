#include "img.h"

#define MAXCHAR 10000

Img** csv_to_imgs(char* file_string, int number_of_imgs) {
	FILE *fp;
	Img** imgs = malloc((number_of_imgs+1) * sizeof(Img*));
	char row[MAXCHAR];
	fp = fopen(file_string, "r");

	// Read the first line 
	if (fgets(row, MAXCHAR, fp) == NULL) {
        errx(EXIT_FAILURE, "fgets()");
    }
    int i = 0;
	while (feof(fp) != 1 && i < number_of_imgs) {
		imgs[i] = malloc(sizeof(Img));

		int j = 0;
		if (fgets(row, MAXCHAR, fp) == NULL) {
            errx(EXIT_FAILURE, "fgets()");
        }
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

void shuffle(Img **imgs, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          Img *temp = img_copy(imgs[j]);
          img_free(imgs[j]);
          imgs[j] = img_copy(imgs[i]);
          imgs[i] = img_copy(temp);
          img_free(temp);
        }
    }
}

void img_print(Img* img) {
    printf("Img Label: %d\n", img->label);
	matrix_print(img->img_data);
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

Img* img_copy(Img *img)
{
    Img *result = malloc(sizeof(Img));
    result->label = img->label;
    result->img_data = matrix_copy(img->img_data);
    return result;
}