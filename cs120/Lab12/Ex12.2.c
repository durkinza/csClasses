/* Zane Durkin
 * Lab12.2
 * 17 November 2016
 */

#include <iostream>
using namespace std;

struct que{
	int num;
	que * next;
};


class stack{
	public:
	que * top;// create a linked list to hold the stack
	stack(){
		top = (new que);
	}
	void append(int x){			// adds a value to the end of the que
		que * j = (new que);
		//j -> num = x;
		que * k = top;
		while(k->next != NULL){
			k = k->next;	
		}
		k->next = j;
		k->num = x;
			
	}
	int remove(){
		int x = top->num;
		top = top->next;	
		return x;
	}
	bool empty(){
		if(top->next == NULL){
			return true;
		}else{
			return false;
		}
	}
	void print(){ // better function for printing list
		que * n = top;
		while(n->next != NULL){
			cout << n->num;
			n = n->next;
		}
		cout << endl;
	}
};

int main(){
	stack stk;
	stk.append(1);
	stk.append(2);
	stk.append(3);
	stk.append(4);
	stk.append(7);
	stk.append(8);
	stk.remove();
	stk.remove();	
	stk.append(5);
	stk.append(6);
	
	while(!stk.empty()){
		cout <<  stk.remove();
	}
	cout << endl;
}
