/* Zane Durkin
 * Lab 7
 * October 13 2016
 */

#include <iostream>
using namespace std;

// define global for array size
int SIZE = 25;

int sum (int arr[]){
	int k = 0;
	for(int i=0; i<SIZE; i++){
		k = k + arr[i];
	}
	return k;
}

float adverage(int total, int size){
	float adv = total / size;
	return adv;
}

void print_r(int arr[]){
	int k = 0;
	cout << "[";
	for(int i=0; i<SIZE-1; i++){
		cout << arr[i] <<", ";
	}
	cout << arr[SIZE-1]<< "]";
}

int main (){
	int nums[SIZE];
	// get all numbers
	cout << "Please enter "<< SIZE <<" integers: "<<endl;
	for (int i=0; i<SIZE; i++){
		cin >> nums[i];
	}
	print_r(nums);
	// find sum
	int total = sum(nums);
	// find adverage
	float adv = adverage(total, SIZE);
	// print info
	cout << endl << "The Sum is: " << total << endl;
	cout << "The adverage is: " << adv << endl;
	cout << "16th element is: " << nums[15] << endl;
	cout << "11th element is: " << nums[10] << endl;
	cout << "10,000th element is: " << nums[10000] << endl;	
}
