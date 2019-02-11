/***********************************************************

 * NAME: Zane Durkin	

 * ID: durk7832

 * QUESTION: Asn1.9b

 * DESCRIPTION: implement the algorthm to determine which adjacency matrix is expectecd for the given graph network

 ***********************************************************/

 #include <stdio.h>
 #define points 5
 int graph_type(bool arr[points][points]);

int main(){
	
	//bool arr[points][points] = {{1, 0, 0, 1, 1}, {0, 1, 1, 1, 1}, {1, 1, 1, 0, 0}, {1, 1, 0, 1, 0}, {1, 1, 0, 0, 1}}; //ring
	//bool arr[points][points] = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}}; //mesh
	bool arr[points][points] = {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0,0}, {1, 0, 0, 0, 0}}; //star
	int gt = graph_type(arr);
	if(gt>0){
		printf("star\n");
	}else if(gt<0){
		printf("mesh\n");
	}else{
		printf("ring\n");
	}
}
int graph_type(bool arr[points][points]){
	for(int i = 0; i < points; i++){
		int counter =0;
		for(int j = 0; j<points-1; j++){
			if(arr[i][j] == 1){counter++;}
		}
		if(counter < 2)
			return 1; // start
		if(counter == 2)
			return 0; // ring
	}
	return -1; // mesh

}

/****************************************************

COMPILE:

gcc -Wall -o 9.out 9.cpp

RUN:

./9.out

OUTPUT:

star

****************************************************/
