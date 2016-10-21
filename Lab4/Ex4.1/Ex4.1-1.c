/* Zane Durkin
 * Lab4
 * 22/9/16
 */

#include <iostream>
using namespace std;

int main(){
	// declare variables
	int a;
	int b;
	int c;
	int d;
	int e;
	// one more variable to hold the total value
	int total;
	// ask for input
	cout << "Enter 5 numbers to find the sum" << endl;
	cin >> a >> b >> c >> d >> e;
	// get the total
	total = a+b+c+d+e;
	// output the total	
	cout << "The Total is: " << total << endl;
}
