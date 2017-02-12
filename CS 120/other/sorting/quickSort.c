#include <iostream>
using namespace std;
//  http://codereview.stackexchange.com/questions/77782/quick-sort-implementation
// define list size to sort
#define SIZE 20
// create testing list(s)
int list[SIZE] = {1, 111, 134, 353, 9, 2, 65, 34, 64, 4, 20, 8, 5, 92, 3, 45, 25, 86, 23};
int list2[SIZE] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int list3[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};


int* quickSort(int* arr){
    int min = SIZE / 2;

    int i = 0;
    int j = SIZE;
    int pivot = arr[min];
    
    for(int j=0; j<SIZE; j++){
	int pivot = arr[j];
        for(int i=0; i<SIZE; i++){
	    if(arr[i] < pivot){
		// move array element to the left of the pivot
        	rotate(arr, i, i+1)
	    }
	} 
    }
    return arr;
}

int main(){
    // run the sorting funciton
    int* Sorted = quickSort(list);

    // print out final array
    for(int i=0; i<SIZE; i++){
	cout<<Sorted[i]<<endl;
    }

}
