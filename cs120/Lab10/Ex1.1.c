/* Zane Durkin
 * Lab10
 * November 3 2016
 */

#include <iostream>
#include <fstream>
using namespace std;

string complementBase(char a){
	// find the given letter and return the correct string
	switch(a){
			case 'A':
				return "T   ";
				break;
			case 'C':
				return "G   ";
				break;
			case 'G':
				return "C   ";
				break;
			case 'T':
				return "A   ";
				break;
		}
}

void DrawDNA(string line){
	// start with buffer spaces
	string response1 = "  ";
	string response2 = "  ";
	string response3 = "  ";
	string response4 = "-"; 
	// for every character, get the compelemnt and build the footer
	for(int i = 0; i < line.size(); i++){
			response1 += ".   ";
			response2 += complementBase(line[i]);
			response3 += "|   ";
			response4 += "-+--";
	}
	// output everything
	cout << response1 << endl << response2 << endl << response3 << endl << response4 << endl;
}

void DrawHeader(string str){
	// get the size of the given string
	int count = str.size();
	string response1 = "-";
	string response2 = "  ";
	string response3 = "  ";
	// for each letter, print a set of header stuff
	for(int i = 0; i<count; i++){
		response1 += "-+--";
		response2 += "|   ";
		response3 += str[i];
		response3 += "   ";
	}
	// output everything
	cout << response1 << endl << response2 << endl << response3 << endl;
}

void prefix(int count, string list, string& prefix){
	for(int i=0; i<count; i++){
		prefix += list[i];
	}
}

int main(){
	// get file
	char fileName[40];
	fstream file;
	cout << "Enter File Name:" << endl;
	cin >> fileName;
	file.open(fileName);
	string line;
	// draw dna
	getline(file, line);
	DrawHeader(line);
	DrawDNA(line);
	// get prefix of line
	string pre;
	prefix(4, line, pre);
	cout << "Prefix is: " << pre << endl;
}
