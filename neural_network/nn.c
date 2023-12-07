#include "nn.h"

#define MAXCHAR 10000

NeuralNetwork* network_create(int input, int hidden,int *hiddens, int output, double lr) {
	NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
	net->input = input;
	net->hidden = hidden;
	net->hiddens = hiddens;
	net->output = output;
	net->learning_rate = lr;
	Matrix** hidden_layers = malloc(hidden * sizeof(Matrix*));
	Matrix** hidden_layer_bias = malloc(hidden * sizeof(Matrix*));
	
	for (int i = 0; i < hidden; i++)
	{
	    hidden_layer_bias[i] = matrix_create(hiddens[i], 1);
	    if (i == 0)
	    {
	        hidden_layers[i] = matrix_create(hiddens[i], input);
	    }
	    else
	    {
	        hidden_layers[i] = matrix_create(hiddens[i], hiddens[i - 1]);
	    }
	    matrix_randomize(hidden_layer_bias[i], net->hiddens[i]);
	    matrix_randomize(hidden_layers[i], net->hiddens[i]);
	}
	
	Matrix* output_layer = matrix_create(output, hiddens[hidden - 1]);
	Matrix* output_bias = matrix_create(output, 1);
	matrix_randomize(output_layer, net->output);
	matrix_randomize(output_bias, net->output);
	net->hidden_bias = hidden_layer_bias;
	net->output_bias = output_bias;
	net->hidden_weights = hidden_layers;
	net->output_weights = output_layer;
	return net; 
}

Matrix* network_predict(NeuralNetwork* net, Img* img)
{
    if (img->img_data->rows != net->input || img->img_data->cols != 1)
    {
        errx(EXIT_FAILURE, "Invalid input.");
    }
    
    Matrix *weighted = dot(net->hidden_weights[0], img->img_data);
    Matrix *biased = add(weighted, net->hidden_bias[0]);
    Matrix *normalized = apply(sigmoid, biased);
    
    for (int i = 0; i < net->hidden; i++)
    {
        matrix_free(weighted);
        matrix_free(biased);
        if (i == net->hidden - 1)
        {
            weighted = dot(net->output_weights, normalized);
            biased = add(weighted, net->output_bias);
            matrix_free(normalized);
            normalized = apply(sigmoid, biased);
        }
        else
        {
            weighted = dot(net->hidden_weights[i + 1], normalized);
            biased = add(weighted, net->hidden_bias[i + 1]);
            matrix_free(normalized);
            normalized = apply(sigmoid, biased);
        }
    }
    matrix_free(weighted);
    matrix_free(biased);
    return normalized;
}

int** network_predict_full_sudoku(NeuralNetwork* net, Img** imgs, size_t n_imgs)
{
    if (n_imgs != 81)
    {
        errx(EXIT_FAILURE, "Invalid number of images.");
    }

    int** result = malloc(9 * sizeof(int*));
	for (int i = 0; i < 9; i++) {
		result[i] = malloc(9 * sizeof(int));
	}
    for (size_t i = 0; i < 81; i++)
    {
        Matrix *prediction = network_predict(net, imgs[i]);
        result[i / 9][i % 9] = matrix_argmax(prediction);
        matrix_free(prediction);
    }

    return result;
}

void network_train(NeuralNetwork *net, Img** imgs, size_t n_imgs, size_t epoch)
{
    Img* flattened = malloc(sizeof(Img));
    Matrix* output;
    for (size_t i = 0; i < epoch; i++)
    {
        printf("Epoch No. %ld\n", i);
        shuffle(imgs, n_imgs); // à inclure seulement si les datasets ne sont pas ordonnés. 
        for (size_t j = 0; j < n_imgs; j++)
        {
            if (j % 1000 == 0) {
                printf("Img No. %d\n", (int)j);
            }
            flattened->img_data = matrix_flatten(imgs[j]->img_data, 0);
            flattened->label = imgs[j]->label;


            output = matrix_create(10, 1);
            output->entries[imgs[j]->label][0] = 1;
            
            network_backpropagate(net, flattened, output);

            matrix_free(flattened->img_data);
            matrix_free(output);
        }
    }
    free(flattened);
    flattened = NULL;
}

void network_train_xor(NeuralNetwork *net, Img** imgs, size_t n_imgs, size_t epoch)
{
    Img* flattened = malloc(sizeof(Img));
    Matrix* output;
    for (size_t i = 0; i < epoch; i++)
    {
        for (size_t j = 0; j < n_imgs; j++)
        {
            flattened->img_data = matrix_flatten(imgs[j]->img_data, 0);
            flattened->label = imgs[j]->label;

            output = matrix_create(1, 1);
            output->entries[0][0] = imgs[j]->label;
            network_backpropagate(net, flattened, output);

            matrix_free(flattened->img_data);
            matrix_free(output);
        }
    }
    free(flattened);
    flattened = NULL;
}

double network_test(NeuralNetwork *net, Img **imgs, size_t n_imgs)
{
    Matrix *prediction;
    double n_correct = 0;
    Img* flattened = malloc(sizeof(Img));

    for (size_t i = 0; i < n_imgs; i++)
    {
        flattened->img_data = matrix_flatten(imgs[i]->img_data, 0);
        flattened->label = imgs[i]->label;
        prediction = network_predict(net, flattened);
        img_print(imgs[i]);
        matrix_print(prediction);
        printf("Prediction is : %d\n", matrix_argmax(prediction));
        if (matrix_argmax(prediction) == imgs[i]->label) { 
            n_correct++;
        }
        matrix_free(prediction);
        matrix_free(flattened->img_data);
    }
    free(flattened);
    flattened = NULL;
    return n_correct / (double)n_imgs;
}

double network_test_xor(NeuralNetwork *net, Img **imgs, size_t n_imgs)
{
    Matrix *prediction;
    double n_correct = 0;
    for (size_t i = 0; i < n_imgs; i++)
    {
        prediction = network_predict(net, imgs[i]);
        if ((prediction->entries[0][0] < 0.5 && imgs[i]->label == 0) || (prediction->entries[0][0] > 0.5 && imgs[i]->label == 1))
        {
            n_correct++;
        }
        matrix_free(prediction);
    }
    return n_correct / (double)n_imgs;
}

void network_backpropagate(NeuralNetwork *net, Img *img, Matrix* output)
{
    
    if (img->img_data->rows != net->input || img->img_data->cols != 1)
    {
        errx(EXIT_FAILURE, "Invalid input.");
    }
    
    Matrix **activations = malloc((net->hidden + 2) * sizeof(Matrix*));
    Matrix **zs = malloc((net->hidden + 1) * sizeof(Matrix*));
    
    Matrix **nabla_b = malloc((net->hidden + 1) * sizeof(Matrix*));
    Matrix **nabla_w = malloc((net->hidden + 1) * sizeof(Matrix*));
    
    Matrix *activation = matrix_copy(img->img_data);
    
    activations[0] = matrix_copy(img->img_data);
    
    Matrix *z;
    Matrix *weighted;
    for (int i = 0; i < net->hidden; i++)
    {
        weighted = dot(net->hidden_weights[i], activation);
        z = add(weighted, net->hidden_bias[i]);
        zs[i] = matrix_copy(z);
        matrix_free(activation);
        activation = apply(sigmoid, z);
        activations[i + 1] = matrix_copy(activation);
        matrix_free(weighted);
        matrix_free(z);
    }
    weighted = dot(net->output_weights, activation);
    z = add(weighted, net->output_bias);
    zs[net->hidden] = matrix_copy(z);
    matrix_free(activation);
    activation = apply(sigmoid, z);
    activations[net->hidden + 1] = matrix_copy(activation);
    
    matrix_free(weighted);
    matrix_free(z);
    matrix_free(activation);
    
    
    
    //Backward pass
    
    Matrix *error = subtract(activations[net->hidden + 1], output);
    Matrix *sig_prime = apply(sigmoid_prime, zs[net->hidden]);
    Matrix *delta = multiply(error, sig_prime);
    
    nabla_b[net->hidden] = matrix_copy(delta);
    
    Matrix *transpo = transpose(activations[net->hidden]);
    Matrix *w_delta = dot(delta, transpo);
    nabla_w[net->hidden] = matrix_copy(w_delta);
    
    Matrix *tmp_delta;
    Matrix *transpo2;
    Matrix *wd;
    for (int i = net->hidden - 1; i >= 0; i--)
    {
        matrix_free(sig_prime);
        matrix_free(transpo);
        matrix_free(w_delta);
        
        z = matrix_copy(zs[i]);
        sig_prime = apply(sigmoid_prime, z);
        if (i == net->hidden - 1)
        {
            transpo2 = transpose(net->output_weights);
        }
        else
        {
            transpo2 = transpose(net->hidden_weights[i + 1]);
        }
        
        tmp_delta = matrix_copy(delta);
        matrix_free(delta);
        wd = dot(transpo2, tmp_delta);
        delta = multiply(wd, sig_prime);
        
        nabla_b[i] = matrix_copy(delta);
        
        transpo = transpose(activations[i]);
        w_delta = dot(delta, transpo);
        nabla_w[i] = matrix_copy(w_delta);
        
        matrix_free(transpo2);
        matrix_free(wd);
        matrix_free(tmp_delta);
        matrix_free(z);
    }
    
    matrix_free(delta);
    matrix_free(error);
    matrix_free(sig_prime);
    matrix_free(transpo);
    matrix_free(w_delta);
    
    Matrix *tmp_b;
    Matrix *scaled_b;
    Matrix *tmp_w;
    Matrix *scaled_w;
    for (int i = 0; i < net->hidden; i++)
    {
        tmp_b = matrix_copy(net->hidden_bias[i]);
        scaled_b = scale(net->learning_rate, nabla_b[i]);
        matrix_free(net->hidden_bias[i]);
        net->hidden_bias[i] = subtract(tmp_b, scaled_b);
        matrix_free(tmp_b);
        matrix_free(scaled_b);
        
        tmp_w = matrix_copy(net->hidden_weights[i]);
        scaled_w = scale(net->learning_rate, nabla_w[i]);
        matrix_free(net->hidden_weights[i]);
        net->hidden_weights[i] = subtract(tmp_w, scaled_w);
        matrix_free(tmp_w);
        matrix_free(scaled_w);
    }
    
    tmp_b = matrix_copy(net->output_bias);
    scaled_b = scale(net->learning_rate, nabla_b[net->hidden]);
    matrix_free(net->output_bias);
    net->output_bias = subtract(tmp_b, scaled_b);
    matrix_free(tmp_b);
    matrix_free(scaled_b);
    
    tmp_w = matrix_copy(net->output_weights);
    scaled_w = scale(net->learning_rate, nabla_w[net->hidden]);
    matrix_free(net->output_weights);
    net->output_weights = subtract(tmp_w, scaled_w);
    matrix_free(tmp_w);
    matrix_free(scaled_w);
    
    //Free
    
    
    for (int i = 0; i < net->hidden + 2; i++)
    {
        matrix_free(activations[i]);
    }
    for (int i = 0; i < net->hidden + 1; i++)
    {
        matrix_free(nabla_w[i]);
        matrix_free(nabla_b[i]);
        matrix_free(zs[i]);
    }
    free(nabla_b);
    free(nabla_w);
    free(zs);
    free(activations);
}

void network_save(NeuralNetwork* net, char* file_string) {
	mkdir(file_string, 0777);
	// Write the descriptor file
	chdir(file_string);
	FILE* descriptor = fopen("descriptor", "w");
	fprintf(descriptor, "%d\n", net->input);
	fprintf(descriptor, "%d\n", net->hidden);
    
    for (int i = 0; i < net->hidden; i++) {
        fprintf(descriptor, "%d\n", net->hiddens[i]);
    }
	
    fprintf(descriptor, "%d\n", net->output);
    fclose(descriptor);
    
    char hidden[12];
    int r;
    for (int i = 0; i < net->hidden; i++) {
        r = snprintf(hidden, sizeof(hidden),"hidden_w%d",i + 1);
        if (r == -1) {
            errx(EXIT_FAILURE, "snprintf()");
        }
        matrix_save(net->hidden_weights[i], (char*)&hidden);
    }
	matrix_save(net->output_weights, "output_w");
    matrix_save(net->output_bias, "output_b");
    
    char hidden1[12];
    for (int i = 0; i < net->hidden; i++) {
        r = snprintf(hidden1, sizeof(hidden1),"hidden_b%d",i + 1);
        if (r == -1) {
            errx(EXIT_FAILURE, "snprintf()");
        }
        matrix_save(net->hidden_bias[i], (char*)&hidden1);
    }
	printf("Successfully written to '%s'\n", file_string);
	chdir("-"); // Go back to the orignal directory
}

NeuralNetwork* network_load(char* file_string) {
	char entry[MAXCHAR];
	chdir(file_string);

	FILE* descriptor = fopen("descriptor", "r");
	//get input number
    if (fgets(entry, MAXCHAR, descriptor) == NULL) {
        errx(EXIT_FAILURE, "fgets()");
    }
	int input = atoi(entry);

    //get hidden layer number
	if (fgets(entry, MAXCHAR, descriptor) == NULL) {
        errx(EXIT_FAILURE, "fgets()");
    }
	int hidden = atoi(entry);

    //get number of neurons per hidden layers
    int hiddens[hidden];
    for (int i = 0; i < hidden; i++) {
        if (fgets(entry, MAXCHAR, descriptor) == NULL) {
            errx(EXIT_FAILURE, "fgets()");
        }
	    hiddens[i] = atoi(entry);
    }

    //get number of output neurons
	if (fgets(entry, MAXCHAR, descriptor) == NULL) {
        errx(EXIT_FAILURE, "fgets()");
    }
	int output = atoi(entry);
	fclose(descriptor);

    char buffer[12];
    int r;
    Matrix **hidden_weights = malloc(hidden * sizeof(Matrix*));
    for (int i = 0; i < hidden; i++) {
        r = snprintf(buffer, sizeof(buffer),"hidden_w%d",i + 1);
        if (r == -1) {
            errx(EXIT_FAILURE, "snprintf()");
        }
        hidden_weights[i] = matrix_load(buffer);
    }

	Matrix* output_weights = matrix_load("output_w");
    Matrix* output_bias = matrix_load("output_b");
    

    char buffer2[12];
    Matrix ** hidden_bias = malloc(hidden * sizeof(Matrix*));
    for (int i = 0; i < hidden; i++) {
        r = snprintf(buffer2, sizeof(buffer2),"hidden_b%d",i + 1);
        if (r == -1) {
            errx(EXIT_FAILURE, "snprintf()");
        }
        hidden_bias[i] = matrix_load(buffer2);
    }
	printf("Successfully loaded network from '%s'\n", file_string);
	chdir("-"); // Go back to the original directory

    NeuralNetwork* net = network_create(input, hidden, hiddens, output, 0);
    
    //free the malloc-ed matrixes because need to overwrite
    for (int i = 0; i < hidden; i++) {
            matrix_free(net->hidden_bias[i]);
            matrix_free(net->hidden_weights[i]);
    }
    
    free(net->hidden_weights);
    free(net->hidden_bias);
    matrix_free(net->output_weights);
    matrix_free(net->output_bias);

    //initializes the remaining needed matrixes
    net->hidden_weights = hidden_weights;
    net->hidden_bias = hidden_bias;
    net->output_bias = output_bias;
    net->output_weights = output_weights;
	return net;
}

void network_print(NeuralNetwork* net) {
	printf("Number of input neurons: %d\n", net->input);
	printf("Number of hidden layers: %d\n", net->hidden);
	for (int i = 0; i < net->hidden; i++)
	{
	    printf("Number of neurons in hidden layer n°%d: %d\n", i, net->hiddens[i]);
	}
	printf("Number of output neurons: %d\n\n", net->output);
	for (int i = 0; i < net->hidden; i++)
	{
	    printf("Weights in hidden layer n°%d:\n", i);
	    matrix_print(net->hidden_weights[i]);
	    printf("Bias in hidden layer n°%d:\n", i);
	    matrix_print(net->hidden_bias[i]);
	}
	printf("Output Weights: \n");
	matrix_print(net->output_weights);
	printf("Output Bias: \n");
	matrix_print(net->output_bias);
}

void network_free(NeuralNetwork *net) {
	matrix_free(net->output_weights);
	matrix_free(net->output_bias);
	for (int i = 0; i < net->hidden; i++)
	{
	    matrix_free(net->hidden_weights[i]);
	    matrix_free(net->hidden_bias[i]);
	}
	free(net->hidden_weights);
	free(net->hidden_bias);
	free(net);
	net = NULL;
}