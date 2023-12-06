#include "activation.h"

double sigmoid(double input) {
	return 1.0 / (1 + exp(-1 * input));
}

double sigmoid_prime(double input)
{
    return sigmoid(input)*(1-sigmoid(input));
}

double leaky_relu(double x) { return x > 0 ? 0.01 * x : 0; }

double d_relu(double x) { return x < 0 ? 0 : 1; }

//double softmax(double x, double sum) { return exp(x) / sum; }

Matrix* softmax(Matrix* m) {
	double total = 0;
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			total += exp(m->entries[i][j]);
		}
	}
	Matrix* mat = matrix_create(m->rows, m->cols);
	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			mat->entries[i][j] = exp(m->entries[i][j]) / total;
		}
	}
	return mat;
}