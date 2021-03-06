/* Zane Durkin
 * Lab12.1
 * 17 November 2016
 */

#include <iostream>
using namespace std;

struct list{
	char letter;
	list * next;
};


class stack{
	public:
	list * top;// create a linked list to hold the stack
	stack(){
		top = (new list);
	}
	void push(char x){			// adds a value to the list
		list * j = (new list);
		j->next = top;
		j->letter=x;
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
		while(n->next != NULL){
			cout << n->letter;
			n = n->next;
		}
		cout << n->letter << endl;
	}
	void sort(){
		list * head = top;
		list * curr = top;
		list * x;
		bool moved = true;

		while(moved){
			moved = false;
			if(curr->letter > curr->next->letter){ // if the letters are out of order
				x = curr;
				curr = curr->next;
				curr->next = x;
				delete x;
			}
		}
			
	}
};

int main(){
	stack stk;
	stk.push('b');
	stk.push('v');
	stk.push('d');
	stk.push('g');
	stk.push('r');
	stk.push('n');
	stk.push('g');
	
	while(!stk.empty()){
		cout << stk.top->letter;
		stk.pop();
	}
	cout << endl;

}
