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
	
	//bool arr[points][points] = {{0, 0, 0, 1, 1}, {0, 0, 1, 0, 1}, {0, 1, 0, 1, 0}, {1, 0, 1, 0, 0}, {1, 1, 0, 0, 0}}; //ring
	//bool arr[points][points] = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}}; //mesh
	bool arr[points][points] = {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0,0}, {1, 0, 0, 0, 0}}; //star
	// get graph type
	int gt = graph_type(arr);
	// say graph type in english
	if(gt>0){
		// if it's a star say so
		printf("star\n");
	}else if(gt<0){
		// if it's a mesh say so
		printf("mesh\n");
	}else{
		// if it's a ring say so
		printf("ring\n");
	}
}
int graph_type(bool arr[points][points]){
	// go through each point
	for(int i = 0; i < points; i++){
		int counter =0;
		// for each point count the number of connections
		for(int j = 0; j<points; j++){
			if(j ==i){continue;}
			// count each connections	
			if(arr[i][j] == 1){counter++;}
		}
		// if it's less than once connection, then we must have a star
		if(counter < 2)
			return 1; // star
		// if it's equal to 2 connections, then it must be a ring
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
