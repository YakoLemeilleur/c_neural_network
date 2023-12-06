#include <stdio.h>
#include "matrix.h"
#include "nn.h"
#include "img.h"
#include <stdlib.h>

void train(Img** imgs, int number_imgs, int* hiddens, int epochs) {
    double score = 0;
    double score1 = 0;
    double score2 = 0;
    double score3 = 0;
    double score4 = 0;
    double score5 = 0;
    double learning_rate = 0.08;
    int i = 0;
    NeuralNetwork* net = network_create(784, 1, hiddens, 10, learning_rate);
    int number = 81;
    Img** img1 = csv_to_imgs("./data/image1.csv", number);
    Img** img2 = csv_to_imgs("./data/image2.csv", number);
    Img** img3 = csv_to_imgs("./data/image3.csv", number);
    Img** img4 = csv_to_imgs("./data/image4.csv", number);
    Img** img_test = csv_to_imgs("data/datatest.csv", 3000);
    while(score < 1) {
        printf("Epoch No. %d\n", i);
        network_train(net, imgs, number_imgs, epochs);
        printf("Learning rate: %lf\nSuccessfully trained net...\n",net->learning_rate);
        score1 = network_test(net, img1, (size_t)number);
        score2 = network_test(net, img2, (size_t)number);
        score3 = network_test(net, img3, (size_t)number);
        score4 = network_test(net, img4, (size_t)number);
        score5 = network_test(net, img_test, 3000);
        score = (score1 + score2 + score3 + score4)/4.0;
        printf("Score: %1.5f - %1.5f - %1.5f - %1.5f \nTesting data : %1.5f\n", score1, score2, score3, score4, score5);
        i++;
    }
    network_save(net, "testing_net");
    network_free(net);
    img_free(img1);
    img_free(img2);
    img_free(img3);
    img_free(img4);
    img_free(img_test);
}

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
    printf("Train(1), Predict(0) or Train for a while (2)?\n");
    scanf("%d",&is_training);
    if (is_training) {
        //TRAINING
        int number_imgs = 3000;
        Img** imgs = csv_to_imgs("./data/dataset.csv", number_imgs);
        int hiddens2[1] = { 40 };
        NeuralNetwork* net2 = network_create(784, 1, hiddens2, 10, 0.08);
        if(is_training == 2) {
            train(imgs,number_imgs,hiddens2, 1);
        }
        else {
            network_train(net2, imgs, number_imgs, 50);
            network_save(net2, "testing_net");
        }
        imgs_free(imgs, number_imgs);
        network_free(net2);
    }
    else {
        //	PREDICTING
        int number_imgs = 81;
        Img** imgs = csv_to_imgs("data/datatest.csv", number_imgs);
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
