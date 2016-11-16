/* Zane Durkin
 * Lab 9
 * October 26 2016
 */

#include <iostream>
#include <cstring>
using namespace std;
void funcStrlen(){
	char test_string[] = "Test String";
	cout << "The strlen() function returns ";
	cout << "the length of a string" << endl;
	cout << "E.g. the length of the string: *";
	cout << test_string << "* is " << strlen(test_string);
	cout << "." << endl;
}

void funcStrcpy(){
	char test_string[] = "Test String";
	char copy_string[40];
	cout << "The strcpy() function returns ";
	cout << "a copy of a string" << endl;
	cout << "E.g. a copy of "<< test_string <<" is the string: *";
	strcpy(copy_string, test_string);
	cout << copy_string;
	cout << "*." << endl;
}

void funcStrcat(){
	char test_string1[] = "Test";
	char test_string2[] = "String";
	cout << "The strcat() function returns ";
	cout << "a concatinated version of the two strings given to it "<< endl;
	cout << "E.g. a concatted string of *" << test_string1 << "* and *" << test_string2<< "*" << endl;
	strcat(test_string1, test_string2);
	cout << "would be *" << test_string1 << "*.";
	
}
void funcStrcmp(){
	char test_string1[] = "Test String";
	char test_string2[] = "Testing String";
	cout << "The strcmp() function returns ";
	cout << "the character value difference of the first different letter in the two given strings "<< endl;
	cout << "E.g. The difference of the first different letter in *" << test_string1 << "* and in *" << test_string2 << "*" << endl;
	cout << "would be at l=" << strcmp(test_string1, test_string2) << endl;
	cout << "where l>1 means that the first character that does not match is greater in string 1 and" << endl;
	cout << "where l<1 means that the first character that does not match is greater in string 2";
}
void funcStrstr(){
	char test_string[] = "A Test String that is longer";
	char small_string[] = "Test String";
	cout << "The strstr() function returns";
	cout << " a pointer to the location of a string within a larger string" << endl;
	cout << "E.g. The pointer to *" << small_string << "* within *" << test_string << "*" << endl;
	cout << "would be " << (void *)strstr(test_string, small_string)  << endl;
}

void funcStrings(){
	string test_string1 = "Test String";
	string test_string2 = "testing string";
	string short_string1 = "Test ";
	string short_string2 = "String";
	string new_string;
	cout << "The length of *" << test_string1 << "* is *" << test_string1.size() << "* " << endl;
	cout << "The comparison of *" << test_string1 << "* and *" << test_string2 << "* is *" << (test_string1 == test_string2) << "* " << endl;
	cout << "The concatination of *" << short_string1 << "* and *" << short_string2 << "* is *" << (short_string1 + short_string2) << "*" << endl;
	new_string = test_string1;
	cout << "The copy of *" << test_string1 << "* is *" << new_string << "*" << endl;
	cout << "You can find *" << short_string2 << "* in *" << test_string1 << "* starting at *" << test_string1.find(short_string2) << "*" ;
}


int main(){
	funcStrlen();		
	cout << endl;
	cout << endl;
	funcStrcpy();
	cout << endl;
	cout << endl;
	funcStrcat();
	cout << endl;
	cout << endl;
	funcStrcmp();
	cout << endl;
	cout << endl;
	funcStrstr();
	cout << endl;
	cout << endl;
	cout << endl;
	funcStrings();
	cout << endl;
	return 0;
}

