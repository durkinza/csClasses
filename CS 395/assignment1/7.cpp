/***********************************************************

 * NAME: Zane Durkin	

 * ID: durk7832

 * QUESTION: Asn1.7b

 * DESCRIPTION: implement an algorithum for the Tower of Hanoi

 ***********************************************************/

 #include <stdio.h>
 #define disks 5

// definitions
int pop(int peg[disks], int &val);
int push(int peg[disks], int val);
int swap_top(int pega[disks], int pegb[disks]);
int is_legal(int srcpeg[disks], int trgpeg[disks]);
int legal_move(int pega[disks], int pegb[disks]);
void print_pegs(int peg1[disks], int peg2[disks], int peg3[disks]);

int main(){
	int peg1 [disks];
	int peg2 [disks];
	int peg3 [disks];

	// fill disks on first peg
	// peg1 = [3, 2, 1] for n = 3
	// peg2 = [0, 0, 0]
	// peg3 = [0, 0, 0]
	for(int a = 0; a < disks; a++){
		peg1[a] = disks - a;
		peg2[a] = 0;
		peg3[a] = 0;
	}
	print_pegs(peg1, peg2, peg3);
	int steps = 0;
	if(disks%2 == 0){
		// even
		// a <--> b
		// a <--> c
		// b <--> c
		while(peg3[disks-1] != 1){
			if(peg3[disks-1] == 1){break;}
			legal_move(peg1, peg2);
			steps++;
			//print_pegs(peg1, peg2, peg3);
			if(peg3[disks-1] == 1){break;}
			legal_move(peg1, peg3);
			steps++;
			//print_pegs(peg1, peg2, peg3);
			if(peg3[disks-1] == 1){break;}
			legal_move(peg2, peg3);
			steps++;
			//print_pegs(peg1, peg2, peg3);
		}
	}else{
		// odd
		// a <--> c
		// a <--> b
		// b <--> c
		while(peg3[disks-1] != 1){
			if(peg3[disks-1] == 1){break;}
			legal_move(peg1, peg3);
			steps++;
			//print_pegs(peg1, peg2, peg3);
			if(peg3[disks-1] == 1){break;}
			legal_move(peg1, peg2);
			steps++;
			//print_pegs(peg1, peg2, peg3);
			if(peg3[disks-1] == 1){break;}
			legal_move(peg2, peg3);
			steps++;
			//print_pegs(peg1, peg2, peg3);
		}
	}
	print_pegs(peg1, peg2, peg3);
	printf("steps:%d\n", steps);


}

int pop(int peg[disks], int &val){
	for(int i = disks-1; i>=0; i--){
		if(peg[i] != 0){
			val = peg[i];
			peg[i] = 0;
			return true;
		}
	}
	return true;
}
int push(int peg[disks], int val){
	for(int i = 0; i<disks; i++){
		if(peg[i] == 0){
			peg[i] = val;
			return true;
		}
	}
	return true;
}

int swap_top(int pega[disks], int pegb[disks]){
	int val;
	if(pop(pega, val)){
		return push(pegb, val);
	}
	return false;
}

int is_legal(int srcpeg[disks], int trgpeg[disks]){
	int a = srcpeg[0];
	int b = trgpeg[0];
	for(int i = 1; i<disks; i++){
		// get top of src
		if(srcpeg[i] != 0){
			a = srcpeg[i];
		}
		// get top of target
		if(trgpeg[i] != 0){
			b = trgpeg[i];
		}
	}
	if( a!= 0 && (a < b || b == 0)){
		return true;
	}
	return false;
}

int legal_move(int pega[disks], int pegb[disks]){
	if(is_legal(pega, pegb)){
		// if pega can be moved to pegb, do so
		return swap_top(pega, pegb);
	}else{
		// if pegb can be moved to pega, do so
		return swap_top(pegb, pega);
	}
}

void print_pegs(int peg1[disks], int peg2[disks], int peg3[disks]){
	for(int i = disks-1; i>=0; i--){
		printf("[%d] [%d] [%d]\n", peg1[i], peg2[i], peg3[i]);
	}
	printf(" p1  p2  p3\n");
}
/****************************************************

COMPILE:

gcc -Wall -o 7.out 7.cpp

RUN:

./7.out

OUTPUT:

[1] [0] [0]
[2] [0] [0]
[3] [0] [0]
[4] [0] [0]
[5] [0] [0]
 p1  p2  p3
[0] [0] [1]
[0] [0] [2]
[0] [0] [3]
[0] [0] [4]
[0] [0] [5]
 p1  p2  p3
steps:31

****************************************************/
