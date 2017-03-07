// Zane Durkin      Homework #3     Bruce Bolden
// File: Line.cpp
// Date: 03/06/17

#include "Line.h"
#include <iostream>
#include <string>
#include <sstream>


Line::Line(int transWait){
	// for a basic constructor, just make the head pointer
	head = NULL;
	changed = false;
	lineWait = transWait;
}

Line::~Line(){
	// delete all nodes upon deconstruction
	customer *n;
	customer *p = head;
	while (p != NULL){
		n = p;
		p = p->next;
		delete n;
	}
}

void Line::addCust(int eT, std::string name, char act, int trn){
	// to add a new customer, first make a new node
	customer *a = new customer();
	// fill in the node's fields
	a->next = head;
	a->entryTime = eT;
	a->name = name;
	a->activity = act;
	a->transactions = (trn * lineWait);
	// move head to start at new node
	head = a;
	changed = true;
}

void Line::addCust(customer *cust){
	// for adding a customer object to the queue
	cust->next = head;
	head = cust;
	changed = true;
}

void Line::delCust(){
	// for removing a customer from the front of the queue

	// to remove the customer, first check that the queue isin't empty
	if(head != NULL){
		customer *c = head;
		if(c->next == NULL){ // if this is true, then there must only be the one element in the list
			delete head;
			head = NULL; // so set the list to empty
		}else{ // if this, then there must be another node to move to
			// create variables to use for searching
			customer *p = c;
			c = c->next;
			// loop though to the front of the queue
			while( c->next != NULL ){
				p = c;
				c = c->next;
			}
			// release the memory
			delete c;
			// remove the node from the queue
			p->next = NULL;
		}
		changed = true;	
	}else{
		// if the queue is empty let the user know
		std::cout << std::endl << "Empty Line";
	}
}

void Line::nextTrans(){
	customer *c = head;
	// this function will decrement the number of transactions the first customer has left to complete
	if(head != NULL){
		while(c->next != NULL){
			c = c->next;
		}
		// decrement the number of transactions left.
		c->transactions--;
		// if no more transactions are left, remove the customer
		if(c->transactions <= 0){
			delCust();
		}
	}
}

void Line::printLine(){
	// to print out line, first make a pointer to transverse the stack	
	customer *c;
	c = head;
	// make sure the stack is not empty
	if(head != NULL){
		// go down the stack and output the lines;
		std::cout << std::endl;
		while(c != NULL){
			std::cout << "| " << c->name << ":" << c->transactions << ":" << c->activity << " ";
			c = c->next;
		} 
	}else{
		// if the stack is empty, tell the user
		std::cout << std::endl << "| Empty Line";
	}
}

int Line::countLine(){
	// create first pointer to transverse the stack with
	customer * a = head;
	// create a counter to use for counting
	int counter = 1;
	// first check that the stack isint empty
	if(head != NULL){
		// for every node in the stack, try to count
		while(a->next != NULL){
			// start the counting at the current node in the stack
			counter++;
			// transverse to the next customer in a queue
			a = a->next;
		}// end while
		return counter;
	}else{
		// if the stack is empty, let the user know
		return 0;
	}
}

Line::customer * Line::popLine(){
	// for removing a customer from the end of the queue
	// make a pointer to hold end element
	customer *a = head;
	// get the next element
	head = head->next;
	// say that something has changed
	changed = true;
	// return the element removed
	return a;	
}
