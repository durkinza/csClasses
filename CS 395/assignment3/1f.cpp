/***********************************************************
 * NAME: Zane Durkin
 * ID: durk7832
 * QUESTION: Asn1.f
 * DESCRIPTION: Finding intersections of arrays
 ***********************************************************/
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

#define LEN 7
int A[LEN] = {1,3,5,7,9,4,8}; // 4,8 should be the intersection.
int B[LEN] = {0,2,4,6,8,10,12};

int bruteForce(int *A, int *B, int *C){
    // Find intersection of A and B and return it in C;
    int k = 0; // number of intersections found
    for(int i = 0; i < LEN; i++){
        for(int j = 0; j < LEN; j++){
            if(A[i] == B[j]){
                // since A and B are the same, we can save either one
                C[k] = A[i];
                // increment our intersection counter
                k++;
            }
        }
    }
    return k;
}

int smart_intersect(int *A, int *B, int *C){
    int k = 0; // number of intersections found
    int i = 0; // counter for A
    int j = 0; // counter for B
    while(i < LEN && j < LEN){
        if(A[i] == B[j]){
            // save where they are equal
            C[k] = A[i];
            k++;
            i++;
            j++;
        }else
        if(A[i] > B[j]){
            // skip all lower numbers in B
            while(A[i] > B[j] && j < LEN){
                j++;
            }
        }else
        if(B[j] > A[i]){
            // skip all lower numbers in A
            while(B[j] > A[i] && i < LEN){
                i++;
            }
        }
    }
    return k;
}

main(){
    int C[LEN]; // 7 elements is more than enough
    int D[LEN]; // 7 elements is more than enough
    int intersects = bruteForce(A, B, C);
    printf("Brute force intersection is: \n");
    for(int i = 0; i < intersects; i++){
        printf("%d,", C[i]);
    }
    printf("\n");

    printf("Smart intersection is: \n");
    sort(A, A+7);
    sort(B, B+7);
    intersects = smart_intersect(A, B, D);
    for(int i = 0; i < intersects; i++){
        printf("%d,", D[i]);
    }

}

/*
*/
/****************************************************

COMPILE:

gcc -Wall -o 1.out 1.c

RUN:

./1.out

OUTPUT:

Brute force intersection is:
4,8,
Smart intersection is:
4,8,

****************************************************/
