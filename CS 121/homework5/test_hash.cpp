/* TestHash.cpp
 *   Test the Hash table code.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "hash.h"
using namespace std;



int main(){  
      //  Put a few values in the table...
	ifstream file;
	string line;
	char * cLine;
	cout << "Please enter the data file you would like to use" << endl;
	cin >> line;
	file.open(line);
	
	while(!file.eof()){
		getline(file, line);
		cout << line << endl;
		//cLine = line.c_str();
		cLine = (char *)malloc(line.size() + 1);
		memcpy(cLine, line.c_str(), line.size() + 1);
		(void)Insert(cLine, cLine);
	} 
   
   PrintHashTable();
         
} 
