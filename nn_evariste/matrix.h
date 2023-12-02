#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>



typedef struct {
	double** entries;
	int rows;
	int cols;
} Matrix;

Matrix* matrix_create(int row, int col);
void matrix_fill(Matrix *m, int n);
void matrix_free(Matrix *m);
void matrix_print(Matrix *m);
Matrix* matrix_copy(Matrix *m);
void matrix_randomize(Matrix* m, int n);
int matrix_closest(Matrix* m);
int matrix_argmax(Matrix* m);
Matrix* matrix_flatten(Matrix* m, int axis);
Matrix* matrix_load(char* file_string);
void matrix_save(Matrix* m, char* file_string);
double uniform_distribution(double low, double high);
