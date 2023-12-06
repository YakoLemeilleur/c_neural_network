#pragma once

#include "matrix.h"
#include "ops.h"
#include "activation.h"
#include "img.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <unistd.h>


typedef struct {
	int input; //Number of input neurons
	int output; //Number of output neurons
	double learning_rate; //Learning rate
	int hidden; //Number of hidden layers
	int *hiddens; //Number of neurons per hidden layer
	Matrix** hidden_weights; //List of all the weights in the hidden layers
	Matrix** hidden_bias; //List of all the bias in the hidden layers
	Matrix* output_bias; //Bias in the output layer
	Matrix* output_weights; //Weight in the output layer
} NeuralNetwork;


NeuralNetwork* network_create(int input, int hidden, int *hiddens, int output, double lr);
void network_print(NeuralNetwork* net);
void network_free(NeuralNetwork *net);
Matrix* network_predict(NeuralNetwork* net, Img* input);
void network_backpropagate(NeuralNetwork *net, Img *img, Matrix* output);
void network_train(NeuralNetwork *net, Img** imgs, size_t n_imgs, size_t epoch);
double network_test(NeuralNetwork *net, Img **imgs, size_t n_imgs);
double network_test_xor(NeuralNetwork *net, Img **imgs, size_t n_imgs);
NeuralNetwork* network_load(char* file_string);
void network_save(NeuralNetwork* net, char* file_string);