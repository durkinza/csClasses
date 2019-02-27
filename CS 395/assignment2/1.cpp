/***********************************************************
 * NAME: Zane Durkin
 * ID: durk7832
 * QUESTION: Asn2.1
 * DESCRIPTION: The Clique Problem
 ***********************************************************/
 #include <iostream>
 using namespace std;
#define K 4
#define ARRSIZE 11

void findClique(int arr[][2], int k, int ret[][2]){
    if(k < 2 || k > ARRSIZE){
        return;
    }
    bool cliqueNotFound = false;
    int start = arr[0][0]; // the vector we started with
    int curr = arr[0][1]; // the vector we are currently looking for
    int steps = 1; // the current number of edges we are using
    // we'll know we have made a loop when curr == start
    while(start != curr){
        for(int i = 1; i<ARRSIZE; i++){
            if(arr[i][0] == curr){
            };
        }
    }
}


int main(){
    int arr[ARRSIZE][2] = { {0,1}, {0, 3}, {0, 4}, {1,2}, {1,4}, {2, 3}, {2, 4}, {2,5}, {3, 4}, {3, 5}, {4, 5} };
    int ret[K][2];
    findClique(arr, K, ret);
}
