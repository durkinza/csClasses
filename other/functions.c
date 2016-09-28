#include <iostream>
using namespace std;

bool getBool(){
    return true;
}

int main(){

    int x=5;
    int y=3;

    cout << add(x, y);
    x =8;
    y =7;
    bool add = getBool();
    if(add){
	add(x, y);
    }else{
        cube(x, y);
    }
}


int add (x, y){
    int z = x+y;
    return z;
}

int cube(x, n){
    int z = x;
    for(int i=2; i<n; i++){
        x = x*z;
    }
    return x;
}
