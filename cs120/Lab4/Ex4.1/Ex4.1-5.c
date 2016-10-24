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
	// initialize variable to count loop iterations
	int i = 0;
	// ask for input
	cout << "Enter 100 numbers to find the sum" << endl;
	// ask five times for input and continuously find the total
	do{
		// get input
		cin >> num;
		if(num == 0){
			break;
		}
		// add input to total
		total+=num;
		i++; // increment i to stop the infinite loop
	}while(i < 100);
	// output the total	
	cout << "The Total is: " << total << endl;
}
