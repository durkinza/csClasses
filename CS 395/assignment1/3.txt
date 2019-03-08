/***********************************************************

 * NAME: Zane Durkin

 * ID: durk7832

 * QUESTION: Asn1.3c

 * DESCRIPTION: implement gaussian elimination algorithm

 ***********************************************************/

 #include <stdio.h>
 #define ARR_LEN 5

void print_board(int board[ARR_LEN][ARR_LEN]);
int main(){
	int arr[ARR_LEN-1][ARR_LEN];
	
	// fill matrix
	for(int a = 0; a<ARR_LEN-1; a++){
		for(int b = 0; b< ARR_LEN; b++){
			arr[a][b] = a+b+1;
		}
	}

	printf("Built array:\n");
	print_board(arr);
	

	// Gaussian elimination
	for(int i = 0; i<ARR_LEN-2; i++){
		//printf("1\n");
		for(int j = i+1; j<ARR_LEN-1; j++){
			//printf("2\n");
			for(int k = i; k<ARR_LEN; k++){
				//printf("3-%d\n", k);
				arr[j][k] = arr[j][k] - arr[i][k] * arr[j][i] / arr[i][i];
			}
		}
	}
	printf("finished:\n");
	print_board(arr);
}
void print_board(int board[ARR_LEN-1][ARR_LEN]){
	for(int i=0; i<ARR_LEN-1; i++){
		for(int j=0; j<ARR_LEN; j++){
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
/****************************************************

COMPILE:

gcc -Wall -o 3.out 3.cpp

RUN:

time ./3.out

OUTPUT:

Built array:
1 2 3 4 5 
2 3 4 5 6 
3 4 5 6 7 
4 5 6 7 8 

finished:
1 2 3 4 5 
0 3 4 5 6 
0 0 5 6 7 
0 0 0 7 8
****************************************************/
