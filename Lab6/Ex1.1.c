/*
 * Zane Durkin
 * Lab6
 * October 6 2016
 */

#include <iostream>
using namespace std;
int function1(int x, int &y);
int function2(int b, int &x);
int g = 4;
int main() {
	int a = 1;
	int b = 5;
	int x = 0;
	cout << "Main 1: a = " << a << " b = " << b;
	cout << " g = " << g << " x = " << x;
	cout << endl << endl << endl << endl << endl;
	{
		int a = 7;
		g = 10;
		cout << "Main 2: a = " << a << " b = " << b;
		cout << " g = " << g << " x = " << x;
		cout << endl << endl << endl << endl << endl;
	}
	x = function1(a,b);
	cout << "Main 3: a = " << a << " b = " << b;
	cout << " g = " << g << " x = " << x;
	cout << endl << endl << endl << endl << endl;
	x = function2(a,b);
	cout << "Main 4: a = " << a << " b = " << b;
	cout << " g = " << g << " x = " << x;
	cout << endl << endl << endl << endl << endl;
}

int function1(int x, int &y) {
	int a;
	a = x-y;
	g = x+y;
	y = a+g;
	cout << "Function1: a = " << a << " x = " ;
	cout << x << " g = " << g;
	cout << endl << endl << endl << endl << endl;
	return a;
}

int function2(int b, int &x) {
	int temp;
	temp = b;
	b = x;
	x = temp;
	temp = function1(b,x);
	g = temp*x;
	return temp;
}
