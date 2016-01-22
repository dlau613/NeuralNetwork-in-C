//Authors: Daniel Lau and Ben Quachtran
// #include "L138_LCDK_aic3106_init.h"
// #include "evmomapl138_gpio.h"

#include <stdlib.h>
// #include "m_mem.h"
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#define NUMPAT 4
#define NUMIN  2
#define NUMHID 2
#define NUMOUT 1

#define num_input 10000
#define input_size 28
#define num_output 14
#define NumHidden 150

// #define rando() ((double)rand()/(RAND_MAX+1))
 #define rando() ((double)rand()/(RAND_MAX))
#define PI 3.1415962



// interrupt void interrupt4(void) // associated in intvecs.asm with INT4
// {
// 	output_left_sample(0);
// 	return;                       //return from ISR
// }

		int    i, j, k, p, m, n, np, op, ranpat[num_input], epoch, num_correct;
	    int    NumPattern = num_input, NumInput = input_size*input_size, NumOutput = num_output;


	    double Input[num_input][input_size][input_size]; /* = { 
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,1} },	//1
	    		  { {0, 0}, {1,0} },	//1
	    		  { {0, 0}, {0,1} },	//1
	    		  { {0, 0}, {1,0} },	//1
	    		  { {0, 0}, {1,1} },	//2
	    		  { {0, 1}, {0,0} },	//1
	    		  { {0, 1}, {0,1} },	//2
	    		  { {0, 1}, {1,0} },	//2
	    		  { {0, 1}, {1,1} },	//3
	    		  { {1, 0}, {0,0} },	//1
	    		  { {1, 0}, {0,1} },	//2
	    		  { {1, 0}, {1,0} },	//2
	    		  { {1, 0}, {1,1} },	//3
	    		  { {1, 1}, {0,0} },	//2
	    		  { {1, 1}, {0,1} },	//3
	    		  { {1, 1}, {1,0} },	//3
	    		  { {1, 1}, {0,1} },	//3
	    		  { {1, 1}, {1,0} },	//3
	    		  { {1, 1}, {1,1} },	//4
	    		  { {1, 1}, {1,1} },	//4
	    		  { {1, 1}, {1,1} },	//4
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,1} },	//1
	    		  { {0, 0}, {1,0} },	//1
	    		  { {0, 0}, {0,1} },	//1
	    		  { {0, 0}, {1,0} },	//1
	    		  { {0, 0}, {1,1} },	//2
	    		  { {0, 1}, {0,0} },	//1
	    		  { {0, 1}, {0,1} },	//2
	    		  { {0, 1}, {1,0} },	//2
	    		  { {0, 1}, {1,1} },	//3
	    		  { {1, 0}, {0,0} },	//1
	    		  { {1, 0}, {0,1} },	//2
	    		  { {1, 0}, {1,0} },	//2
	    		  { {1, 0}, {1,1} },	//3
	    		  { {1, 1}, {0,0} },	//2
	    		  { {1, 1}, {0,1} },	//3
	    		  { {1, 1}, {1,0} },	//3
	    		  { {1, 1}, {0,1} },	//3
	    		  { {1, 1}, {1,0} },	//3
	    		  { {1, 1}, {1,1} },	//4
	    		  { {1, 1}, {1,1} },	//4
	    		  { {1, 1}, {1,1} },	//4
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,0} },	//0
	    		  { {0, 0}, {0,1} },	//1
	    		  { {0, 0}, {1,0} },	//1
	    		  { {0, 0}, {0,1} },	//1
	    		  { {0, 0}, {1,0} },	//1
	    		  { {0, 0}, {1,1} },	//2
	    		  { {0, 1}, {0,0} },	//1
	    		  { {0, 1}, {0,1} },	//2
	    		  { {0, 1}, {1,0} },	//2
	    		  { {0, 1}, {1,1} },	//3
	    		  { {1, 0}, {0,0} },	//1
	    		  { {1, 0}, {0,1} },	//2
	    		  { {1, 0}, {1,0} },	//2
	    		  { {1, 0}, {1,1} },	//3
	    		  { {1, 1}, {0,0} },	//2
	    		  { {1, 1}, {0,1} },	//3
	    		  { {1, 1}, {1,0} },	//3
	    		  { {1, 1}, {0,1} },	//3
	    		  { {1, 1}, {1,0} },	//3
	    		  { {1, 1}, {1,1} },	//4
	    		  { {1, 1}, {1,1} },	//4
	    		  { {1, 1}, {1,1} },	//4

	    };*/
	    double Target[num_input][num_output]; /*= { 
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,0,1},
	    		   {0,0,0,0,1},
	    		   {0,0,0,0,1},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,0,1},
	    		   {0,0,0,0,1},
	    		   {0,0,0,0,1},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {1,0,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,1,0,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,0,1,0,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,1,0},
	    		   {0,0,0,0,1},
	    		   {0,0,0,0,1},
	    		   {0,0,0,0,1},

	    };*/


	    double SumH[num_input][num_output], WeightIH[NumHidden][input_size][input_size], Hidden[num_input][NumHidden];
	    double SumO[num_input][num_output], WeightHO[NumHidden][num_output], Output[num_input][num_output];
	    double DeltaO[num_output], SumDOW[NumHidden], DeltaH[NumHidden];
	    double DeltaWeightIH[NumHidden][input_size][input_size], DeltaWeightHO[NumHidden][num_output];
	    double Error, eta = 0.1, alpha = 0.9, smallwt = 0.05;	//eta is learning rate,

	    FILE * fp;

static int train = 0, recognize = 0;
int arg_max(double* array);
int main(void)
{
	//training or recognizing?
	//train => load training data and labels
	//load training digits and labels, do epochs, save weights
	fp = fopen("training_digits.txt", "r");
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 28; j++) {
			for (k = 0; k < 28; k++) {
				fscanf(fp, "%lf", &Input[i][j][k]);
			}
		}
		// printf("image %d\n", i);
	}
	fclose(fp);

	fp = fopen("training_labels.txt", "r");
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 14; j++) {
			fscanf(fp, "%lf", &Target[i][j]);
		}
	}
	fclose(fp);

	/////////if doing recognition
	if (recognize ==1) {
		printf("begin recognition\n");
		fp = fopen("weightsIH.txt", "r");
		printf("opened weightsIH\n");
		for (i = 0; i < NumHidden; i++) {
			for (j = 0; j < input_size; j++) {
				for (k = 0; k < input_size; k++)
					fscanf(fp,"%lf", &WeightIH[i][j][k]);
			}
		}
		fclose(fp);

		fp = fopen("weightsHO.txt", "r");
		printf("opened weightsHO\n");
		for (i = 0; i < NumHidden; i++) {
			for (j = 0; j < num_output; j++) 
				fscanf(fp, "%lf", &WeightHO[i][j]);
		}
		fclose(fp);

			// printf("%lf  %lf  %lf  %lf \n", WeightHO[0][0], WeightHO[0][1],WeightHO[0][2], WeightHO[0][3]  );
/////////////////////////////////////////////////
  //     	for( i = 0 ; i < num_input ; i++ ) {    /* repeat for all the training patterns */
		//     for( j = 0 ; j < NumHidden ; j++ ) {    /* compute hidden unit activations */
		//         SumH[i][j] = 0 ;
		//         for( k = 0; k < input_size ; k++ ) {

		//         	for (m = 0; m < input_size ; m++)
		//         	{
		//                 SumH[i][j] += Input[i][k][m] * WeightIH[j][k][m] ;
		//         	}
		//         }
		//         Hidden[i][j] = 1.0/(1.0 + exp(-SumH[i][j])) ;
		//     }
		//     for( k = 0 ; k < num_output ; k++ ) {    /* compute output unit activations and errors */
		//         SumO[i][k] = 0 ;
		//         for( j = 0 ; j < NumHidden ; j++ ) {
		//             SumO[i][k] += Hidden[i][j] * WeightHO[j][k] ;
		//         }
		//         Output[i][k] = 1.0/(1.0 + exp(-SumO[i][k])) ;   /* Sigmoidal Outputs */
		//        // Output[p][k] = SumO[p][k];  //    Linear Outputs
		//     }
		// }
//////////////////////////////////////////
		  printf("starting epochs\n");
    for( epoch = 0 ; epoch < 2 ; epoch++) {    /* iterate weight updates */
    	printf("epoch: %d \n",epoch);
        // for( p = 0 ; p < NumPattern ; p++ ) {    /* randomize order of individuals */
        //     ranpat[p] = p ;
        // }
        // for( p = 1 ; p < NumPattern ; p++) {
        //     np = p + rando() * ( NumPattern + 1 - p ) ;
        //     op = ranpat[p] ; ranpat[p] = ranpat[np] ; ranpat[np] = op ;
        // }
        Error = 0.0 ;



        for( i = 0 ; i < num_input ; i++ ) {    /* repeat for all the training patterns */
            for( j = 0 ; j < NumHidden ; j++ ) {    /* compute hidden unit activations */
                SumH[i][j] = 0 ;
                for( k = 0; k < input_size ; k++ ) {

                	for (m = 0; m < input_size ; m++)
                	{
	                    SumH[i][j] += Input[i][k][m] * WeightIH[j][k][m] ;
                	}
                }
                Hidden[i][j] = 1.0/(1.0 + exp(-SumH[i][j])) ;
            }
            for( k = 0 ; k < num_output ; k++ ) {    /* compute output unit activations and errors */
                SumO[i][k] = 0 ;
                for( j = 0 ; j < NumHidden ; j++ ) {
                    SumO[i][k] += Hidden[i][j] * WeightHO[j][k] ;
                }
                Output[i][k] = 1.0/(1.0 + exp(-SumO[i][k])) ;   /* Sigmoidal Outputs */
   //             Output[p][k] = SumO[p][k];  //    Linear Outputs

	            // Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]) ;   /* SSE */
	         	Error -= ( Target[i][k] * log( Output[i][k] ) + ( 1.0 - Target[i][k] ) * log( 1.0 - Output[i][k] ) ) ;  //  Cross-Entropy Error
	      		// DeltaO[k] = (Target[i][k] - Output[i][k]) * Output[i][k] * (1.0 - Output[i][k]) ;   /* Sigmoidal Outputs, SSE 
	        	DeltaO[k] = Target[i][k] - Output[i][k];  //   Sigmoidal Outputs, Cross-Entropy Error 
	          	// DeltaO[k] = Target[i][k] - Output[i][k];    // Linear Outputs, SSE
            }
            for( j = 0 ; j < NumHidden ; j++ ) {    /* 'back-propagate' errors to hidden layer */
               SumDOW[j] = 0.0 ;
                for( k = 0 ; k < NumOutput ; k++ ) {
                    SumDOW[j] += WeightHO[j][k] * DeltaO[k] ;
                }
                DeltaH[j] = SumDOW[j] * Hidden[i][j] * (1.0 - Hidden[i][j]) ;
            }
            // for( j = 0 ; j < NumHidden ; j++ ) {     /* update weights WeightIH */
            //    // DeltaWeightIH[j][0][0] = eta * DeltaH[j] + alpha * DeltaWeightIH[0][j] ;
            //    // WeightIH[0][j] += DeltaWeightIH[0][j] ;
            //     for( m = 0 ; m < input_size ; m++ ) {

            //     	for (n = 0; n < input_size ; n++)
            //     	{
            //         DeltaWeightIH[j][m][n] = eta * Input[i][m][n] * DeltaH[j] + alpha * DeltaWeightIH[j][m][n];
            //         WeightIH[j][m][n] += DeltaWeightIH[j][m][n] ;
            //     	}
            //     }
            // }
            // for( j = 0 ; j < NumHidden ; j ++ ) {    /* update weights WeightHO */
            //     // DeltaWeightHO[0][k] = eta * DeltaO[k] + alpha * DeltaWeightHO[0][k] ;
            //     // WeightHO[0][k] += DeltaWeightHO[0][k] ;
            //     for( k = 0 ; k < NumOutput ; k++ ) {
            //         DeltaWeightHO[j][k] = eta * Hidden[i][j] * DeltaO[k] + alpha * DeltaWeightHO[j][k] ;
            //         WeightHO[j][k] += DeltaWeightHO[j][k] ;
            //     }
            // }
        }
        if( epoch%100 == 0 ) fprintf(stdout, "\nEpoch %-5d :   Error = %lf", epoch, Error) ;
        if( Error < 0.0004 ) break ;  /* stop learning when 'near enough' */
    }
		for (i = 0; i < num_input; i++) {
			j = arg_max(Target[i]); 
			k = arg_max(Output[i]);
			// printf ("j: %d, k: %d\n",j,k);
			if (j == k)	
				num_correct++;

		}
		printf("percent correct: %lf\n", (double)((double)num_correct*100/(double)num_input));
		printf("recognition done\n");
		// for( k = 0 ; k < NumOutput ; k++ ) {
	 //        fprintf(stdout, "Target%-4d\tOutput%-4d\t", k, k) ;
	 //    }
	 //    for( p = 0 ; p < NumPattern ; p++ ) {
	 //    fprintf(stdout, "\n%d\t", p) ;
	 //        // for( i = 0 ; i < input_size ; i++ ) {
	 //        // 	for (j = 0; j < input_size; j++)
	 //        // 		fprintf(stdout, "%lf\t", Input[p][i][j]) ;
	 //        // }
	 //        for( k = 0 ; k < NumOutput ; k++ ) {
	 //            fprintf(stdout, "%lf\t%lf\t", Target[p][k], Output[p][k]) ;
	 //        }
	 //    }
	    fprintf(stdout, "\n\nGoodbye!\n\n") ;
	    return 1;
	}
	printf("begin\n");

   	fp = fopen("training_digits.txt", "r");
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 28; j++) {
			for (k = 0; k < 28; k++) {
				fscanf(fp, "%lf", &Input[i][j][k]);
			}
		}
	}
	fclose(fp);

	fp = fopen("training_labels.txt", "r");
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 14; j++) {
			fscanf(fp, "%lf", &Target[i][j]);
		}
	}
	fclose(fp);


    // double Input[num_input][input_size][input_size]; // input
    //double Target[num_input][num_output]; // output
    printf("above\n");



	printf("beginning\n");



    printf("declaring\n");

    for( i = 0 ; i < NumHidden ; i++ ) {    /* initialize WeightIH and DeltaWeightIH */
        for( j = 0 ; j < input_size ; j++ ) {

        	for (k = 0; k < input_size ; k++)
        	{
	            DeltaWeightIH[i][j][k] = 0.0 ;
	            WeightIH[i][j][k] = 2.0 * ( rando() - 0.5 ) * smallwt ;
        	}

        }
    }
	printf("initializing\n");

    for( i = 0 ; i < NumHidden ; i ++ ) {    /* initialize WeightHO and DeltaWeightHO */
        for( j = 0 ; j < num_output ; j++ ) {
            DeltaWeightHO[i][j] = 0.0 ;
            WeightHO[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
        }
    }
    printf("starting epochs\n");
    for( epoch = 0 ; epoch < 2 ; epoch++) {    /* iterate weight updates */
    	printf("epoch: %d \n",epoch);
        // for( p = 0 ; p < NumPattern ; p++ ) {    /* randomize order of individuals */
        //     ranpat[p] = p ;
        // }
        // for( p = 1 ; p < NumPattern ; p++) {
        //     np = p + rando() * ( NumPattern + 1 - p ) ;
        //     op = ranpat[p] ; ranpat[p] = ranpat[np] ; ranpat[np] = op ;
        // }
        Error = 0.0 ;



        for( i = 0 ; i < num_input ; i++ ) {    /* repeat for all the training patterns */
            for( j = 0 ; j < NumHidden ; j++ ) {    /* compute hidden unit activations */
                SumH[i][j] = 0 ;
                for( k = 0; k < input_size ; k++ ) {

                	for (m = 0; m < input_size ; m++)
                	{
	                    SumH[i][j] += Input[i][k][m] * WeightIH[j][k][m] ;
                	}
                }
                Hidden[i][j] = 1.0/(1.0 + exp(-SumH[i][j])) ;
            }
            for( k = 0 ; k < num_output ; k++ ) {    /* compute output unit activations and errors */
                SumO[i][k] = 0 ;
                for( j = 0 ; j < NumHidden ; j++ ) {
                    SumO[i][k] += Hidden[i][j] * WeightHO[j][k] ;
                }
                Output[i][k] = 1.0/(1.0 + exp(-SumO[i][k])) ;   /* Sigmoidal Outputs */
   //             Output[p][k] = SumO[p][k];  //    Linear Outputs

	            // Error += 0.5 * (Target[i][k] - Output[i][k]) * (Target[i][k] - Output[i][k]) ;   /* SSE */
	         	Error -= ( Target[i][k] * log( Output[i][k] ) + ( 1.0 - Target[i][k] ) * log( 1.0 - Output[i][k] ) ) ;  //  Cross-Entropy Error
	      		// DeltaO[k] = (Target[i][k] - Output[i][k]) * Output[i][k] * (1.0 - Output[i][k]) ;   /* Sigmoidal Outputs, SSE 
	        	DeltaO[k] = Target[i][k] - Output[i][k];  //   Sigmoidal Outputs, Cross-Entropy Error 
	          	// DeltaO[k] = Target[i][k] - Output[i][k];    // Linear Outputs, SSE
            }
            for( j = 0 ; j < NumHidden ; j++ ) {    /* 'back-propagate' errors to hidden layer */
               SumDOW[j] = 0.0 ;
                for( k = 0 ; k < NumOutput ; k++ ) {
                    SumDOW[j] += WeightHO[j][k] * DeltaO[k] ;
                }
                DeltaH[j] = SumDOW[j] * Hidden[i][j] * (1.0 - Hidden[i][j]) ;
            }
            for( j = 0 ; j < NumHidden ; j++ ) {     /* update weights WeightIH */
               // DeltaWeightIH[j][0][0] = eta * DeltaH[j] + alpha * DeltaWeightIH[0][j] ;
               // WeightIH[0][j] += DeltaWeightIH[0][j] ;
                for( m = 0 ; m < input_size ; m++ ) {

                	for (n = 0; n < input_size ; n++)
                	{
                    DeltaWeightIH[j][m][n] = eta * Input[i][m][n] * DeltaH[j] + alpha * DeltaWeightIH[j][m][n];
                    WeightIH[j][m][n] += DeltaWeightIH[j][m][n] ;
                	}
                }
            }
            for( j = 0 ; j < NumHidden ; j ++ ) {    /* update weights WeightHO */
                // DeltaWeightHO[0][k] = eta * DeltaO[k] + alpha * DeltaWeightHO[0][k] ;
                // WeightHO[0][k] += DeltaWeightHO[0][k] ;
                for( k = 0 ; k < NumOutput ; k++ ) {
                    DeltaWeightHO[j][k] = eta * Hidden[i][j] * DeltaO[k] + alpha * DeltaWeightHO[j][k] ;
                    WeightHO[j][k] += DeltaWeightHO[j][k] ;
                }
            }
        }
        if( epoch%100 == 0 ) fprintf(stdout, "\nEpoch %-5d :   Error = %lf", epoch, Error) ;
        if( Error < 0.0004 ) break ;  /* stop learning when 'near enough' */
    }
	    //write weights input->hidden
	    fp = fopen("weightsIH.txt", "w+");
		for (i = 0; i < NumHidden; i++) {
			for (j = 0; j < input_size; j++) {
				for (k = 0; k < input_size; k++) {
					fprintf(fp, "%lf ", WeightIH[i][j][k]);
				}
			}
			fprintf(fp, " \n");
		}
		fclose(fp);

		//write weights hidden -> output
		fp = fopen("weightsHO.txt", "w+");
		for (i = 0; i < NumHidden; i++) {
			for (j = 0; j < num_output; j++) {
				fprintf(fp, "%lf ", WeightHO[i][j]);
			}
			fprintf(fp," \n");
		}
		fclose(fp);
		


	    fprintf(stdout, "\n\nNETWORK DATA - EPOCH %d\n\nPat\t", epoch) ;   /* print network outputs */
	    // for( i = 0 ; i < NumInput ; i++ ) {
	    //     fprintf(stdout, "Input%-4d\t", i) ;
	    // }
	    // for( k = 0 ; k < NumOutput ; k++ ) {
	    //     fprintf(stdout, "Target%-4d\tOutput%-4d\t", k, k) ;
	    // }
	    // for( p = 0 ; p < NumPattern ; p++ ) {
	    // fprintf(stdout, "\n%d\t", p) ;
	    //     // for( i = 0 ; i < input_size ; i++ ) {
	    //     // 	for (j = 0; j < input_size; j++)
	    //     // 		fprintf(stdout, "%lf\t", Input[p][i][j]) ;
	    //     // }
	    //     for( k = 0 ; k < NumOutput ; k++ ) {
	    //         fprintf(stdout, "%lf\t%lf\t", Target[p][k], Output[p][k]) ;
	    //     }
	    // }
	    for (i = 0; i < num_input; i++) {
			j = arg_max(Target[i]); 
			k = arg_max(Output[i]);
		//	 printf ("j: %d, k: %d\n",j,k);
			if (j == k)	
				num_correct++;

		}
		printf("percent correct: %lf\n", (double)((double)num_correct*100/(double)num_input));
	    fprintf(stdout, "\n\nGoodbye!\n\n") ;
		// L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
		// 	while(1);

	    return 1 ;
}

int arg_max(double* array) {
	int arg = 0;
	int i = 0;
	double current_max = 0;
	for (i = 0; i < 14; i ++) {
		if (array[i] > current_max) {
			current_max = array[i];
			arg = i;
		}
	}
	return arg;
}

