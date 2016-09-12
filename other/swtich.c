#include <iostream>
using namespace std;

int main(){
    //int score = 90;
    int score;
    cin >> score;
    switch(score){
	case 90 ... 200: cout<<"A"<<endl;
		break;
	case 80 ... 89: cout<<"B"<<endl;
		break;
	case 70 ... 79: cout<<"C"<<endl;
		break;
	case 60 ... 69: cout<<"D"<<endl;
		break;
	default: cout<<"F"<<endl;
    }
}
