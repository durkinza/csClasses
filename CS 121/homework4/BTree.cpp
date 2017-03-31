/* Zane Durkin   Homework #4
 * Bruce Bolden
 * File: BTree.cpp
 * Date: 03/28/17
 */
#include "BTree.h"
#include <iostream>
#include <string>

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
	std::cout << "starting print" << std::endl;
	_printTitles(head, 0);
}
void BinaryTree::_printTitles(btree *tree, int spaces){
	if(tree!=NULL){
		_printTitles(tree->left, spaces+4);
		std::string space = "";
		for(int i = 0; i<spaces; i++){
			space = space+" ";
		}
		std::cout << space << (spaces/4) << ": " << tree->title << std::endl<<std::endl;
		_printTitles(tree->right, spaces+4);
	}
}
void BinaryTree::printActorsOfShow(std::string show){
	std::cout << "starting print" << std::endl;
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
			_printActorsOfShow(tree->left, show);	
			_printActorsOfShow(tree->right, show);
		}
	}
}


void BinaryTree::addNode(std::string title, std::string genre, std::string url, std::string actors[], int numOfActors){
	btree * newleaf = new btree;
	
	newleaf->title = title;
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
		std::cout << "head is: " << head->title << std::endl;
	}else{
		btree * parentptr = head;
		btree * targetptr = parentptr;
		// go through tree till a spot is found
		while(targetptr != NULL){
			// see if the new title is lower in the alphabet that then current parent
			if(newleaf->title < targetptr->title){
				std::cout << "    going left" << std::endl;
				parentptr = targetptr;
				targetptr = targetptr->left;
				std::cout << "        parentptr is now: " << parentptr->title << std::endl;
			}else{
				// if the current title is higher thatn the current parent
				parentptr = targetptr;
				targetptr = targetptr->right;
				std::cout << "    going right" << std::endl;
				std::cout << "        parentptr is now: " << parentptr->title << std::endl;
			}
		}
		//
		//targetptr = newleaf;
		if(newleaf->title < parentptr->title){
			parentptr->left = newleaf;
			std::cout << "left child is" << parentptr->left->title << std::endl;
		}else{
			parentptr->right = newleaf;
			std::cout << "right child is" << parentptr->right->title << std::endl;
		}
	}
/*
	if(head == NULL){
		head = newleaf;
	}else{
	_addNode(head, newleaf);
	}*/
}
void BinaryTree::_addNode(btree * tree, btree *data){
	if(tree == NULL){
		std::cout << "addding node" << std::endl;
		tree = data;
	}else if(data->title < tree->title){
		std::cout << "    going left" << std::endl;
		_addNode(tree->left, data);
	}else{		
		std::cout << "    going right" << std::endl;
		_addNode(tree->right, data);
	}
}
