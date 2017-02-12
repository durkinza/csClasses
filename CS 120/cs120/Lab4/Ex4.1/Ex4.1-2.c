/* Zane Durkin
 * Lab4
 * 22/9/16
 */

#include <iostream>
using namespace std;

int main(){
	// declare variable
	int num;
	// initialize variable to hold the total value
	int total = 0;
	// ask for input
	cout << "Enter 5 numbers to find the sum" << endl;
	// ask five times for input and continuously find the total
	for(int i=0; i<5; i++){
		// get input
		cin >> num;
		// add input to total
		total+=num;
	}
	// output the total	
	cout << "The Total is: " << total << endl;
}
