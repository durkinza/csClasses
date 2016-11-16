/* Zane Durkin
 * Lab11
 * 10 November 2016
 */

#include <iostream>
using namespace std;

struct user{
	char name[40];
	int id;
};

typedef struct user usr;

usr addusr(){
	usr user;
	cout << "please enter the user's name" << endl;
	cin >> user.name;
	cout << "Please enter the user's ID number" << endl;
	cin >> user.id;
	return user;
}

void printusr(usr user){
	cout << "Username: " << user.name << " ID: " << user.id << endl;
}

int main(){
	int i;
	usr users[10]; // declare users array
	for(i=0; i < 10; i++){ // create every user
		users[i] = addusr();
	}
	for(i=0; i < 10; i++){ // print every user
		printusr(users[i]);
	}
}
