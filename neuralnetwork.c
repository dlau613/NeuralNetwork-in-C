// //Authors: Daniel Lau and Ben Quachtran
// // #include "L138_LCDK_aic3106_init.h"
// // #include "evmomapl138_gpio.h"

// #include <stdlib.h>
// #include "m_mem.h"
// #include "bmp.h"
// #include <time.h>
// #include <math.h>
// #include <string.h>
// #include <stdio.h>
// #define NUMPAT 4
// #define NUMIN  2
// #define NUMHID 2
// #define NUMOUT 1

// #define TRAIN_INIT 1
// #define TRAIN_CONT 2
// #define IDENT 3

// #define num_input 10000
// #define input_size 28
// #define num_output 14	
// #define NumHidden 700
// #define num_epochs 3

// // #define rando() ((double)rand()/(RAND_MAX+1))
// #define rando() ((double)rand()/(RAND_MAX))
// #define PI 3.1415962



// // interrupt void interrupt4(void) // associated in intvecs.asm with INT4
// // {
// // 	output_left_sample(0);
// // 	return;                       //return from ISR
// // }


// double Input[num_input][input_size][input_size]; 
// double Target[num_input][num_output]; 
		
// int count_digits[num_output];
// int correct_digits[num_output];

// static double SumH[num_input][num_output], WeightIH[NumHidden][input_size][input_size], Hidden[num_input][NumHidden];
// static double SumO[num_input][num_output], WeightHO[NumHidden][num_output], Output[num_input][num_output];
// static double DeltaO[num_output], SumDOW[NumHidden], DeltaH[NumHidden];
// double DeltaWeightIH[NumHidden][input_size][input_size], DeltaWeightHO[NumHidden][num_output];
// double Error =0, eta = 0.01, alpha = 0.1, smallwt = 0.05;	//eta is learning rate,

// int arg_max(double* array);
// void split_bmp(char* file, double first[input_size][input_size], double second[input_size][input_size], double third[input_size][input_size]);
// int identify_image(double image[input_size][input_size], int load_w, char * IH, char * HO);
// void train_network(int cont, char * images, char * labels, char * IH, char * HO);
// void batch_identify(int load_w, char * images, char * labels, char * IH, char * HO, int num_images);
// void init();
// void load_weights(char * IH, char * HO);
// void initialize_weights();
// void print_image(double img[input_size][input_size]);
// void load_image(char * file, double tmp[input_size][input_size]);
// int main(void)
// {
// 	printf("eta: %lf, alpha: %lf, hidden: %d, num_input: %d\n", eta, alpha, NumHidden,num_input);
	
// 	//initializing lots of things to 0 (i think its unnecessary)
// 	init();
// 	// train_network(0,"combined_images.txt", "combined_labels.txt", "Weights/weightsIH_combined.txt", "Weights/weightsHO_combined.txt");
// 	// train_network(0,"signs_images.txt", "signs_labels.txt", "Weights/weightsIH_signs.txt", "Weights/weightsHO_signs.txt");
// 	// batch_identify(1, "mnisttest.txt", "mnisttestlabel.txt", "Weights/weightsIH_digits.txt", "Weights/weightsHO_digits.txt");
// 	// batch_identify(1, "mnisttest.txt", "mnisttestlabel.txt", "Weights/weightsIH_digits.txt", "Weights/weightsHO_digits.txt", 10000);
// 	double first[input_size][input_size], second[input_size][input_size], third[input_size][input_size];
	
// 	split_bmp("/Users/Daniel/Documents/Neural_Network/Numbers/oneplustwo.bmp",first,second,third);	
// 	// print_image(first);
// 	load_weights("Weights/weightsIH_combined.txt", "Weights/weightsHO_combined.txt");
// 	print_image(first);
// 	print_image(second);
// 	print_image(third);

// 	//identif
// 	int a = identify_image(first,1, "Weights/weightsIH_digits.txt", "Weights/weightsHO_digits.txt");
// 	// printf("image is a: %d\n",c);
// 	int b = identify_image(second,1, "Weights/weightsIH_signs.txt", "Weights/weightsHO_signs.txt");
// 	// printf("image is a: %d\n",c);
// 	int c = identify_image(third,1, "Weights/weightsIH_digits.txt", "Weights/weightsHO_digits.txt");
	
// 	char symbol[5];
// 	switch (b) {
// 		case 10:
// 			strcpy(symbol, "+");
// 			break;
// 		case 11:
// 			strcpy(symbol, "-");
// 			break;
// 		case 12:
// 			strcpy(symbol, "/");
// 			break;
// 		case 13:
// 			strcpy(symbol, "x");
// 			break;
// 		default:
// 			strcpy(symbol, "?");
// 			break;
// 	}
// 	printf("%d %s %d\n", a, symbol,c);

	
//  //    fprintf(stdout, "Goodbye!\n") ;
// 	// // L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
// 	// // 	while(1);

//     return 1 ;
// }

// void train_network(int cont, char * images, char * labels, char * IH, char * HO) {
// 	int i,j,k,m,n,op,ip,epoch, num_correct;
// 	int ranpat[num_input];
// 	FILE * fp;
// 	//load training data into Input and load training labels into Target
// 	printf("loading training data\n");
// 	fp = fopen(images, "r");
// 	for (i = 0; i < num_input; i++) 
// 		for (j = 0; j < input_size; j++) 
// 			for (k = 0; k < input_size; k++) 
// 				fscanf(fp, "%lf", &Input[i][j][k]);/////switched and k and j
// 	fclose(fp);
// 	print_image(Input[500]);
// 	print_image(Input[2500]);
// 	print_image(Input[4500]);
// 	print_image(Input[6500]);
// 	fp = fopen(labels, "r");
// 	for (i = 0; i < num_input; i++) 
// 		for (j = 0; j < num_output; j++) 
// 			fscanf(fp, "%lf", &Target[i][j]);
// 	fclose(fp);

// 	//set the weights for initial or continue training
//     printf("initializing weights\n");
//     if (cont == 0) 
//  		initialize_weights();
//     else 
//     	load_weights(IH, HO);

// 	printf("starting epochs\n");
//     for( epoch = 0 ; epoch < num_epochs; epoch++) {    /* iterate weight updates */
//     	// printf("epoch: %d \n",epoch);
//         for( i = 0 ; i < num_input ; i++ )    /* randomize order of individuals */
//             ranpat[i] = i ;
//         for ( i = num_input-1; i > 0; i--) {//knuth shuffle
//         	ip = rand() % (i+1);
//         	op = ranpat[i]; ranpat[i] = ranpat[ip] ; ranpat[ip] = op;
//         }
//         Error = 0;
//         for( ip = 0 ; ip < num_input ; ip++ ) {  // repeat for all the training patterns 
//         	i = ranpat[ip];
//             for( j = 0 ; j < NumHidden ; j++ ) {    /* compute hidden unit activations */
//                 SumH[i][j] = 0;
//                 for( k = 0; k < input_size ; k++ )
//                 	for (m = 0; m < input_size ; m++)
// 	                    SumH[i][j] += Input[i][k][m] * WeightIH[j][k][m] ;
//                 Hidden[i][j] = 1.0/(1.0 + exp(-SumH[i][j])) ;
//             }
//             for( k = 0 ; k < num_output ; k++ ) {    /* compute output unit activations and errors */
//                 SumO[i][k] = 0;
//                 for( j = 0 ; j < NumHidden ; j++ ) 
//                     SumO[i][k] += Hidden[i][j] * WeightHO[j][k] ;
//                 Output[i][k] = 1.0/(1.0 + exp(-SumO[i][k])) ;   /* Sigmoidal Outputs */
//                	// Output[p][k] = SumO[p][k];  //    Linear Outputs
// 	            Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]) ;   /* SSE */
// 	         	// Error -= ( Target[i][k] * log( Output[i][k] ) + ( 1.0 - Target[i][k] ) * log( 1.0 - Output[i][k] ) ) ;  //  Cross-Entropy Error
// 	      		DeltaO[k] = (Target[i][k] - Output[i][k]) * Output[i][k] * (1.0 - Output[i][k]) ;   // Sigmoidal Outputs, SSE  
// 	        	// DeltaO[k] = Target[i][k] - Output[i][k];  //   Sigmoidal Outputs, Cross-Entropy Error 
// 	          	// DeltaO[k] = Target[i][k] - Output[i][k];    // Linear Outputs, SSE
//             }
//             for( j = 0 ; j < NumHidden ; j++ ) {    /* 'back-propagate' errors to hidden layer */
//                	SumDOW[j] = 0.0 ;
//                 for( k = 0 ; k < num_output ; k++ )
//                     SumDOW[j] += WeightHO[j][k] * DeltaO[k] ;
//                 DeltaH[j] = SumDOW[j] * Hidden[i][j] * (1.0 - Hidden[i][j]) ;
//             }
//             for( j = 0 ; j < NumHidden ; j++ ) {     /* update weights WeightIH */
//                 for( m = 0 ; m < input_size ; m++ ) 
//                 	for (n = 0; n < input_size ; n++)
//                 	{
// 	                    DeltaWeightIH[j][m][n] = eta * Input[i][m][n] * DeltaH[j] + alpha * DeltaWeightIH[j][m][n];
// 	                    WeightIH[j][m][n] += DeltaWeightIH[j][m][n];
//                 	}
//             }
//             for( j = 0 ; j < NumHidden ; j ++ ) {    /* update weights WeightHO */
//                 for( k = 0 ; k < num_output ; k++ ) {
//                     DeltaWeightHO[j][k] = eta * Hidden[i][j] * DeltaO[k] + alpha * DeltaWeightHO[j][k] ;
//                     WeightHO[j][k] += DeltaWeightHO[j][k] ;
//                 }
//             }
//         }
//         num_correct = 0;
// 		for (i = 0; i < num_input; i++) {
// 			j = arg_max(Target[i]); 
// 			k = arg_max(Output[i]);
// 			if (j == k)	
// 				num_correct++;
// 		} 
//         // if( epoch%100 == 0 ) 
//         	fprintf(stdout, "Epoch %-5d :   Error = %lf , Percent Correct: %lf\n", epoch, Error,(double)((double)num_correct*100/(double)num_input));
//         if( Error < 0.0004 ) 
//         	break ;  /* stop learning when 'near enough' */
//     	if (eta > 0.000001)
// 			eta = eta *.5;
//     }

//     //write weights input->hidden
//     fp = fopen(IH, "w+");
// 	for (i = 0; i < NumHidden; i++) {
// 		for (j = 0; j < input_size; j++) 
// 			for (k = 0; k < input_size; k++)
// 				fprintf(fp, "%lf ", WeightIH[i][j][k]);
// 		fprintf(fp, " \n");
// 	}
// 	fclose(fp);

// 	//write weights hidden -> output
// 	fp = fopen(HO, "w+");
// 	for (i = 0; i < NumHidden; i++) {
// 		for (j = 0; j < num_output; j++) 
// 			fprintf(fp, "%lf ", WeightHO[i][j]);
// 		fprintf(fp," \n");
// 	}
// 	fclose(fp);

// 	fp = fopen("eta.txt", "w+"); //write eta to file
// 	fprintf(fp, "%lf", eta);
// 	fclose(fp);  

//    num_correct = 0;
//     for (i = 0; i < num_input; i++) {
// 		j = arg_max(Target[i]); 
// 		k = arg_max(Output[i]);
// 		count_digits[k]++;
// 		 // printf ("j: %d, k: %d\n",j,k);
// 		if (j == k)	{
// 			correct_digits[k]++;
// 			num_correct++;
// 		}

// 	}
// 	for (i = 0; i < num_output - 4; i++) {
// 		printf("percent correct for %d: %d / %d = %.2f \n", i,correct_digits[i],count_digits[i], (double)correct_digits[i]/count_digits[i]);
// 	}
// 	printf("percent correct: %lf error: %lf\n", (double)((double)num_correct*100/(double)num_input),Error);	 	
// }
// int arg_max(double* array) {
// 	int arg = 0;
// 	int i = 0;
// 	double current_max = 0;
// 	for (i = 0; i < num_output; i ++) {
// 		if (array[i] > current_max) {
// 			current_max = array[i];
// 			arg = i;
// 		}
// 	}
// 	return arg;
// }

// void split_bmp(char* file, double first[input_size][input_size], double second[input_size][input_size], double third[input_size][input_size]) { 
// 	int i,j,k;
// 	unsigned char *bitmap = imread(file);
// 	// printf("splitting bmp\n");
// 	for (j = 0; j < input_size; j++) {
// 		for (k = 0; k < input_size*3; k++) {
// 			if (k < input_size) {
// 				first[input_size - 1 -j][k % input_size] = bitmap[3*(j*input_size*3 +k)];
// 			}
// 			else if (k < input_size * 2) {
// 				second[input_size - 1 -j][k % input_size] = bitmap[3*(j*input_size*3 +k)];
// 			}
// 			else
// 				third[input_size - 1 -j][k % input_size] = bitmap[3*(j*input_size*3 +k)];
// 		}
// 	}	
// }

// int identify_image(double image[input_size][input_size], int load_w, char * IH, char * HO) {
// 	int j,k,m;
// 	double sum_hidden[NumHidden] = {0};
// 	double sum_output[num_output] = {0};
// 	double img_output[num_output] = {0};
// 	double img_hidden[NumHidden] = {0};
// 	if (load_w) 
// 		load_weights(IH, HO);
// 	// print_image(image);
//     for( j = 0 ; j < NumHidden ; j++ ) {    // compute hidden unit activations 
//        sum_hidden[j] = 0 ;
//         for( k = 0; k < input_size ; k++ ) {
//         	for (m = 0; m < input_size ; m++){
//                 // sum_hidden[j] += Input[i][k][m] * WeightIH[j][k][m];
//                 sum_hidden[j] += image[k][m] * WeightIH[j][k][m];
//         	}
//         }
//         img_hidden[j] = 1.0/(1.0 + exp(-sum_hidden[j])) ; 
//     }
//     for( k = 0 ; k < num_output ; k++ ) {     //compute output unit activations and errors 
//         sum_output[k] = 0 ;
//         for( j = 0 ; j < NumHidden ; j++ ) {
//             sum_output[k] += img_hidden[j] * WeightHO[j][k];
//         }
//         img_output[k] = 1.0/(1.0 + exp(-sum_output[k]));   //Sigmoidal Outputs 
//        // Output[p][k] = SumO[p][k];  //    Linear Outputs
//         // Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]); 
//         // printf("%.2lf \n", Error);
//     }
// 	return arg_max(img_output);
// }

// void batch_identify(int load_w, char * images, char * labels, char * IH, char * HO, int num_images) {
// 	int correct = 0;
// 	int i, j, k;
// 	FILE * fp;
// 	if (load_w) {
// 		load_weights(IH, HO);
// 	}
// 	fp = fopen(images, "r");
// 	for (i = 0; i < num_images; i++) 
// 		for (j = 0; j < input_size; j++) 
// 			for (k = 0; k < input_size; k++) 
// 				fscanf(fp, "%lf", &Input[i][j][k]);/////switched and k and j
// 	fclose(fp);

// 	fp = fopen(labels, "r");
// 	for (i = 0; i < num_images; i++) 
// 		for (j = 0; j < num_output; j++) 
// 			fscanf(fp, "%lf", &Target[i][j]);

// 	for (i = 0; i < num_images;i++) {
// 		if (arg_max(Target[i]) == identify_image(Input[i], 0, NULL, NULL))
// 			correct++;
// 	}
// 	printf("percent correct: %lf\n", (double)((double)correct*100/(double)num_input));
// }

// void load_weights(char * IH, char * HO) {
// 	int i,j,k;
// 	FILE * fp;
// 	printf("loading weights\n");
// 	fp = fopen(IH, "r");
// 	for (i = 0; i < NumHidden; i++) 
// 		for (j = 0; j < input_size; j++) 
// 			for (k = 0; k < input_size; k++)
// 				fscanf(fp,"%lf", &WeightIH[i][j][k]);//read in weightsIH
// 	fclose(fp);

// 	fp = fopen(HO, "r");
// 	for (i = 0; i < NumHidden; i++) 
// 		for (j = 0; j < num_output; j++) 
// 			fscanf(fp, "%lf", &WeightHO[i][j]); //read in weightsOH
// 	fclose(fp);

// 	fp = fopen("eta.txt", "r");
// 	fscanf(fp, "%lf", &eta);
// 	fclose(fp);
// 	printf("done loading weights\n");
// }
// void initialize_weights() {
// 	int i, j, k;
//     for( i = 0 ; i < NumHidden ; i++ )    /* initialize WeightIH and DeltaWeightIH */
//         for( j = 0 ; j < input_size ; j++ ) 
//         	for (k = 0; k < input_size ; k++)
//         	{
// 	            DeltaWeightIH[i][j][k] = 0.0 ;
// 	            WeightIH[i][j][k] = 2.0 * ( rando() - 0.5 ) * smallwt ;
//         	}
//     for( i = 0 ; i < NumHidden ; i++ ) {   /* initialize WeightHO and DeltaWeightHO */
//         for( j = 0 ; j < num_output ; j++ ) {
//             DeltaWeightHO[i][j] = 0.0 ;
//             WeightHO[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
//     	}
//     }	
// }
// void init() {
// 	int i, j, k;
// 	for (i = 0; i < NumHidden; i++) {
// 		DeltaH[i] = 0;
// 	}
// 	for (i = 0; i < num_output; i++) {
// 		DeltaO[i] = 0;
// 		count_digits[i] = 0;
// 		correct_digits[i] = 0;
// 	}
// 	for (i = 0; i < num_input; i++) {
//         for( j = 0 ; j < input_size ; j++ ) 
//         	for (k = 0; k < input_size ; k++)
// 	            DeltaWeightIH[i][j][k] = 0.0 ;  
// 	    for (j = 0; j < num_output; j++) {
// 	    	SumO[i][j] = 0;
// 	    	SumH[i][j] = 0;  
// 	    }
// 	}
//     for( i = 0 ; i < NumHidden ; i++ )   
//         for( j = 0 ; j < num_output ; j++ ) 
//             DeltaWeightHO[i][j] = 0.0;
// }

// void print_image(double image[input_size][input_size]) {
// 	int j, k;
// 	for (j = 0; j < input_size; j++) {
// 		for (k = 0; k < input_size; k++) {
// 			if (image[j][k] > 1)
// 				printf("1 ");
// 			else
// 				printf("0 ");
// 		}
// 		printf("\n");
// 	}
// 	printf("\n");
// }

// void load_image(char * file, double tmp[input_size][input_size]) {
// 	int j,k;
// 	unsigned char * bitmap = imread(file); 
// 	for (j = 0; j < input_size; j++) 
// 		for (k = 0; k < input_size; k++) 
// 			tmp[input_size - 1 - j][k] = bitmap[3*(j*input_size + k)];	// flip along diagonal going from top left to bottom right
// }



//Authors: Daniel Lau and Ben Quachtran
// #include "L138_LCDK_aic3106_init.h"
// #include "evmomapl138_gpio.h"

#include <stdlib.h>
#include "m_mem.h"
#include "bmp.h"
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#define NUMPAT 4
#define NUMIN  2
#define NUMHID 2
#define NUMOUT 1

#define TRAIN_INIT 1
#define TRAIN_CONT 2
#define IDENT 3

#define num_input 24000
#define input_size 28
#define num_output 14
#define NumHidden 800
#define num_epochs 5



// #define rando() ((double)rand()/(RAND_MAX+1))
#define rando() ((double)rand()/(RAND_MAX))
#define PI 3.1415962



 // interrupt void interrupt4(void) // associated in intvecs.asm with INT4
 // {
 // 	output_left_sample(0);
 // 	return;                       //return from ISR
 // }


double Input[num_input][input_size][input_size];
// #pragma DATA_SECTION(Input,".EXT_RAM");

double Target[num_input][num_output];
// #pragma DATA_SECTION(Target,".EXT_RAM");

int count_digits[num_output];
int correct_digits[num_output];


static double SumH[num_input][num_output];
// #pragma DATA_SECTION(SumH,".EXT_RAM");

static double WeightIH[NumHidden][input_size][input_size];
// #pragma DATA_SECTION(WeightIH,".EXT_RAM");

static double Hidden[num_input][NumHidden];
// #pragma DATA_SECTION(Hidden,".EXT_RAM");


static double SumO[num_input][num_output];
// #pragma DATA_SECTION(SumO,".EXT_RAM");

static double WeightHO[NumHidden][num_output];
// #pragma DATA_SECTION(WeightHO,".EXT_RAM");

static double Output[num_input][num_output];
// #pragma DATA_SECTION(Output,".EXT_RAM");


static double DeltaO[num_output], SumDOW[NumHidden], DeltaH[NumHidden];

double DeltaWeightIH[NumHidden][input_size][input_size];
// #pragma DATA_SECTION(DeltaWeightIH,".EXT_RAM");

double DeltaWeightHO[NumHidden][num_output];
// #pragma DATA_SECTION(DeltaWeightHO,".EXT_RAM");

double Error =0, eta = 0.01, alpha = 0.1, smallwt = 0.05;	//eta is learning rate,

int arg_max(double* array);
void split_bmp(char* file, double first[input_size][input_size], double second[input_size][input_size], double third[input_size][input_size]);
int identify_image(double image[input_size][input_size], int load_w, char * IH, char * HO);
void train_network(int cont, char * images, char * labels, char * IH, char * HO);
void batch_identify(int load_w, char * images, char * labels, char * IH, char * HO, int num_images);
void init();
void load_weights(char * IH, char * HO);
void initialize_weights();
void print_image(double img[input_size][input_size]);
void load_image(char * file, double tmp[input_size][input_size]);

int main(void)
{
	printf("eta: %lf, alpha: %lf, hidden: %d, num_input: %d\n", eta, alpha, NumHidden,num_input);
	//initializing lots of things to 0 (i think its unnecessary)
	init();

	char input[256];
	while (1) {
		strcpy(input, "");

		printf("Type help for command list\n");
		printf("Enter a command: ");
		fgets(input,sizeof(input) ,stdin);
		input[strcspn(input, "\n")] = 0;
		if (strncmp(input,"help", 5) == 0)
		{
			printf("load weights\ntrain\nrecognize one\nrecognize three\nexit\n");
		}
		else if (!strncmp(input, "load weights", 13)) {
			load_weights("Weights/weightsIH_combined.txt","Weights/weightsHO_combined.txt");
			//	load_weights("C:\\Users\\EE113D\\MyProjects\\Jerry_lab1\\Weights\\weightsIH_combined.txt","C:\\Users\\EE113D\\MyProjects\\Jerry_lab1\\Weights\\weightsHO_combined.txt");
		}
		else if (!strncmp(input, "train",6)) {
				train_network(0,"combined_images.txt", "combined_labels.txt", "Weights/weightsIH_combined.txt", "Weights/weightsHO_combined.txt");
		}
		else if (!strncmp(input, "recognize one", 14))
		{
			// static double image[input_size][input_size];
			static double image[input_size][input_size];
			printf("Enter file name: ");
			strcpy(input,"");
			fgets(input, sizeof(input),stdin);
			input[strcspn(input, "\n")] = 0;
			char filepath[256];
			strcpy(filepath, "");
			// strcpy(filepath, "C:\\Users\\EE113D\\MyProjects\\Jerry_lab1\\");
			strcpy(filepath, "/Users/Daniel/Documents/Neural_Network/Numbers/");
			strcat(filepath, input);
			load_image(filepath, image);
			// load_image("/Users/Daniel/Documents/Neural_Network/Numbers/four.bmp",image);
			print_image(image);
			int a = identify_image(image,0, "/Jerry_lab1/Weights/weightsIH_combined.txt", "/Jerry_lab1/Weights/weightsHO_combined.txt");
			char symbol[5];
			switch (a) {
				case 10:
					printf("+\n");
//					strcpy(symbol, "+");
					break;
				case 11:
					printf("-\n");
//					strcpy(symbol, "-");
					break;
				case 12:
					printf("/\n");
//					strcpy(symbol, "/");
					break;
				case 13:
					printf("x\n");
//					strcpy(symbol, "x");
					break;
				default:
					printf("%d\n",a);
					break;
			}
		}
		else if (!strncmp(input, "recognize three",16)) {
			static double first[input_size][input_size], second[input_size][input_size], third[input_size][input_size];
			printf("Enter file name: ");
			fgets(input, sizeof(input),stdin);
			input[strcspn(input, "\n")] = 0;
			char filepath[256];
			// strcpy(filepath, "C:\\Users\\EE113D\\MyProjects\\Jerry_lab1\\");
			strcpy(filepath, "/Users/Daniel/Documents/Neural_Network/Numbers/");
			strcat(filepath, input);
			printf("%s\n", filepath);
			split_bmp(filepath, first,second,third);
			print_image(first);
			print_image(second);
			print_image(third);
			int a = identify_image(first,0, "/Jerry_lab1/Weights/weightsIH_digits.txt", "/Jerry_lab1/Weights/weightsHO_digits.txt");
			int b = identify_image(second,0, "/Jerry_lab1/Weights/weightsIH_digits.txt", "/Jerry_lab1/Weights/weightsHO_digits.txt");
			int c = identify_image(third,0, "/Jerry_lab1/Weights/weightsIH_digits.txt", "/Jerry_lab1/Weights/weightsHO_digits.txt");
			char symbol[5];
			switch (b) {
				case 10:
					strcpy(symbol, "+");
					break;
				case 11:
					strcpy(symbol, "-");
					break;
				case 12:
					strcpy(symbol, "/");
					break;
				case 13:
					strcpy(symbol, "x");
					break;
				default:
					strcpy(symbol, "?");
					break;
			}
			printf("%d %s %d\n", a, symbol,c);
		}
		else if (!strncmp(input, "exit",5)) {
			break;
		}
		else
		{
			printf("Command not recognized\n");
		}
	}
//	printf("initialize done\n");
	// train_network(0,"combined_images.txt", "combined_labels.txt", "Weights/weightsIH_combined.txt", "Weights/weightsHO_combined.txt");
	// train_network(0,"signs_images.txt", "signs_labels.txt", "Weights/weightsIH_signs.txt", "Weights/weightsHO_signs.txt");
	// batch_identify(1, "mnisttest.txt", "mnisttestlabel.txt", "Weights/weightsIH_digits.txt", "Weights/weightsHO_digits.txt");
	// batch_identify(1, "mnisttest.txt", "mnisttestlabel.txt", "Weights/weightsIH_digits.txt", "Weights/weightsHO_digits.txt", 10000);


     fprintf(stdout, "Goodbye!\n") ;
	  // L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	  // 	while(1);

    return 1;
}




void train_network(int cont, char * images, char * labels, char * IH, char * HO) {
	int i,j,k,m,n,op,ip,epoch, num_correct;
	int ranpat[num_input];
	FILE * fp;
	//load training data into Input and load training labels into Target
	printf("loading training data\n");
	fp = fopen(images, "r");
	for (i = 0; i < num_input; i++)
		for (j = 0; j < input_size; j++)
			for (k = 0; k < input_size; k++)
				fscanf(fp, "%lf", &Input[i][j][k]);/////switched and k and j
	fclose(fp);
//	print_image(Input[500]);
//	print_image(Input[2500]);
//	print_image(Input[4500]);
//	print_image(Input[6500]);
	fp = fopen(labels, "r");
	for (i = 0; i < num_input; i++)
		for (j = 0; j < num_output; j++)
			fscanf(fp, "%lf", &Target[i][j]);
	fclose(fp);

	//set the weights for initial or continue training
    printf("initializing weights\n");
    if (cont == 0)
 		initialize_weights();
    else
    	load_weights(IH, HO);

	printf("starting epochs\n");
    for( epoch = 0 ; epoch < num_epochs; epoch++) {    /* iterate weight updates */
    	// printf("epoch: %d \n",epoch);
        for( i = 0 ; i < num_input ; i++ )    /* randomize order of individuals */
            ranpat[i] = i ;
        for ( i = num_input-1; i > 0; i--) {//knuth shuffle
        	ip = rand() % (i+1);
        	op = ranpat[i]; ranpat[i] = ranpat[ip] ; ranpat[ip] = op;
        }
        Error = 0;
        for( ip = 0 ; ip < num_input ; ip++ ) {  // repeat for all the training patterns
        	i = ranpat[ip];
            for( j = 0 ; j < NumHidden ; j++ ) {    /* compute hidden unit activations */
                SumH[i][j] = 0;
                for( k = 0; k < input_size ; k++ )
                	for (m = 0; m < input_size ; m++)
	                    SumH[i][j] += Input[i][k][m] * WeightIH[j][k][m] ;
                Hidden[i][j] = 1.0/(1.0 + exp(-SumH[i][j])) ;
            }
            for( k = 0 ; k < num_output ; k++ ) {    /* compute output unit activations and errors */
                SumO[i][k] = 0;
                for( j = 0 ; j < NumHidden ; j++ )
                    SumO[i][k] += Hidden[i][j] * WeightHO[j][k] ;
                Output[i][k] = 1.0/(1.0 + exp(-SumO[i][k])) ;   /* Sigmoidal Outputs */
               	// Output[p][k] = SumO[p][k];  //    Linear Outputs
	            Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]) ;   /* SSE */
	         	// Error -= ( Target[i][k] * log( Output[i][k] ) + ( 1.0 - Target[i][k] ) * log( 1.0 - Output[i][k] ) ) ;  //  Cross-Entropy Error
	      		DeltaO[k] = (Target[i][k] - Output[i][k]) * Output[i][k] * (1.0 - Output[i][k]) ;   // Sigmoidal Outputs, SSE
	        	// DeltaO[k] = Target[i][k] - Output[i][k];  //   Sigmoidal Outputs, Cross-Entropy Error
	          	// DeltaO[k] = Target[i][k] - Output[i][k];    // Linear Outputs, SSE
            }
            for( j = 0 ; j < NumHidden ; j++ ) {    /* 'back-propagate' errors to hidden layer */
               	SumDOW[j] = 0.0 ;
                for( k = 0 ; k < num_output ; k++ )
                    SumDOW[j] += WeightHO[j][k] * DeltaO[k] ;
                DeltaH[j] = SumDOW[j] * Hidden[i][j] * (1.0 - Hidden[i][j]) ;
            }
            for( j = 0 ; j < NumHidden ; j++ ) {     /* update weights WeightIH */
                for( m = 0 ; m < input_size ; m++ )
                	for (n = 0; n < input_size ; n++)
                	{
	                    DeltaWeightIH[j][m][n] = eta * Input[i][m][n] * DeltaH[j] + alpha * DeltaWeightIH[j][m][n];
	                    WeightIH[j][m][n] += DeltaWeightIH[j][m][n];
                	}
            }
            for( j = 0 ; j < NumHidden ; j ++ ) {    /* update weights WeightHO */
                for( k = 0 ; k < num_output ; k++ ) {
                    DeltaWeightHO[j][k] = eta * Hidden[i][j] * DeltaO[k] + alpha * DeltaWeightHO[j][k] ;
                    WeightHO[j][k] += DeltaWeightHO[j][k] ;
                }
            }
        }
        num_correct = 0;
		for (i = 0; i < num_input; i++) {
			j = arg_max(Target[i]);
			k = arg_max(Output[i]);
			if (j == k)
				num_correct++;
		}
        // if( epoch%100 == 0 )
        	fprintf(stdout, "Epoch %-5d :   Error = %lf , Percent Correct: %lf\n", epoch, Error,(double)((double)num_correct*100/(double)num_input));
        if( Error < 0.0004 )
        	break ;  /* stop learning when 'near enough' */
    	if (eta > 0.000001)
			eta = eta *.5;
    }

    //write weights input->hidden
    fp = fopen(IH, "w+");
	for (i = 0; i < NumHidden; i++) {
		for (j = 0; j < input_size; j++)
			for (k = 0; k < input_size; k++)
				fprintf(fp, "%lf ", WeightIH[i][j][k]);
		fprintf(fp, " \n");
	}
	fclose(fp);

	//write weights hidden -> output
	fp = fopen(HO, "w+");
	for (i = 0; i < NumHidden; i++) {
		for (j = 0; j < num_output; j++)
			fprintf(fp, "%lf ", WeightHO[i][j]);
		fprintf(fp," \n");
	}
	fclose(fp);

	fp = fopen("eta.txt", "w+"); //write eta to file
	fprintf(fp, "%lf", eta);
	fclose(fp);

   num_correct = 0;
    for (i = 0; i < num_input; i++) {
		j = arg_max(Target[i]);
		k = arg_max(Output[i]);
		count_digits[k]++;
		 // printf ("j: %d, k: %d\n",j,k);
		if (j == k)	{
			correct_digits[k]++;
			num_correct++;
		}

	}
	for (i = 0; i < num_output - 4; i++) {
		printf("percent correct for %d: %d / %d = %.2f \n", i,correct_digits[i],count_digits[i], (double)correct_digits[i]/count_digits[i]);
	}
	printf("percent correct: %lf error: %lf\n", (double)((double)num_correct*100/(double)num_input),Error);
}
int arg_max(double* array) {
	int arg = 0;
	int i = 0;
	double current_max = 0;
	for (i = 0; i < num_output; i ++) {
		if (array[i] > current_max) {
			current_max = array[i];
			arg = i;
		}
	}
	return arg;
}

void split_bmp(char* file, double first[input_size][input_size], double second[input_size][input_size], double third[input_size][input_size]) {
	int j,k;
	unsigned char *bitmap = imread(file);
	// printf("splitting bmp\n");
	for (j = 0; j < input_size; j++) {
		//if (j==9)
		//	print_image(first);
		//printf("j is %d\n",j);

		for (k = 0; k < input_size*3; k++) {
		//	if (j>8)
			//	printf("k is %d\n",k);

			if (k < input_size) {
				first[input_size - 1 -j][k % input_size] = bitmap[3*(j*input_size*3 +k)];
			}
			else if (k < input_size * 2) {
				second[input_size - 1 -j][k % input_size] = bitmap[3*(j*input_size*3 +k)];
			}
			else
				third[input_size - 1 -j][k % input_size] = bitmap[3*(j*input_size*3 +k)];
		}
	}
}

int identify_image(double image[input_size][input_size], int load_w, char * IH, char * HO) {
	int j,k,m;
	static double sum_hidden[NumHidden] = {0};
	static double sum_output[num_output] = {0};
	static double img_output[num_output] = {0};
	static double img_hidden[NumHidden] = {0};
	if (load_w)
		load_weights(IH, HO);
	// print_image(image);
    for( j = 0 ; j < NumHidden ; j++ ) {    // compute hidden unit activations
       sum_hidden[j] = 0 ;
        for( k = 0; k < input_size ; k++ ) {
        	for (m = 0; m < input_size ; m++){
                // sum_hidden[j] += Input[i][k][m] * WeightIH[j][k][m];
                sum_hidden[j] += image[k][m] * WeightIH[j][k][m];
        	}
        }
        img_hidden[j] = 1.0/(1.0 + exp(-sum_hidden[j])) ;
    }
    for( k = 0 ; k < num_output ; k++ ) {     //compute output unit activations and errors
        sum_output[k] = 0 ;
        for( j = 0 ; j < NumHidden ; j++ ) {
            sum_output[k] += img_hidden[j] * WeightHO[j][k];
        }
        img_output[k] = 1.0/(1.0 + exp(-sum_output[k]));   //Sigmoidal Outputs
       // Output[p][k] = SumO[p][k];  //    Linear Outputs
        // Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]);
        // printf("%.2lf \n", Error);
    }
	return arg_max(img_output);
}

void batch_identify(int load_w, char * images, char * labels, char * IH, char * HO, int num_images) {
	int correct = 0;
	int i, j, k;
	FILE * fp;
	if (load_w) {
		load_weights(IH, HO);
	}
	fp = fopen(images, "r");
	for (i = 0; i < num_images; i++)
		for (j = 0; j < input_size; j++)
			for (k = 0; k < input_size; k++)
				fscanf(fp, "%lf", &Input[i][j][k]);/////switched and k and j
	fclose(fp);

	fp = fopen(labels, "r");
	for (i = 0; i < num_images; i++)
		for (j = 0; j < num_output; j++)
			fscanf(fp, "%lf", &Target[i][j]);

	for (i = 0; i < num_images;i++) {
		if (arg_max(Target[i]) == identify_image(Input[i], 0, NULL, NULL))
			correct++;
	}
	printf("percent correct: %lf\n", (double)((double)correct*100/(double)num_input));
}

void load_weights(char * IH, char * HO) {
	int i,j,k;
	FILE * fp;
	char buf[20];
	printf("loading weights...\n");
	fp = fopen(IH, "r");
	for (i = 0; i < NumHidden; i++)
		for (j = 0; j < input_size; j++)
			for (k = 0; k < input_size; k++) {
				fscanf(fp,"%lf", &WeightIH[i][j][k]);//read in weightsIH
//				if (i == 0 && j == 0 && k == 0)
//					fscanf(fp, "%s", buf);
			}
	fclose(fp);
	//printf("%s\n",buf);
	//printf("------  %lf  -------\n", WeightIH[0][0][0]);

	fp = fopen(HO, "r");
	for (i = 0; i < NumHidden; i++)
		for (j = 0; j < num_output; j++)
			fscanf(fp, "%lf", &WeightHO[i][j]); //read in weightsOH
	fclose(fp);

	fp = fopen("eta.txt", "r");
	fscanf(fp, "%lf", &eta);
	fclose(fp);
	printf("done loading weights\n");
}
void initialize_weights() {
	int i, j, k;
    for( i = 0 ; i < NumHidden ; i++ )    /* initialize WeightIH and DeltaWeightIH */
        for( j = 0 ; j < input_size ; j++ )
        	for (k = 0; k < input_size ; k++)
        	{
	            DeltaWeightIH[i][j][k] = 0.0 ;
	            WeightIH[i][j][k] = 2.0 * ( rando() - 0.5 ) * smallwt ;
        	}
    for( i = 0 ; i < NumHidden ; i++ ) {   /* initialize WeightHO and DeltaWeightHO */
        for( j = 0 ; j < num_output ; j++ ) {
            DeltaWeightHO[i][j] = 0.0 ;
            WeightHO[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
    	}
    }
}
void init() {
	int i, j, k;
	for (i = 0; i < NumHidden; i++) {
		DeltaH[i] = 0;
	}
	for (i = 0; i < num_output; i++) {
		DeltaO[i] = 0;
		count_digits[i] = 0;
		correct_digits[i] = 0;
	}
	for (i = 0; i < num_input; i++) {
        for( j = 0 ; j < input_size ; j++ )
        	for (k = 0; k < input_size ; k++)
	            DeltaWeightIH[i][j][k] = 0.0 ;
	    for (j = 0; j < num_output; j++) {
	    	SumO[i][j] = 0;
	    	SumH[i][j] = 0;
	    }
	}
    for( i = 0 ; i < NumHidden ; i++ )
        for( j = 0 ; j < num_output ; j++ )
            DeltaWeightHO[i][j] = 0.0;
}

void print_image(double image[input_size][input_size]) {
	int j, k;
	for (j = 0; j < input_size; j++) {
		for (k = 0; k < input_size; k++) {
			if (image[j][k] > 1)
				printf("1 ");
			else
				printf("0 ");
		}
		printf("\n");
	}
	printf("\n");
}

void load_image(char * file, double tmp[input_size][input_size]) {
	int j,k;
	unsigned char * bitmap = imread(file);
	for (j = 0; j < input_size; j++)
		for (k = 0; k < input_size; k++)
			tmp[input_size - 1 - j][k] = bitmap[3*(j*input_size + k)];	// flip along diagonal going from top left to bottom right
}










