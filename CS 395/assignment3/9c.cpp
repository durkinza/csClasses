/***********************************************************
 * NAME: Zane Durkin
 * ID: durk7832
 * QUESTION: Asn1.9
 * DESCRIPTION: coin-collecting problem
 ***********************************************************/
#include <stdio.h>
#include <iomanip>
#include <iostream>
#define width 6
#define height 5

using namespace std;

int larger_of(int a, int b){
    if (a > b){
        return a;
    }else{
        return b;
    }
}
int coin_prob(int board[][height]){
    int larger = 0;
    int matrix[width][height] = {};
    printf("Starting matrix:\n");
    for( int j =0; j < height; j++){
        for(int i = 0; i < width; i++){
            cout << setw(3) << setfill(' ') << board[i][j] << ',';
            matrix[i][j] = board[i][j];
        }
        cout << endl;
    }
    for(int i = 0; i < width; i++){
        for( int j =0; j < height; j++){
            bool block_right = true;
            bool block_down = true;
            if(board[i][j] == -1){
                //printf(" [%d,%d] is blocked\n", i, j);
                for(int k = j; k >= 0; k--){
                    if(board[i][k] != -1)
                        block_right = false;
                }
                for(int k = i; k >= 0; k--){
                    if(board[k][j] != -1)
                         block_down = false;
                }
                if(block_right){
                    //printf("blocking right\n");
                    for(int k = i; k < width; k++){
                        board[k][j] = -1;
                        matrix[k][j] = -1;
                    }
                }
                if(block_down){
                    //printf("blocking down\n");
                    for(int k = j; k < height; k++){
                        board[i][k] = -1;
                        matrix[i][k] = -1;
                    }
                }
            }else{
                //printf("not blocked\n");
                int left = (i<=0?0:matrix[i-1][j]);
                int up = (j<=0?0:matrix[i][j-1]);
                larger = larger_of(left, up);
                if( board[i][j] == 1){
                    larger = larger+1;
                }
                matrix[i][j] = larger;
            }
        }
    }

    printf("\n");
    printf("\n");
    printf("Ending Matrix:\n");
    for( int j =0; j < height; j++){
        for(int i = 0; i < width; i++){
            cout << setw(3) << setfill(' ') << matrix[i][j] << ',';
            //printf("%d,",matrix[i][j]);
        }
        cout << endl;
        //printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("Optimum path:\n");
    printf("(ties are broken by traveling right)\n");
    int j = height - 1;
    int i = width - 1;
    while(i >= 0 && j > 0 ){
        int left = (i<=0?0:matrix[i-1][j]);
        int up = (j<=0?0:matrix[i][j-1]);
        if( left > up){
            i = i - 1; //move left
            matrix[i][j] = '>';
        }else{// default traveling up
            j = j - 1; //move up
            matrix[i][j] = 'v';
        }

    }
    for( int j =0; j < height; j++){
        for(int i = 0; i < width; i++){
            if(matrix[i][j] == -1){
                cout << setw(3) << setfill(' ') << 'X' << ',';
            }else if(matrix[i][j] > 10){ // if it's a character, not a token, then print the character
                cout << setw(3) << setfill(' ') << (char)matrix[i][j] << ',';
            }else{
                cout << setw(3) << setfill(' ') << board[i][j] << ',';
            }
            //printf("%d,",matrix[i][j]);
        }
        cout << endl;
        //printf("\n");
    }
}



int main(){
    /* -1 = blocked
     * 0 = empty
     * 1 = token
    */
    // this is the board from the assignment
    int board[width][height] = {
        {0, 1, 0, 0, -1},
        {-1, 0, 1, 0, -1},
        {0, 0, 0, 0, -1},
        {1, -1, -1, 1, 0},
        {0, 1, 1, 0, 1},
        {0, 0, 0, 1, 0}
    };
    coin_prob(board);
}

/****************************************************

COMPILE:

gcc -Wall -o 9.out 9c.cpp

RUN:

./9.out

OUTPUT:
Starting matrix:
  0, -1,  0,  1,  0,  0,
  1,  0,  0, -1,  1,  0,
  0,  1,  0, -1,  1,  0,
  0,  0,  0,  1,  0,  1,
 -1, -1, -1,  0,  1,  0,


Ending Matrix:
  0, -1, -1, -1, -1, -1,
  1,  1,  1, -1, -1, -1,
  1,  2,  2, -1, -1, -1,
  1,  2,  2,  3,  3,  4,
 -1, -1, -1,  3,  4,  4,


Optimum path:
(ties are broken by traveling right)
  v,  X,  X,  X,  X,  X,
  >,  v,  0,  X,  X,  X,
  0,  >,  v,  X,  X,  X,
  0,  0,  >,  >,  >,  v,
  X,  X,  X,  0,  1,  0,

****************************************************/
