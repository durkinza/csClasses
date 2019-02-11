/***********************************************************

 * NAME: Zane Durkin	

 * ID: durk7832

 * QUESTION: Asn1.8

 * DESCRIPTION: Write algorithm for 2^N = 2^n-1 + 2^n-1

 ***********************************************************/

 #include <stdio.h>
 
 int twon(int n);

int main(){
	for(int n=0; n<10; n++){
		printf("2^%d = %d\n", n, twon(n));
	}
}

int twon(int n){
	if(n == 0){
		return 1;
	}else{
		return twon(n-1)+twon(n-1);
	}
}

/****************************************************

COMPILE:

gcc -Wall -o 8.out 8.cpp

RUN:

./8.out

OUTPUT:

2^0 = 1
2^1 = 2
2^2 = 4
2^3 = 8
2^4 = 16
2^5 = 32
2^6 = 64
2^7 = 128
2^8 = 256
2^9 = 512

****************************************************/
