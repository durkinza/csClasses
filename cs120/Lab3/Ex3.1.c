/* Lab3 Zane Durkin
 * 09/15/16
 */

#include <iostream>
using namespace std;

int main(){
	int i; int j; 								// create variables to hold values
	cout << "Please enter a number" << endl; 	// ask the user for input
	cin >> i;									// get the input
	cout << "One more time." << endl;			// ask for another number
	cin >> j;
	if(i<j){
		cout << i << " (i) is smaller" << endl;	// tell the user if i is smaller
	}else if(j<i){
		cout << j << " (j) is smaller" << endl; // tell if j is smaller
	}else{
		cout << i << "=" << i << " these numbers are the same" << endl; // tell the user that the numbers are the same
	}
}
