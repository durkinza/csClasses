/***********************************************************

 * NAME: Zane Durkin

 * ID: durk7832

 * QUESTION: Asn1.3c

 * DESCRIPTION: implement gaussian elimination algorithm

 ***********************************************************/

 #include <stdio.h>
 #define ARR_LEN 3

int main()

{
	int arr[ARR_LEN][ARR_LEN];
	
	// fill matrix
	for(int a = 0; a<ARR_LEN; a++){
		for(int b = 0; b< ARR_LEN; b++){
			arr[a][b] = a+b+1;
		}
	}
	
	//printf("done building array\n");

	// Gaussian elimination
	for(int i = 0; i<ARR_LEN-2; i++){
		printf("1\n");
		for(int j = i+1; j<ARR_LEN-1; j++){
			printf("2\n");
			for(int k = i; k<=ARR_LEN; k++){
				//printf("3-%d\n", k);
				arr[j][k] = arr[j][k]-arr[i][k]*arr[j][i] / arr[i][i];
			}
		}
	}
	//printf("done\n");
}

/****************************************************

COMPILE:

gcc -Wall -o 3.out 3.cpp

RUN:

time ./3.out

OUTPUT:

real0m0.009s
user0m0.002s
sys0m0.000s

****************************************************/
