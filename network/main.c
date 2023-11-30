#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/img.h"
#include "neural/activations.h"
#include "neural/nn.h"
#include "matrix/matrix.h"
#include "matrix/ops.h"
#include <err.h>
void test_neural_network(NeuralNetwork* net){
	int* input = malloc(2 * sizeof(int));
	char continu = 0;
	while(continu != '2') {
		printf("Enter 1 or 0\n");
		scanf("%d",input);
		printf("Enter 1 or 0\n");
		scanf("%d",input + 1);
		Matrix* prediction = network_predict_xor(net, input);
		printf("%d XOR %d = %lf\n", *input, input[1],prediction->entries[0][0]);
		printf("Enter a key to continue\n");
		scanf("%s", &continu);
	}
}

void test_neural_network_img(NeuralNetwork* net, char* path) {
	Img* img = png_to_img("../test.png");
	img_print(img);
	Img** imgs = malloc(sizeof(Img*));
	imgs[0] = img;
	double score = network_predict_imgs(net, imgs, 1);
	//Matrix* prediction = network_predict_img(net, img);
//	for (int i = 0; i < prediction->rows; i++) {
//		printf("prediction[%d] = %lf\n", i, prediction->entries[i][0]);
//	}
//	printf("The predicted number is : %d\n", matrix_argmax(prediction));
	printf("score: %lf\n", score);
	img_free(img);
}

int main() {
	srand(time(NULL));
	int* choice = malloc(sizeof(int));
	scanf("%d", choice);
	if(*choice == 42) {
		int* is_training = malloc(sizeof(int));
		printf("Train(1) or Predict(0) ?\n");
		scanf("%d",is_training);
		if(*is_training) {
			//TRAINING
			int number_imgs = 10000;
			Img** imgs = csv_to_imgs("./data/mnist_train.csv", number_imgs);
			
			NeuralNetwork* net = network_create(784, 400, 10, 0.1);
			network_train_batch_imgs(net, imgs, number_imgs);
			network_save(net, "testing_net");
		}
		else {
			//	PREDICTING
			int number_imgs = 3;
			Img** imgs = csv_to_imgs("data/result.csv", number_imgs);
//			img_print(imgs[0]);
			NeuralNetwork* net = network_load("testing_net");
			double score = network_predict_imgs(net, imgs, number_imgs);
			printf("Score: %1.5f\n", score);
			//printf("Now you can test your own images: please enter the path of the image you want to predict\n");
			//char path[40];
			//scanf("%s",path);
//			printf("Single test\n");
//			Img **imgs = csv_to_imgs("data/training.csv", 1);
	//		printf("after csv to img\n");
	//		double test = network_predict_imgs(net,imgs,1);
	//		printf("Score: %1.5f\n", test);
			test_neural_network_img(net, "./test.png");
			imgs_free(imgs, number_imgs);
			network_free(net);
			//
		}
	}
/*	else {
		int* is_training = malloc(sizeof(int));
		printf("Train(1) or Predict(0) ?\n");
		scanf("%d",is_training);
		if(*is_training) {	
			//TRAINING
			int nb_training_sets = 10000;
			int** training = malloc(nb_training_sets * sizeof(int*));
			for (int i = 0; i < nb_training_sets; i++) {
					training[i] = malloc(2 * sizeof(int));
					training[i][0] = rand() % 2;
					training[i][1] = rand() % 2; 
					//printf("training[i][0] = %d, training[i][1] = %d\n", training[i][0],training[i][1]);
				}
			NeuralNetwork* net = network_create(2, 4, 1, 0.1);
			int num_epochs = 10000; // Increased training iterations
			double* accuracies = malloc(10 * sizeof(int));
			for (int epoch = 0; epoch < num_epochs; epoch++) {
				network_train_batch_xors(net, training, nb_training_sets);
				double accuracy = network_predict_xors(net, training, nb_training_sets);
				if(accuracy * 100 == 100.00) {
					printf("100%% accuracy on epoch No. %d\n",epoch);	
					break;
				}
				if (epoch % 1000 == 0) {
					double accuracy = network_predict_xors(net, training, nb_training_sets);
					printf("Epoch %d, Accuracy: %.2f%%\n", epoch, accuracy * 100);
					accuracies[(epoch / 1000)] = accuracy; 
					if(epoch != 0 && accuracies[epoch/1000] == accuracies[(epoch/1000)-1]) {
						net = network_create(2, 4, 1, 0.1);
						printf("net changed\n");
					}
					if(accuracy * 100 == 100.00) break;
				}
			}
			network_save(net, "testing_xor_net");
			for (int i = 0; i < nb_training_sets; i++) {
				free(training[i]);
			}
			free(training);
			
		}
		else {
			//PREDICTING
			int prediction_number = 3000;
			int** training = malloc(prediction_number * sizeof(int*));
			for (int i = 0; i < prediction_number; i++) {
				training[i] = malloc(2 * sizeof(int));
				training[i][0] = rand() % 2;
				training[i][1] = rand() % 2; 
			}
			NeuralNetwork* net = network_load("testing_xor_net");
			double score = network_predict_xors(net, training, 1000);
			printf("Score: %1.5f%%\n",score * 100);
			for (int i = 0; i < prediction_number; i++) {
				free(training[i]);
			}
			free(training);
			test_neural_network(net);
			network_free(net);
		}
	//}*/
	return 0;
}
