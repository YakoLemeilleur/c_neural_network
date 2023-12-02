#include "activation.h"

double sigmoid(double input) {
	return 1.0 / (1 + exp(-1 * input));
}

double sigmoid_prime(double input)
{
    return sigmoid(input)*(1-sigmoid(input));
}