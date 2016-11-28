
#include <iostream>
#include <cstring>
using namespace std;

struct list{
	double score;
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
		j->score=score;
		j->prev=top;
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
		while(n->next != NULL){
			cout << n->name << ": " << n->score << endl;
			n = n->next;
		}
		//cout << n->name << ": " << n->score << endl;
	}

	void sort(){
		list * n = top;
		list * j;
		list * k;
		this->print();
		bool moved = true;
		while (moved){
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
			}
			while( (n->next != NULL) && (n != NULL) ){
				if(n->score < (n->next)->score){ // if the next element has a larger score, switch them
					moved = true;
					//"1->2   ;   1<-2->3   : 2<-3->4  ; 3<-4; n=2"
					(n->prev)->next=n->next;
					//"1->3   ;   1<-2->3   : 2<-3->4  ; 3<-4; n=2"

					j = (n->next)->next;
					//"1->3   ;   1<-2->3   : 2<-3->4  ; 3<-4; n=2; j=4"

					(n->next)->prev=n->prev;
					//"1->3   ;   1<-2->3   : 1<-3->4  ; 3<-4; n=2; j=4"

					n->prev=n->next;
					//"1->3   ;   3<-2->3   : 1<-3->4  ; 3<-4; n=2; j=4"

					(n->next)->next=n;
					//"1->3   ;   3<-2->3   : 1<-3->2  ; 3<-4; n=2; j=4"

					n->next=j;
					//"1->3   ;   3<-2->j   : 1<-3->2  ; 3<-4; n=2; j=4"

					((n->next)->next)->prev=n;
					//"1->3   ;   3<-2->j   : 1<-3->2  ; 2<-4; n=2; j=4"
					
					n=n->next;
					//"1->3   ;   3<-2->j   : 1<-3->2  ; 2<-4; n=j; j=4"

					//"1->3   ;   1<-3->2   : 3<-2->4  ; 2<-4; n=j; j=4"
					/*
					list * k = n;
					list * l = n->next;
					n = n->next;
					cout << n-> score << "<" << (n->next)->score << endl;
					n->next = k; 
					k->next = l;	
					*/
				}else{ // if they are equal or the current has a higher score, continue down the line
					n = n->next;
				}
			}
		}
	}	
};

int main(){
	stack stk;
	stk.push("jhon", 76.8, "Ac1");
	stk.push("Max", 55.7, "Sd20");
	//stk.push("Sue", 90.6, "Dd39");
	//stk.push("Max", 94.4, "Sr28");
	stk.print();
	stk.sort();
	stk.print();

}
