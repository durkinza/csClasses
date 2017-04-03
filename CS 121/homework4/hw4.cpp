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
//	BTree.printTitles();
//	BTree.printActorsOfShow("M*A*S*H");
	BTree.printShowsOfActor("Nancy Cartwright");
}
