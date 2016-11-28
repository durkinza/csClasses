
#include <iostream>
#include <cstring>
#include <math.h>
using namespace std;

struct list{
	int score;
	string name;
	string sid;
	list * next;
	list * prev;
};


class stack{
	public:
	list * top;// create a linked list to hold the stack
	stack(){
		top = (new list);
	}
	void push(string name, double score, string sid){			// adds a value to the list
		list * j = (new list);
		j->next = top;
		j->name=name;
		j->score=(int)ceil(score);
		(j->next)->prev = j;
		//j->prev=top;
		j->sid=sid;
		top = j;	
	}
	void pop(){
		top = top->next;	
	}
	bool empty(){
		if(top == NULL){
			return true;
		}else{
			return false;
		}
	}
	void print(){ // better function for printing list
		list * n = top;
		cout << "+-------+-----------+-------+--------+" << endl;
		cout << "| Grade | StudentId | Score |  Name  |" << endl;
		cout << "+-------+-----------+-------+--------+" << endl;
		while(n->next != NULL){
			switch(n->score){
				case 91 ... 200: cout<<"   A   ";
					break;
				case 81 ... 90: cout<<"   B   ";
					break;
				case 71 ... 80: cout<<"   C   ";
					break;
				case 61 ... 70: cout<<"   D   ";
					break;
				default: cout<<"   F   ";
			}
			cout << "     " << n->sid << "       " << n->score << "      " << n->name << endl;
			n = n->next;
		}
		//cout << n->name << ": " << n->score << endl;
	}

	int sort(){
		list * n = top;
		list * j;
		list * k;
		bool moved = true;
		while (moved){
			n = top;
			moved = false;
			if(n->score < (n->next)->score){ // move first element if necessary. (this is different due to the lack of a prev
				moved =true;
				k = n->next;
				n->prev=n->next;
				n->next = n->next->next;
				n->next->prev = n;
				// here k is only pointer left for n[2]
				k->prev = NULL;
				k->next = n;
				top = k;
			}
			while( (n->next)->next != NULL ){
				if(n->score < (n->next)->score){ // if the next element has a larger score, switch them
					moved = true;
					(n->prev)->next=n->next;
					j = (n->next)->next;
					(n->next)->prev=n->prev;
					n->prev=n->next;
					(n->next)->next=n;
					n->next=j;
					(n->next)->prev=n;
				}else{ // if they are equal or the current has a higher score, continue down the line
					if (n->next != NULL){
						n = n->next;
					}
				}
			}
		}
	}	
};

int main(){
	stack stk;
	stk.push("jhon",     76.8, "Ac21" );
	stk.push("Max1",     55.7, "Sd20");
	stk.push("Sue",      90.6, "Dd39");
	stk.push("Max2",     94.4, "Sr28");
	stk.push("Max3",     86.8, "Td44");
	stk.push("John",     98.0, "Rf31");
	stk.push("Max",      93.5, "Fd22");
	stk.push("Alan",     79.8, "Tg34");
	stk.push("Derby",    68.0, "Er42");
	stk.push("Steve",    55.7, "Yh29");
	stk.push("Jonathan", 81.3, "Rf52");
	stk.push("Shawn",    84.9, "Rg73");
	stk.push("Dercy",    75.4, "Hy38");

	stk.sort();
	stk.print();

}
