//Zane Durkin      Homework 2     Bruce Bolden
// File: hw2.cpp
// Date: 11/02/17

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "animals.h"
using namespace std;

int main(){
	// make animal object
	Animals animal;
	// make other variable
	ifstream file;
	string word; 
	string line;
	// load file
	cout << "Please enter the file you would like to read" << endl;
	cin >> line;
	file.open(line);

	// read first line of file to build stack
	getline(file, line);
	
	// generate starting stack
	//  break the first line up by word
	istringstream iss(line);
	//  for each word(animal) add it to the stack
	while(iss >> word){
		animal.addAnimal(word);
	}
	// give some padding between chooseing file and output
	cout << endl << endl;
	
	// print out the starting stack of animals
	animal.printAnimals();
	// give some more padding 
	cout << endl << endl;

	// now follow the rest of the file for commands
	while(getline(file, line)){
		// for everyline left in the file
		// break the line up by words
		istringstream iss(line);
		// if the line has words - continue
		if(iss >> word){
			// if the first word is delete or add
			if(word == "delete"){
				// get the second word in the line
				iss >> word;
				// tell the user the action being done
				cout << "deleting: " << word << endl;
				// delete the animal from the list
				animal.delAnimal(word);
			}else if(word == "add"){
				// get the second word in the line
				iss >> word;
				// tell the user the action being done
				cout << "adding: " << word  << endl;
				// add the animal to the list
				animal.addAnimal(word);
			}else{
				// if the given command is not know, then tell the user
				cout << "unknown command: " << word;
			}
			// after every command, print the new stack
			cout << endl << endl;
			animal.printAnimals();
			cout << endl << endl;
		}
	}
	// close the file when finished;
	file.close();
}
