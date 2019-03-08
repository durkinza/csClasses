/***********************************************************

 * NAME: Zane Durkin	

 * ID: durk7832

 * QUESTION: Asn1.10

 * DESCRIPTION: Write brute force battleship algorithm 

 ***********************************************************/

 #include <stdio.h>
 #define spaces 10
void print_board(char board[spaces][spaces]);
int main(){
	// starting board
	char board[spaces][spaces] = {
		{126, 126, 126, 126, 126, 126, 66, 126, 126, 126},
		{126, 68, 68, 126, 126, 126, 66, 126, 126, 126},
		{126, 126, 126, 126, 126, 126, 66, 126, 126, 126},
		{126, 126, 126, 126, 126, 126, 66, 126, 126, 126},
		{126, 126, 126, 126, 126, 126, 126, 126, 67, 67},
		{126, 65, 126, 126, 126, 126, 126, 126, 126, 126},
		{126, 65, 126, 126, 126, 126, 126, 126, 126, 126},
		{126, 65, 126, 126, 126, 126, 83, 83, 83, 126},
		{126, 65, 126, 126, 126, 126, 126, 126, 126, 126},
		{126, 65, 126, 126, 126, 126, 126, 126, 126, 126},
	};
	// print start board
	print_board(board);
	// brute force board to find ships
	for(int i = 0; i<spaces; i++){
		// go through each column and check each space 
		for(int j =0; j<spaces; j++){
			if(board[i][j] != 126){
				// if it's a ship, mark the hit
				board[i][j] = 88;
				printf("Hit: %c%d\n", (char)65+j, i+1);
			}
		}
	}
	// print ending board
	print_board(board);
}

void print_board(char board[spaces][spaces]){
	// print A-Z
	printf("   ");
	for(int z=0; z<spaces; z++){
		printf("%c ", (char)65+z);
	}
	printf("\n");
	// print each row
	for(int i=0; i<spaces; i++){
		printf("%2d ", i+1);
		for(int j=0; j<spaces; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

/****************************************************

COMPILE:

gcc -Wall -o 10.out 10.cpp

RUN:

./10.cpp

OUTPUT:

   A B C D E F G H I J 
 1 ~ ~ ~ ~ ~ ~ B ~ ~ ~ 
 2 ~ D D ~ ~ ~ B ~ ~ ~ 
 3 ~ ~ ~ ~ ~ ~ B ~ ~ ~ 
 4 ~ ~ ~ ~ ~ ~ B ~ ~ ~ 
 5 ~ ~ ~ ~ ~ ~ ~ ~ C C 
 6 ~ A ~ ~ ~ ~ ~ ~ ~ ~ 
 7 ~ A ~ ~ ~ ~ ~ ~ ~ ~ 
 8 ~ A ~ ~ ~ ~ S S S ~ 
 9 ~ A ~ ~ ~ ~ ~ ~ ~ ~ 
10 ~ A ~ ~ ~ ~ ~ ~ ~ ~ 
Hit: G1
Hit: B2
Hit: C2
Hit: G2
Hit: G3
Hit: G4
Hit: I5
Hit: J5
Hit: B6
Hit: B7
Hit: B8
Hit: G8
Hit: H8
Hit: I8
Hit: B9
Hit: B10
   A B C D E F G H I J 
 1 ~ ~ ~ ~ ~ ~ X ~ ~ ~ 
 2 ~ X X ~ ~ ~ X ~ ~ ~ 
 3 ~ ~ ~ ~ ~ ~ X ~ ~ ~ 
 4 ~ ~ ~ ~ ~ ~ X ~ ~ ~ 
 5 ~ ~ ~ ~ ~ ~ ~ ~ X X 
 6 ~ X ~ ~ ~ ~ ~ ~ ~ ~ 
 7 ~ X ~ ~ ~ ~ ~ ~ ~ ~ 
 8 ~ X ~ ~ ~ ~ X X X ~ 
 9 ~ X ~ ~ ~ ~ ~ ~ ~ ~ 
10 ~ X ~ ~ ~ ~ ~ ~ ~ ~

****************************************************/
