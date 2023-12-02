#include <stdio.h>
#include "matrix.h"
#include "nn.h"
#include "img.h"
#include <stdlib.h>

int main()
{
    srand(3);
    /*
    int hiddens[1] = { 2 };
    NeuralNetwork *net = network_create(2, 1, hiddens, 1, 0.1);
    
    network_print(net);
    printf("\n\n\n");
    
    int n = 100;
    Img **inputs = malloc(n * sizeof(Img*));
    for (int i = 0; i < n; i++)
    {
        inputs[i] = malloc(sizeof(Img));
        inputs[i]->img_data = matrix_create(2, 1);
        int l1 = rand() % 2;
        int l2 = rand() % 2;
        inputs[i]->img_data->entries[0][0] = l1;
        inputs[i]->img_data->entries[1][0] = l2;
        inputs[i]->label = l1 ^ l2;
    }
    
    
    
    double result = network_test_xor(net, inputs, n);
    printf("\nNetwork result : %f\n\n", result);
    
    network_train_xor(net, inputs, n, 1000);
    network_print(net);
    
    result = network_test_xor(net, inputs, n);
    
    
    printf("\n\n\nProut:\n");
    img_print(inputs[1]);
    Matrix *prediction = network_predict(net, inputs[1]);
    matrix_print(prediction);
    matrix_free(prediction);
    
    printf("\n\n\nNetwork result : %f\n\n", result);
    
    imgs_free(inputs, n);
    network_free(net);*/
    

    int is_training;
    printf("Train(1) or Predict(0) ?\n");
    scanf("%d",&is_training);
    if (is_training) {
        //TRAINING
        int number_imgs = 10000;
        Img** imgs = csv_to_imgs("./data/mnist_train.csv", number_imgs);
        int hiddens2[1] = { 200 };
        NeuralNetwork* net2 = network_create(784, 1, hiddens2, 10, 0.1);
        network_train(net2, imgs, number_imgs, 3);
        network_save(net2, "testing_net");
        imgs_free(imgs, number_imgs);
        network_free(net2);
    }
    else {
        //	PREDICTING
        int number_imgs = 6;
        Img** imgs = csv_to_imgs("data/result.csv", number_imgs);
        NeuralNetwork* net = network_load("testing_net");
        double score = network_test(net, imgs, (size_t)number_imgs);
        printf("Score: %1.5f\n", score);
        imgs_free(imgs, number_imgs);
        network_free(net);
    }
    
    /*
    int number_imgs = 10000;
    Img** imgs = csv_to_imgs("./data/mnist_train.csv", number_imgs);
    int hiddens2[1] = { 100 };
    NeuralNetwork* net = network_create(784, 1, hiddens2, 10, 0.1);
    double score = network_test(net, imgs, (size_t)number_imgs);
    printf("Score: %1.5f\n", score);

    network_train(net, imgs, number_imgs, 10);
    imgs_free(imgs, number_imgs);

    number_imgs = 100;
    Img** imgs2 = csv_to_imgs("data/mnist_test.csv", number_imgs);
    network_test(net, imgs2, number_imgs);
    printf("Successfully trained the network\n");
    
    score = network_test(net, imgs2, (size_t)number_imgs);
    printf("Score: %1.5f\n", score);
    imgs_free(imgs2, number_imgs);
    network_free(net);*/
    return 0;
}
