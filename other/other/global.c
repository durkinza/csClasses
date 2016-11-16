#include <iostream>
using namespace std;

int c = 10;

void foo(int &x){
    x++;
    c++;
}

int main(){
    int c = 20;
    foo(c);
    cout << c;
}
