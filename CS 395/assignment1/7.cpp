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
	// print starting setup
	print_pegs(peg1, peg2, peg3);
	int steps = 0;
	// switch pattern based on even/odd number of disks
	if(disks%2 == 0){
		// even
		// a <--> b
		// a <--> c
		// b <--> c
		while(peg3[disks-1] != 1){
			// check if process is done
			if(peg3[disks-1] == 1){break;}
			// preform legal move
			legal_move(peg1, peg2);
			// keep track of number of steps taken
			steps++;
			// print each step
			//print_pegs(peg1, peg2, peg3);
			// check if process is done
			if(peg3[disks-1] == 1){break;}
			// preform legal move
			legal_move(peg1, peg3);
			// keep track of number of steps taken
			steps++;
			// print each step
			//print_pegs(peg1, peg2, peg3);
			// check if process is done
			if(peg3[disks-1] == 1){break;}
			// preform legal move
			legal_move(peg2, peg3);
			// keep track of number of steps taken
			steps++;
			// print each step
			//print_pegs(peg1, peg2, peg3);
		}
	}else{
		// odd
		// a <--> c
		// a <--> b
		// b <--> c
		while(peg3[disks-1] != 1){
			// check if process is done
			if(peg3[disks-1] == 1){break;}
			// preform legal move
			legal_move(peg1, peg3);
			// keep track of number of steps taken
			steps++;
			// print each step
			//print_pegs(peg1, peg2, peg3);
			// check if process is done
			if(peg3[disks-1] == 1){break;}
			// preform legal move
			legal_move(peg1, peg2);
			// keep track of number of steps taken
			steps++;
			// print each step
			//print_pegs(peg1, peg2, peg3);
			// check if process is done
			if(peg3[disks-1] == 1){break;}
			// preform legal move
			legal_move(peg2, peg3);
			// keep track of number of steps taken
			steps++;
			// print each step
			//print_pegs(peg1, peg2, peg3);
		}
	}
	// print final output
	print_pegs(peg1, peg2, peg3);
	// say how many steps it took.
	printf("steps:%d\n", steps);
}

int pop(int peg[disks], int &val){
	// find top of disk
	for(int i = disks-1; i>=0; i--){
		// if we haven't reached top, continue
		if(peg[i] != 0){
			// if we are at the top, return value
			val = peg[i];
			// remove from peg
			peg[i] = 0;
			// return passed
			return true;
		}
	}
	// peg was empty, but it's fine
	return true;
}
int push(int peg[disks], int val){
	// find val on top of the disk
	for(int i = 0; i<disks; i++){
		// check if we have reached the top
		if(peg[i] == 0){
			// place disk on top and leave
			peg[i] = val;
			return true;
		}
	}
	// peg is too full, return error
	return true;
}

int swap_top(int pega[disks], int pegb[disks]){
	int val;
	// get the top of peg1
	if(pop(pega, val)){
		// put it on the top of pegb
		return push(pegb, val);
	}
	// if it failed then return failed
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
	// only swap if it is allowed
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
	// print each peg, one row at a time
	for(int i = disks-1; i>=0; i--){
		printf("[%d] [%d] [%d]\n", peg1[i], peg2[i], peg3[i]);
	}
	// label pegs at the bottom
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
