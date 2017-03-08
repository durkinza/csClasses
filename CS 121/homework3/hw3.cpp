/* Zane Durkin
 * Bruce Bolden CS 121 
 * Homework 3
 * 28 February 2017
 */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Line.h"
using namespace std;

// creating global variables
// create the lines
Line Eline(2);
Line line2(4);
Line line3(4);

// function for switching lines, overloaded to hand different line types
void switchLines(Line::customer *cus, int lineTo, int lineFrom);
void checkLines();
void addToShortest(bool elig, int ETA, string name, char act, int trans);

int main(){
	// create the counter for the clock. this will be incremented as a clock cycle
	int clock = 1;
	// create some variables
	bool changed = false;
	bool eligible = false;	

	// variables to hold customer info
	int number;
	string name;
	char act;
	int trans;
	// include data file
	ifstream file;
	string line;
	cout << "Please enter the data file you would like to use" << endl;
	cin >> line;
	file.open(line);
	// get first line to start
	getline(file, line);
	istringstream iss;
	iss.str(line);
	iss >> number;
	// continue until we have reached the end of the simulation
	while( ( !file.eof() ) || ( Eline.countLine()>0 ) || ( line2.countLine()>0 ) || ( line3.countLine()>0 ) ){
		cout << "clock: " << clock << endl;
		while(!file.eof()){
			//	see if customer can be added yet, if not, break out and wait
			if(number <= clock){
				changed = true;
				iss >> name >> act >> trans;
				cout << endl << "Adding: " << name << " " << act << " " << trans << endl;
				// see if the customer can go in the express lane
				if(act == 'C' || act =='D'){
					// these activities are able to use the express line
					eligible = true;
				}
				addToShortest(eligible, number, name, act, trans);
				// reset defauit eligiblility 
				eligible = false;
				// get next customer ready
				// get line
				getline(file, line);
				// make sure you are not just at the end of the file
				if(!file.eof()){
					// turn the new line into a string stream
					iss.str("");
					iss.clear();
					iss.str(line);
					// get the next number ready
					iss >> number;
				}else{
					break;
				}
			}else{
				break;
			}
		} // end while
		// tell each line to complete one transaction
		Eline.nextTrans();
		line2.nextTrans();
		line3.nextTrans();
		// look to see if any customers should move
		checkLines();
		// print out lines if something has changed
		if(changed || Eline.changed || line2.changed || line3.changed){
			Eline.printLine();
			line2.printLine();
			line3.printLine();
			cout << endl << endl;
			// set all changes back to false
			changed = false;
			Eline.changed = false;
			line2.changed = false;
			line3.changed = false;
		}
		// increment clock
		clock++; 

	}// end while
	// say that program ended well
	return 0;
}

// function for switching lines;
void switchLines(Line::customer *cus, int lineTo, int lineFrom){
	// if they want switch to express line, make sure the have the correct activity
	if(lineTo == 1){
		if(cus->activity == 'C' || cus->activity == 'D'){
			Eline.addCust(cus);
		}else{
			// if they can't, move them back to the line they came from
			if(lineFrom == 2){
				line2.addCust(cus);
			}else if(lineFrom == 3){
				line3.addCust(cus);
			}
		}
	// if they want to move to line2, move them
	}else if(lineTo == 2){
		line2.addCust(cus);
	// if they want to move to line3, move them
	}else if(lineTo == 3){
		line3.addCust(cus);
	}
	
}
// function for seeing if a line needs to switch;
void checkLines(){
	// variable to hold cutomer
	Line::customer *cus;

	// if express line is empty, see if someone wants to move	
	if(Eline.countLine() == 0){
		// see if another line has extra people
		if(line2.countLine() >= 2){
			// switch them from line 2 to express line
			switchLines(line2.popLine(), 1, 2);
		}else if(line3.countLine() >= 2){
			// switch them from line 3 to express line
			switchLines(line3.popLine(), 1, 3);
		}
	}

	// if line 2 is empty, see if someone wants to move
	if(line2.countLine() == 0){
		// see if other lines have extra people
		if(Eline.countLine() >= 2){
			switchLines(Eline.popLine(), 2, 1);
		}else if(line3.countLine() >= 2){
			switchLines(line3.popLine(), 2, 3);
		}
	}

	// if line 3 is empty, see if someone wants to move
	if(line3.countLine() == 0){
		// see if other lines have extra people
		if(Eline.countLine() >= 2){
			switchLines(Eline.popLine(), 3, 1);
		}else if(line2.countLine() >= 2){
			switchLines(line2.popLine(), 3, 2);
		}
	}
	
}


// function for finding the shortest line
void addToShortest(bool elig, int ETA, string name, char act, int trans){
	int l1=Eline.countLine();
	int l2=line2.countLine();
	int l3=line3.countLine();
	if(elig){
		if( (l1 <= l2) && (l1 <= l3) ){
			Eline.addCust(ETA, name, act, trans);
			return;
		}
	}
	if( l2 <= l3){
		line2.addCust(ETA, name, act, trans);
	}else{
		line3.addCust(ETA, name, act, trans);
	}
	
}

