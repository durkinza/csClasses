/* Zane Durkin, Chase Ogle, Delaney Fitzgerald
 * 29 September 2016
 * Lab5 
 */

#include <iostream>
#include <math.h>

using namespace std;

int Nnum(){
	// Zane Durkin
	float x; // create number holder
	bool asking = true; // boolean to escape loop
	do{
		cout << "Please enter a natural number" << endl;
		cin >> x;
		if(x <= 0){
			cout << "Use a number larger than 0" << endl;
			continue;
		}
		if( floorf(x) == x)
			asking = false;  // it is an integer
		else{
			cout << "That is not a natural number" << endl;
			continue;
		}
	}while(asking);

	return x;
}
void output(int i){
        //Delaney Fitzgerald

        cout << i << endl;

}
int mult(int a, int b){
	// Chase Ogle
    int product;
    product= a*b;
	return product;
}

int main(){
	int x,y,m;
	x = Nnum();
	y = Nnum();
	cout << "entered numbers are: " << endl;
	output(x);
	output(y);
	cout << "The Product is: " << mult(x,y) << endl;
}
