/***********************************************************

 * NAME: Zane Durkin	

 * ID: durk7832

 * QUESTION: Asn1.12

 * DESCRIPTION: implement the brute-force algorithm for convex-hull program

 ***********************************************************/

 #include <stdio.h>
 #define points 10

int isLeft(int * pointA, int * pointB, int * pointC);

int main(){
	// {x, y}
	int arr[points][2] = {{1, 3}, {5, 3}, {1, 4}, {2, 7}, {3, 6}, {6, 2}, {5, 2}, {8, 4}, {8, 3}, {4, 9}};

	int location;

	bool left = false;
	bool right = false;
	bool collinear = false;

	for( int i = 0; i < points; i++ ){

		for( int j = 0; j < points; j++ ){
			if( j == i ){
				continue;
			}

			left = false;
			right = false;
			collinear = false;
			for( int k = 0; k < points; k++ ){
				if( k == j || k == i ){
					continue;
				}
				location = isLeft(arr[i], arr[j], arr[k]);
				//printf("points (%d, %d, %d): %d\n",i, j, k, location);
				if(location > 0){
					left = true;
				}else if(location < 0){
					right = true;
				}else{
					collinear = true;
				}
				
			}
			//printf("vals: r:%d l:%d c:%d\n", right, left, collinear);
		
			if((!left) || (!right)){
				printf("point %d: {%d, %d} is convex\n", i, arr[i][0], arr[i][1]);
				break;
			}

		}
	
	}	

}

int isLeft(int * pointA, int * pointB, int * pointC){
	return ( ( pointB[0] - pointA[0] ) * ( pointC[1] - pointA[1] ) - ( pointB[1] - pointA[1] ) * ( pointC[0] - pointA[0] ) );
}


/****************************************************

COMPILE:

gcc -Wall -o 12.out 12.cpp

RUN:

./12.out

OUTPUT:

point 0: {1, 3} is convex
point 2: {1, 4} is convex
point 3: {2, 7} is convex
point 5: {6, 2} is convex
point 6: {5, 2} is convex
point 7: {8, 4} is convex
point 8: {8, 3} is convex
point 9: {4, 9} is convex

****************************************************/
