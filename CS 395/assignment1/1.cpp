/***********************************************************

 * NAME: Zane Durkin

 * ID: durk7832

 * QUESTION: Asn1.1

 * DESCRIPTION: find minium distance between two elements of a list

 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

int main(){

	const int ARR_LEN = 25;
	int arr[ARR_LEN] = {2, 7, 34, 83, 27, 36, 24, 13, 63, 734, 30, 235, 562, 213, 32, 68, 93, 39, 95, 72, 104, 163, 295, 150, 230};
	int dmin = abs(arr[0]-arr[1]);
    
	std::sort(arr, arr+ARR_LEN);
	for(int i =0; i< ARR_LEN-1; i++){
		if(abs(arr[i]-arr[i+1]) < dmin){
			dmin = abs(arr[i]-arr[i+1]);
		}
	}
	printf("Minimum is: %i\n", dmin);

}



/****************************************************

COMPILE:

gcc -Wall -o 1.out 1.c

RUN:

./1.out

OUTPUT:

Minimum is: 2

****************************************************/
