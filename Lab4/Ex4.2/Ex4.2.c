/* Zane Durkin
 * Lab 4.2
 * 22/9/16
 */

#include <iostream>
using namespace std;

int main(){
	// build chart topper
	cout << "   0 1 2 3 4 5 6 7 8 9" << endl;

	// for every row, 
	for(int i=40; i<111; i+=10){
		// output the value for the column
		cout << i;
		for(int j=0; j<10; j++){
			cout << ' ' << static_cast<char>(i+j);
		}
		cout << endl;
	}
}
