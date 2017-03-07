// Zane Durkin      Homework #3     Bruce Bolden
// File: line.h
// Date: 03/06/17

#include <string>
class Line{
	public:
		struct customer{
			customer *next;
			int entryTime;
			std::string name;
			char activity;
			int transactions;
		};
		customer *head;
		typedef customer customer;
		int lineWait;
		bool changed;
		// start making function declarations
		Line(int transWait);
		~Line();
		void addCust(int ET, std::string name, char act, int Trn);
		void addCust(customer *cust);
		void nextTrans();
		void delCust();
		customer * popLine();			
		void printLine();
		int countLine();
};
