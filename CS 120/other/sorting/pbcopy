// Zane Durkin
// index sorting algorithum 
// 9/16/16

#include <iostream>
using namespace std;

#define SIZE 20
int list[SIZE] = {1, 111, 134, 353, 9, 2, 65, 34, 64, 4, 20, 8, 5, 92, 3, 45, 25, 86, 23};

int* testSort (int* arr){
    int arr2[SIZE];			// array to hold indexs
    int arr3[SIZE];			// array to hold newly sorted array for returning
    int big;				// variable to hold index of biggest number
    for(int j=0; j<SIZE; j++){		// step through the building of arr2
	big = 0;			//     reset big to be 
	for(int i=0; i<SIZE; i++){	//     step through arr
	    bool used =false;		//     variable to hold if array element has already been used
	    for(int k=0; k<j; k++){	//     step through arr2 to see if element has been used yet
		if(i == arr2[k]){	//         if the element has been used, say so
		    used = true;
		}
	    }
	    if(!used){			//     if the element hasn't been used, use it
		if(arr[big] < arr[i]){
		    big=i;
		}
	    }   
        }
	arr2[j] = big;
//	cout<<big<<endl;
    }

    for(int i=0; i<SIZE; i++){
	int j = arr2[i];
	arr3[i] = arr[j];
    }
    return arr3;
}

int main (){
    // run bubble sort function with given list
    int* Sorted = testSort(list);	// switch given list to try different testing lists
    // print out final array
    for(int i=0; i<SIZE; i++){
	cout<<Sorted[i]<<endl;
    }
}

//jamil@uidaho.edu

