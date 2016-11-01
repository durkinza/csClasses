/* Zane Durkin
 * Lab10
 * November 2
 */

#include <iostream>
#include <fstream>
using namespace std;

bool isData(string line){
	bool data = false;
	for(int i=0; i < line.size(); i++){
		if(line[i] == 'A' || line[i] == 'C' || line[i] == 'G' || line[i] == 'T'){
			data = true;
			break;
		}else if(line[i] == '-' || line[i] == '|'){
			break;
		}
	}
	return data;
}

void DrawDNA(string line){
	string response1 = "  ";
	string response2 = "  ";
	string response3 = "  ";
	string response4 = "-"; 
	for(int i = 0; i < line.size(); i++){
		switch(line[i]){
			case 'A':
				response1 += ".   ";
				response2 += "T   ";
				response3 += "|   ";
				response4 += "-+--";
				break;
			case 'C':
				response1 += ".   ";
				response2 += "G   ";
				response3 += "|   ";
				response4 += "-+--";
				break;
			case 'G':
				response1 += ".   ";
				response2 += "C   ";
				response3 += "|   ";
				response4 += "-+--";
				break;
			case 'T':
				response1 += ".   ";
				response2 += "A   ";
				response3 += "|   ";
				response4 += "-+--";
				break;
		}
	}
	cout << response1 << endl << response2 << endl << response3 << endl << response4 << endl;
}

void prefix(int count, string list, string& prefix){
	for(int i=0; i<count; i++){
		prefix += list[i];
	}
}

int main(){
	char fileName[40];
	fstream file;
	cout << "Enter File Name:" << endl;
	cin >> fileName;
	file.open(fileName);
	string line;
	while(getline(file, line)){
		cout << line << endl;
		if(isData(line)){
			DrawDNA(line);
		}
	}
}
