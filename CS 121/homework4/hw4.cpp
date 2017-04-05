/* Zane Durkin
 * Bruce Bolden CS 121
 * Homework 4
 * 28 March 2017
 */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "BTree.h"
using namespace std;

// create global binary tree object
BinaryTree BTree;

// list functions

int main(){
	// get data file
	ifstream file;
	string line;
	cout << "Please enter the data file you would like to use" << endl;
	cin >> line;
	file.open(line);
	
	string title;
	string genre;
	string url;
	int j = 0;
	while(!file.eof()){
		//cout << "getting movie" << endl;
		getline(file, title);
		// check that the next line has info before continuing
		while(title==""){
			getline(file, title);
		}
		// get the genre
		getline(file, genre);
		// get url
		getline(file, url);
		// setup temporary variables to hold actors
		int numOfActors=0;
		string actor[20];
		// get every actors until there are no more actors to get
		do{
			getline(file, line); // get the next line
			if(line != ""){
				//cout << "    getting actor" << endl;
				actor[numOfActors++] = line;
			}
		// continue to get actors untill there are no more
		}while( (line != "") && (numOfActors<20) );
		BTree.addNode(title, genre, url, actor, numOfActors);
	}
BTree.printTitles();
cout << endl << endl;
cout << "The actors of \"The Saint\"" << endl;
BTree.printActorsOfShow("The Saint");
cout << endl << "The actors of \"Benson\"" << endl;
BTree.printActorsOfShow("Benson");
cout << endl << "The actors of \"Dexter's Laboratory\"" << endl;
BTree.printActorsOfShow("Dexter's Laboratory");
cout << endl << endl << "Shows that Nancy Cartwright was an actress in:" << endl;
BTree.printShowsOfActor("Nancy Cartwright");
cout << endl << endl << "Shows that Marcia Wallace was an actresss in:" << endl;
BTree.printShowsOfActor("Marcia Wallace");
cout << endl << endl << "Shows that Bill Daily was an actor in:" << endl;
BTree.printShowsOfActor("Bill Daily");
cout << endl << endl << "Shows that Joe Penney was an actor in:" << endl;
BTree.printShowsOfActor("Joe Penny");
cout << endl << endl << "Shows that Leonard Nimoy was an actor in:" << endl;
BTree.printShowsOfActor("Leonard Nimoy");
cout << endl << endl << "Shows that Richard Dean Anderson was an actor in:" << endl;
BTree.printShowsOfActor("Richard Dean Anderson");
cout << endl << endl << "All shows between 1965 and 1975:" << endl;
  BTree.printShowsOfYears(1965, 1975);
cout << endl << endl << "All shows between 1955 and 1965:" << endl;
  BTree.printShowsOfYears(1955, 1965);
}
