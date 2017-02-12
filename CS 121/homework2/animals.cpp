// Zane Durkin      Homework #2     Bruce Bolden
// File: animals.cpp
// Date: 11/02/17

#include "animals.h"
#include <iostream>
#include <string>

Animals::Animals(){
	// for a basic constructor, just make the head pointer
	head = NULL;
}

Animals::Animals(std::string anml){
	// if there is an animal given with construction, add it the stack
	animal *a = new animal();
	a->type = anml;
	a->next = NULL;
	head = a;
}

Animals::~Animals(){
	// delete all nodes upon deconstruction
	animal *n;
	animal *p = head;
	while (p != NULL){
		n = p;
		p = p->next;
		delete n;
	}
}

void Animals::addAnimal(std::string anml){
	// to add a new animal, first make a new node
	animal *a = new animal();
	// fill in the node's fields
	a->type = anml;
	a->next = head;
	// move head to start at new node
	head = a;
}

void Animals::delAnimal(std::string anml){
	// to delete an animal, first check that the stack isin't empty
	if(head != NULL){
		// create variables to use for searching
		animal *c = head;
		animal *p;
		// if the animal is found at the start of the stack, 
		//	just move the header down
		if(c->type == anml){
			head = c->next;
			delete c;
		}else{
			// if the animal is found further down the stack, 
			//	loop through the stack until the animal is found or
			//	you've reached the end of the stack
			while( c->next != NULL ){
				p = c;
				c = c->next;
				// if the animal is found
				if(c->type == anml){
					// remove the node from the stack
					p->next = c->next;
					// disconect the rest of the stack from the node
					c->next = NULL;
					// release the memory
					delete c;
					// break out of the loop and stop searching
					break;
				}
			}
		}
	}else{
		// if the stack is empty let the user know
		std::cout << "Empty Stack" << std::endl;
	}
}

void Animals::printAnimals(){
	// to print out animals, first make a pointer to transverse the stack	
	animal *c;
	c = head;
	// make sure the stack is not empty
	if(head != NULL){
		// go down the stack and output the animals;
		while(c != NULL){
			std::cout << c->type << std::endl;
			c = c->next;
		}
	}else{
		// if the stack is empty, tell the user
		std::cout << "Empty Stack" << std::endl;
	}
}
