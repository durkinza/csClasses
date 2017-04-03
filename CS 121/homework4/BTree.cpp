/* Zane Durkin   Homework #4
 * Bruce Bolden
 * File: BTree.cpp
 * Date: 03/28/17
 */
#include "BTree.h"
#include <iostream>
#include <string>
#include <sstream>

BinaryTree::BinaryTree(){
	head = NULL;
}

BinaryTree::~BinaryTree(){
	_DeleteTree(head);
}

void BinaryTree::_DeleteTree(btree *tree){
	
	if(tree != NULL){
		_DeleteTree(tree->left);
		_DeleteTree(tree->right);
		delete tree;
	}
		tree = NULL;
}

void BinaryTree::printTitles(){
	// use a different function so that function can be recursive
	_printTitles(head, 0);
}
void BinaryTree::_printTitles(btree *tree, int spaces){
	if(tree!=NULL){
		_printTitles(tree->left, spaces+4);
		std::string space = "";
		for(int i = 0; i<spaces; i++){
			space = space+" ";
		}
		// use spaces to keep the tree formated correctly, and give curent height in tree
		std::cout << space << (spaces/4) << ": " << tree->title << std::endl<<std::endl;
		_printTitles(tree->right, spaces+4);
	}
}
void BinaryTree::printActorsOfShow(std::string show){
	// using seperate function 
	_printActorsOfShow(head, show);
}
void BinaryTree::_printActorsOfShow(btree *tree, std::string show){
	if(tree!=NULL){
		// if the given title is in or matches the current node
		if(tree->title.find(show) != std::string::npos){
			// print out all of the actors
			for(int i=0; i< tree->numOfActors; i++){
				std::cout << tree->actors[i] << std::endl;
			}
		}else{
			// printing in pre-order is ok, because only one show will be printed
			_printActorsOfShow(tree->left, show);	
			_printActorsOfShow(tree->right, show);
		}
	}
}

void BinaryTree::printShowsOfActor(std::string actor){
	// use a seperate function that can use recursion
	_printShowsOfActor(head, actor);
}

void BinaryTree::_printShowsOfActor(btree *tree, std::string actor){
	if(tree!=NULL){
		// print shows in pre-order form, this will keep the list alphabetical
		_printShowsOfActor(tree->left, actor);	
		// if the given title is in or matches the current node
		bool found = false;
		for(int i=0; i<tree->numOfActors; i++){
			if(tree->actors[i].find(actor) != std::string::npos){
				found = true;
			}
		}
		if(found){
			// print out all of the actors
			std::cout << tree->title << std::endl;
		}
		_printShowsOfActor(tree->right, actor);
	}
}


void BinaryTree::addNode(std::string title, std::string genre, std::string url, std::string actors[], int numOfActors){
	btree * newleaf = new btree;
	
	newleaf->title = title;
	newleaf->releaseYear = std::stoi(title.substr(title.find("(")+1, 4));
	newleaf->genre = genre;
	newleaf->url = url;
	newleaf->right = NULL;
	newleaf->left = NULL;
	for( int i=0; i<numOfActors; i++){
		newleaf->actors[i] = actors[i];
	}
	newleaf->numOfActors = numOfActors;
	if(head == NULL){
		head = newleaf;
	}else{
		btree * parentptr = head;
		btree * targetptr = parentptr;
		// go through tree till a spot is found
		while(targetptr != NULL){
			// see if the new title is lower in the alphabet that then current parent
			if(newleaf->title < targetptr->title){
				parentptr = targetptr;
				targetptr = targetptr->left;
			}else{
				// if the current title is higher thatn the current parent
				parentptr = targetptr;
				targetptr = targetptr->right;
			}
		}
		// see if node should be added to the left or the right
		if(newleaf->title < parentptr->title){
			parentptr->left = newleaf;
		}else{
			parentptr->right = newleaf;
		}
	}
}

void BinaryTree::_addNode(btree * tree, btree *data){
	if(tree == NULL){
		tree = data;
	}else if(data->title < tree->title){
		_addNode(tree->left, data);
	}else{		
		_addNode(tree->right, data);
	}
}
