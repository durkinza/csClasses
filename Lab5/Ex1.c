/* Zane Durkin, Chase ogle, Delaney Fitzgerald
 * 
 * 
 */

#include <iostream>
#include <math.h>
using namespace std;
int Nnum();

int main(){
	cout << Nnum();
}

int Nnum(){
	float x;
	bool asking = true;
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
