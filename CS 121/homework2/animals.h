// Zane Durkin      Homework #2     Bruce Bolden
// File: animals.h
// Date: 11/02/17

#include <string>
class Animals{
	public:
		Animals();
		Animals(std::string anml);
		~Animals();
		void addAnimal(std::string anml);
		void delAnimal(std::string anml);
		void printAnimals();
	private:
		struct animal{
			animal *next;
			std::string type;
		};
		animal *head;
};
