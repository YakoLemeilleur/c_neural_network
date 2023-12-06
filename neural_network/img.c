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
				imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] = atoi(token) > 100 ? 1 : 0;/// 256.0;
			}
			token = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
	return imgs;
}

void swap(Img** img1, Img** img2) {
	Img *temp = img_copy(*img1);
	img_free(*img1);
	*img1 = img_copy(*img2);
	img_free(*img2);
	*img2 = img_copy(temp);
	img_free(temp);
}

void shuffle(Img **imgs, size_t n)
{
    if (n > 1) 
    {
        for (size_t i = n-1; i > 0; --i) 
        {
			size_t j = rand() % (i + 1);
			if(j != i) {
				swap(&imgs[i], &imgs[j]);
			}
        }
    }
}

void img_print(Img* img) {
    printf("Img Label: %d\n", img->label);
	printf("Rows: %d Columns: %d\n", img->img_data->rows, img->img_data->cols);
	for (int i = 0; i < img->img_data->rows; i++) {
		for (int j = 0; j < img->img_data->cols; j++) {
			if(img->img_data->entries[i][j]) {
				printf("■");
			}
			else {
				printf(".");
			}
		}
		printf("\n");
	}
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