/***********************************************************
 * NAME: Zane Durkin
 * ID: durk7832
 * QUESTION: Asn2.5
 * DESCRIPTION: Russian peasent mult.
 ***********************************************************/
#include <iostream>
#include <math.h>       /* floor */
using namespace std;


int mult(int one, int two){
    int tmp1 = one, tmp2 = two, i=0;
    int arr[one][2];
    int out = 0;
    while(tmp1 >= 1){
        arr[i][0] = tmp1;
        arr[i][1] = tmp2;
        tmp1 = floor(tmp1 / 2);
        tmp2 = tmp2 * 2;
        if(tmp1%2 != 0){
            out += tmp2
        }
        i++;
    }
    printf("total: %d\n", out);
    out = tmp2;
    for(int j = 0; j<i-2; j++){
        printf("count: %d\n", arr[j][1]);
        if(arr[j][0] % 2 != 0){
            out += arr[j][1];
        }
    }
    return out;
}

int main(){
    printf("mult: %d\n",mult(146, 37));
}
