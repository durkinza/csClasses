#include <iostream>
using namespace std;

//define list size to sort
#define SIZE 20
// create three different testing lists
int list[SIZE] = {1, 111, 134, 353, 9, 2, 65, 34, 64, 4, 20, 8, 5, 92, 3, 45, 25, 86, 23};
int list2[SIZE] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int list3[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

int* indexSort (int* arr){
    for(int j=0; j<SIZE; j++){			// go through each index in the list
	int smallest=j;				//     place holder for index of smallest number
	int a = arr[j];				//     set testing number to variable
	for(int k=j; k<SIZE; k++){		//     for every element that folllows
	    if(arr[k] < arr[smallest]){		//         compare the element to see if it is smaller than the smallest
		smallest = k;			//             if it is smaller, set it to the new smallest
	    }					//         end if
	}					//     end for
	arr[j] = arr[smallest];			//     switch current element with the smallest
	arr[smallest] = a;			//
    }						// end for
    return arr;					// return new array 
}

int main (){
    // run indexSort function with given list
    int* Sorted = indexSort(list);	// switch given list to try different testing lists
    // print out final array
    for(int i=0; i<SIZE; i++){
	cout<<Sorted[i]<<endl;
    }
}
