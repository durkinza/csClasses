#include <iostream>
using namespace std;

//define list size to sort
#define SIZE 20
// create three different testing lists
int list[SIZE] = {1, 111, 134, 353, 9, 2, 65, 34, 64, 4, 20, 8, 5, 92, 3, 45, 25, 86, 23};
int list2[SIZE] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int list3[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

int* bubbleSort (int* arr){
    bool change = true;			// create boolean to watch for changes
    // go through the given array
    while (change){			// keep running until no changes are found
	change = false;			// reset boolean to watch for changes
        for (int i=0; i<(SIZE-1); i++){ // compare the element down the list
	    int a = arr[i];		// grab the comparing element in the list
	    int b = arr[i+1];		// grab the next element in the list
	    if(a > b){			// if the numbers are out of order
		arr[i+1] = a;		//     switch the elements in the array
		arr[i] = b;		//
		change = true;		//     say that something was changed
	    }// end if
    	}// end for
    }// end while
    return arr;
}

int main (){
    // run bubble sort function with given list
    int* Sorted = bubbleSort(list);	// switch given list to try different testing lists
    // print out final array
    for(int i=0; i<SIZE; i++){
	cout<<Sorted[i]<<endl;
    }
}
