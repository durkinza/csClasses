/***********************************************************
 * NAME: Zane Durkin
 * ID: durk7832
 * QUESTION: Asn2.6
 * DESCRIPTION: Write a divide and conquer algorithm
 ***********************************************************/
#include <iostream>
#include <math.h>       /* floor */
using namespace std;

// define list size to sort
#define SIZE 100
int list[SIZE] = {46,85,4,93,37,92,63,80,38,44,94,43,16,88,79,51,17,5,20,30,100,77,71,7,86,42,19,52,8,67,76,95,45,96,82,78,28,54,25,73,81,69,47,70,21,41,35,89,27,62,56,50,10,32,18,49,61,3,1,68,98,14,99,72,53,64,22,91,11,57,29,23,60,31,87,26,97,75,48,55,34,66,36,13,24,39,83,12,65,84,6,2,90,15,74,59,33,58,9,40};


void print(int *arr, int size){
    for(int i = 0; i < size-1; i++){
      cout << arr[i] << ',';
    }
    cout << arr[size - 1] << endl;
}

void merge(int * arr, int start1, int end1, int start2, int end2, int * ret_arr){
    int i = 0, a = start1, b = start2; // how far we are in merging the list
    while(a <= end1 || b <= end2){
        if(a <= end1){
            if(arr[a] >= arr[b] || b > end2){
                // if arr[a] is >= arr[b], or list 2 is out of elements
                ret_arr[i] = arr[a];
                a++;
            }else if(arr[b] > arr[a]){
                // if arr[b] > arr[a]
                ret_arr[i] = arr[b];
                b++;
            }
        }else{
            // if we are out of elements on list 1
            ret_arr[i] = arr[b];
            b++;
        }
        i++;
    }
    // copy ret_arr over arr
    int k = 0;
    for(int j = start1; j <= end2; j++){
        arr[j] = ret_arr[k];
        k++;
    }
}

void dandc(int *arr, int start, int end, int * ret_arr){
    if(end - start <= 1){return;}
    int split = ceil( ( end - start ) / 2);
    //print(arr, end);
    dandc(arr, start, end - split, ret_arr);
    dandc(arr, (end  - split) + 1, end, ret_arr);
    //printf("\nsplit is: %d, end: %d, start: %d\n", split, end, start);
    merge(arr, start, end-split, (end-split )+ 1, end, ret_arr);
}


main(){
    int ret_arr[SIZE];
    //print(list, SIZE);
    dandc(list, 0, SIZE, ret_arr);
    print(ret_arr, SIZE);
}
/****************************************************

COMPILE:

gcc -Wall -o 6.out 6.cpp

RUN:

./6.cpp

OUTPUT:
100,99,96,94,93,92,91,90,87,86,83,82,80,79,78,77,76,95,75,74,73,81,72,71,70,69,68,98,67,66,65,84,64,63,62,61,60,59,57,56,55,54,53,51,50,48,47,46,85,45,43,42,41,40,38,44,37,36,35,89,34,33,58,32,31,29,28,27,26,97,25,24,39,23,22,21,20,30,19,52,18,49,17,16,88,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1
****************************************************/
