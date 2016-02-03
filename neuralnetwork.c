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

#define num_input 60000
#define input_size 28
#define num_output 14	
#define NumHidden 300

// #define rando() ((double)rand()/(RAND_MAX+1))
#define rando() ((double)rand()/(RAND_MAX))
#define PI 3.1415962



// interrupt void interrupt4(void) // associated in intvecs.asm with INT4
// {
// 	output_left_sample(0);
// 	return;                       //return from ISR
// }

int    i, j, k, p, m, n, np, op, ip, ranpat[num_input], epoch, num_correct;
int    NumPattern = num_input, NumInput = input_size*input_size, NumOutput = num_output;

static double Input[num_input][input_size][input_size]; 
static double Target[num_input][num_output]; 
		
static double SumH[num_input][num_output], WeightIH[NumHidden][input_size][input_size], Hidden[num_input][NumHidden];
static double SumO[num_input][num_output], WeightHO[NumHidden][num_output], Output[num_input][num_output];
static double DeltaO[num_output], SumDOW[NumHidden], DeltaH[NumHidden];
static double DeltaWeightIH[NumHidden][input_size][input_size], DeltaWeightHO[NumHidden][num_output];
double Error =0, eta = 0.1, alpha = 0.1, smallwt = 0.05;	//eta is learning rate,

FILE * fp;

unsigned char * bitmap;

static int display = 0, input_image = 1;
static int mode = TRAIN_INIT; //TRAIN_INIT, TRAIN_CONT, IDENT
int arg_max(double* array);
int main(void)
{
	printf("mode: %d, eta: %lf, alpha: %lf, hidden: %d, display: %d\n", mode, eta, alpha, NumHidden, display);

	//initializing lots of things to 0 (i think its unnecessary)
	for (i = 0; i < NumHidden; i++) {
		DeltaH[i] = 0;
	}
	for (i = 0; i < num_output; i++) {
		DeltaO[i] = 0;
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

	//LOAD INPUT ARRAY WITH EITHER TRAINING DATA OR DATA TO IDENTIFY
	if (mode != IDENT) {//not IDENT means its training
		fp = fopen("mnistimage.txt", "r");
		for (i = 0; i < num_input; i++) 
			for (j = 0; j < input_size; j++) 
				for (k = 0; k < input_size; k++) 
					fscanf(fp, "%lf", &Input[i][j][k]);/////switched and k and j
		fclose(fp);

		fp = fopen("mnistlabel.txt", "r");
		for (i = 0; i < num_input; i++) 
			for (j = 0; j < num_output; j++) 
				fscanf(fp, "%lf", &Target[i][j]);

		for (j = 0; j < input_size; j++) {//for testing, when we iterate through this way we want it to print normally
			for (k = 0; k < input_size; k++) {
				//printf("%.0lf ", Input[4050][j][k]);
				 if (Input[2][j][k] > 1 ) 
				 	printf("1 ");
				 else
				 	printf("0 ");
			}
			printf(" \n");
		}
		fclose(fp);

	}

	if (mode == IDENT) {//load the image into input
		if (input_image == 1) {
			bitmap = imread("/Users/Daniel/Documents/Neural_Network/zero.bmp");
			for (i = 0; i < num_input; i++) 
				for (j = 0; j < input_size; j++) 
					for (k = 0; k < input_size; k++) 
						Input[i][input_size - j][k] = bitmap[3*(j*input_size + k)];	// flip along diagonal going from top left to bottom right

			for (i = 0; i < num_input; i++) {//if we iterate through in this order we want it to print it normally
				for (j = 0; j < input_size; j++) {
					for (k = 0; k < input_size; k++) {
						// printf("%.0lf ", Input[i][j][k]);
						if (Input[i][j][k] > 1)
							printf("1 ");
						else 
							printf("0 ");
					}
					printf("\n"); 
				}
			}
			printf("here\n");
			for (i = 0; i < num_output; i++) 
				Target[0][i] = 0;
			Target[0][6] = 1; 
			for (i = 0; i < num_output; i++ ) {
				printf("%.2lf ", Target[0][i]);
			}
			printf("\n");
		}
		else {
			fp = fopen("training_digits.txt", "r");
			for (i = 0; i < num_input; i++) 
				for (j = 0; j < input_size; j++) 
					for (k = 0; k < input_size; k++) 
						fscanf(fp, "%lf", &Input[i][k][j]);/////switched and k and j
			fclose(fp);

			fp = fopen("training_labels.txt", "r");
			for (i = 0; i < num_input; i++) 
				for (j = 0; j < num_output; j++) 
					fscanf(fp, "%lf", &Target[i][j]);
		}
	}

	/////////if not doing initial training, load in the weights and eta
	if (mode != TRAIN_INIT) {
		printf("begin recognition\n");
		fp = fopen("weightsIH.txt", "r");
		for (i = 0; i < NumHidden; i++) 
			for (j = 0; j < input_size; j++) 
				for (k = 0; k < input_size; k++)
					fscanf(fp,"%lf", &WeightIH[i][j][k]);//read in weightsIH
		fclose(fp);

		fp = fopen("weightsHO.txt", "r");
		for (i = 0; i < NumHidden; i++) 
			for (j = 0; j < num_output; j++) 
				fscanf(fp, "%lf", &WeightHO[i][j]); //read in weightsOH
		fclose(fp);

		fp = fopen("eta.txt", "r");
		fscanf(fp, "%lf", &eta);
		fclose(fp);
	}

	if (mode == IDENT) {//if identifying images
		printf("first\n");
		for (i = 0; i < num_output; i++ ) {
			printf("%.2lf ", Target[0][i]);
		}
		printf("\n");
      	for( i = 0 ; i < num_input ; i++ ) {    //repeat for all the training patterns 
		    for( j = 0 ; j < NumHidden ; j++ ) {    // compute hidden unit activations 
		       SumH[i][j] = 0 ;
		        for( k = 0; k < input_size ; k++ ) {
		        	for (m = 0; m < input_size ; m++){
		                SumH[i][j] += Input[i][k][m] * WeightIH[j][k][m];
		        	}
		        }
		        Hidden[i][j] = 1.0/(1.0 + exp(-SumH[i][j])) ; 
		    }
		    for( k = 0 ; k < num_output ; k++ ) {     //compute output unit activations and errors 
		        SumO[i][k] = 0 ;
		        for( j = 0 ; j < NumHidden ; j++ ) {
		            SumO[i][k] += Hidden[i][j] * WeightHO[j][k];
		        }
		        Output[i][k] = 1.0/(1.0 + exp(-SumO[i][k]));   //Sigmoidal Outputs 
		       // Output[p][k] = SumO[p][k];  //    Linear Outputs
		        Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]); 
		        // printf("%.2lf \n", Error);
		    }
		}
		printf("messed up\n");
		for (i = 0; i < num_output; i++ ) {
			printf("%.2lf ", Target[0][i]);
		}
		printf("\n");
		for (i = 0; i < num_output; i++ ) {
			printf("%.2lf ", Output[0][i]);
		}
		printf("\nIt is a %d\n", arg_max(Output[0]));
	}
	if (mode == TRAIN_INIT) {//for initial training we need to initalize the weights
	    printf("initializing weights\n");
	    for( i = 0 ; i < NumHidden ; i++ )    /* initialize WeightIH and DeltaWeightIH */
	        for( j = 0 ; j < input_size ; j++ ) 
	        	for (k = 0; k < input_size ; k++)
	        	{
		            DeltaWeightIH[i][j][k] = 0.0 ;
		            WeightIH[i][j][k] = 2.0 * ( rando() - 0.5 ) * smallwt ;
	        	}
	    for( i = 0 ; i < NumHidden ; i++ )    /* initialize WeightHO and DeltaWeightHO */
	        for( j = 0 ; j < num_output ; j++ ) {
	            DeltaWeightHO[i][j] = 0.0 ;
	            WeightHO[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
	        }
	}

	if (mode != IDENT) { //if training
	    printf("starting epochs\n");
	    for( epoch = 0 ; epoch < 1; epoch++) {    /* iterate weight updates */
	    	// printf("epoch: %d \n",epoch);
	    	if (eta > 0.000001)
				eta = eta *.5;
	        for( i = 0 ; i < NumPattern ; i++ )    /* randomize order of individuals */
	            ranpat[i] = i ;
	        for ( i = NumPattern-1; i > 0; i--) {//knuth shuffle
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
	                for( k = 0 ; k < NumOutput ; k++ )
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
	                for( k = 0 ; k < NumOutput ; k++ ) {
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
	    }
	
	    //write weights input->hidden
	    fp = fopen("weightsIH.txt", "w+");
		for (i = 0; i < NumHidden; i++) {
			for (j = 0; j < input_size; j++) 
				for (k = 0; k < input_size; k++)
					fprintf(fp, "%lf ", WeightIH[i][j][k]);
			fprintf(fp, " \n");
		}
		fclose(fp);

		//write weights hidden -> output
		fp = fopen("weightsHO.txt", "w+");
		for (i = 0; i < NumHidden; i++) {
			for (j = 0; j < num_output; j++) 
				fprintf(fp, "%lf ", WeightHO[i][j]);
			fprintf(fp," \n");
		}
		fclose(fp);

		fp = fopen("eta.txt", "w+"); //write eta to file
		fprintf(fp, "%lf", eta);
		fclose(fp);
	}
		


    fprintf(stdout, "NETWORK DATA - EPOCH %d\n", epoch) ;   /* print network outputs */
    if (display == 1) {
			for( i = 0 ; i < NumInput ; i++ ) 
		        fprintf(stdout, "Input%-4d\t", i) ;
		    for( k = 0 ; k < NumOutput ; k++ ) 
		        fprintf(stdout, "Target%-4d\tOutput%-4d\t", k, k) ;
		    for( p = 0 ; p < NumPattern ; p++ ) {
		    	fprintf(stdout, "\n%d\t", p) ;
		        // for( i = 0 ; i < input_size ; i++ ) {
		        // 	for (j = 0; j < input_size; j++)
		        // 		fprintf(stdout, "%lf\t", Input[p][i][j]) ;
		        // }
		        for( k = 0 ; k < NumOutput ; k++ ) {
		            fprintf(stdout, "%lf\t%lf\t", Target[p][k], Output[p][k]) ;
		        }
		    }
	}
    num_correct = 0;
    for (i = 0; i < num_input; i++) {
		j = arg_max(Target[i]); 
		k = arg_max(Output[i]);
	//	 printf ("j: %d, k: %d\n",j,k);
		if (j == k)	
			num_correct++;

	}
	printf("percent correct: %lf error: %lf\n", (double)((double)num_correct*100/(double)num_input),Error);
    fprintf(stdout, "Goodbye!\n") ;
	// L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	// 	while(1);

    return 1 ;
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

