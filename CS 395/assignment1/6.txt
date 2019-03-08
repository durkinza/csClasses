/***********************************************************

 * NAME: Zane Durkin	

 * ID: durk7832

 * QUESTION: Asn1.6e

 * DESCRIPTION: implement the Enigma algorithm

 ***********************************************************/

 #include <stdio.h>

int main()

{

	#define ARR_LEN 6
	int arr[ARR_LEN][ARR_LEN];

	// fill matrix
	for(int a = 0; a<ARR_LEN; a++){
		for(int b = 0; b< ARR_LEN; b++){
			arr[a][b] = 2;
		}
	}

	// (optinal) make it non-symetric
	//arr[2][3] = 4;

	// for each row
	for(int i = 0; i < ARR_LEN-1; i++){
		// for each column
		for( int j = 1+1; j< ARR_LEN; j++){
			// check if the value is symetric
			if(arr[i][j] != arr[j][i]){
				// if they are different, fail and leave
				printf("false\n");
				return 1;//non-zero for failed
			}
		}
	}
	// if everything was correct, return true
	printf("true\n");
	return 0;

}

/****************************************************

COMPILE:

gcc -Wall -o 6.out 6.cpp

RUN:

./6.out

OUTPUT:

 true

****************************************************/
