#pragma once
#include "matrix.h"
#include <math.h>

double sigmoid(double input);
double sigmoid_prime(double input);
double leaky_relu(double x);
double d_relu(double x);
Matrix* softmax(Matrix* m);