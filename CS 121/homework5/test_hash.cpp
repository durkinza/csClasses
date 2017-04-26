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
		//cout << line << endl;
		cLine = (char *)malloc(line.size() + 1);
		memcpy(cLine, line.c_str(), line.size() + 1);
		(void)Insert(cLine, cLine);
	} 
   
   PrintHashTable();
	    
	search( "zymotechnics" );
	search( "superdiabolically" );
	search( "blepharosphincterectomy" );
	search( "unmisinterpretable" );
	search( "tennis" );
	search( "classy");
	search( "machinotechnique");
	search( "breakfast");
	search( "cucumber");
	search( "fishy");
	search( "coffeetime");

	return EXIT_SUCCESS;
} 
