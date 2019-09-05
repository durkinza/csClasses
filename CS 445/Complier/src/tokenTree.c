/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * Classs CS 445
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenTree.h"
#include "yytab.h"

char * clean_sval(char * string){
	int len = 0;
	char * stringPointer = string;
	// create a space for the new string
	char * newString = malloc(strlen(string)+1);

	// walk up to the "
	while(*stringPointer != '\"'){
		// if we hit the end of the string, return
		if(*stringPointer == '\0'){
			newString[len] = '\0';
			return realloc(newString, 1);
		}
		stringPointer++;
	}
	
	// add " to new string
	stringPointer++;

	// continue until ending quotes
	while(*stringPointer != '\"'){
		// if we see \ look for the next character
		// if it's one we know, replace the set with the ascii value of that escaped character
		if(*stringPointer == '\\'){
			switch(stringPointer[1]){
				case 'n':
					newString[len++] = '\n'; 
					break;
				case 't':
					newString[len++] = '\t'; 
					break;
				case '\\':
					newString[len++] = '\\'; 
					break;
				case '\"':
				case '\'':
					newString[len++] = '\'';
					break;
				default:
					newString[len++] = '\\';
					newString[len++] = stringPointer[1];
			}
			stringPointer += 2;
		}else{
			newString[len++] = *stringPointer;
			stringPointer++;
		}
	}
	// add NULL to end of string
	newString[len] = '\0';
	// remove extra allocated space and return
	return realloc(newString, len+1);
}

token * create_token(int category, char * text, int colno, int lineno, char * filename, char * value){
	// create a token 
	token * t = malloc(sizeof(token));
	// fill in the token values
	t->category = category;
	t->text = malloc((strlen(text)+1));
	strcpy(t->text, text);
	t->colno = colno;
	t->lineno = lineno;
	t->filename = malloc((strlen(filename)+1));
	strcpy(t->filename, filename);	
	// for now we will set sval to empty values
	t->sval = NULL;
	// now, based on the category of the token,
	// if it's a hexliteral, implicitly convert to an integer
	if( category == T_HEXLITERAL){
		t->ival = (int)strtol(value, NULL, 0); 		
		t->category = T_INTLITERAL;
	}else
	// we will set the ival, dval or sval accordingly
	if( category == T_INTLITERAL){
		// convert string to int.
		t->ival = atoi(value);
	}else if(category == T_FLOATLITERAL){
		// convert string to double.
		t->dval = atof(value);	
	}else if(category == T_STRINGLITERAL){
		// for sval, convert \n (and the like) to the actual characters
		char * val = clean_sval(value);
		// create space for the sval
		t->sval = malloc(strlen(val)+1);
		// copy the string over
		strncpy(t->sval, val, strlen(val)+1);
		// clear up the malloc'd space for val
		free(val);
	}
	return t;
}

tTree * push_to_tree( tTree * parent, token * leaf){
	// create a new tree element
	tTree * t = malloc(sizeof(tTree));
	t->nbranches = 0;
	t->prodrule = 0;
	// set the token to be the leaf of the branch
	t->leaf = leaf;
	
	if( parent == NULL){
		// if we are the element of the tree, become the tree
		parent = t;
	}else{
		// if we can become a branch, do so.
		parent->branches[parent->nbranches] = t;
		parent->nbranches++;
	}
	return t;
}

void print_token(token * leaf){
	// print out basic info from the token
	printf("%-10d %-20s %-5d\t %-17s ", leaf->category, leaf->text, leaf->lineno, leaf->filename);
	
	if(leaf->category == T_INTLITERAL)
		// if we are an int, print the ival
		printf("%d\n",leaf->ival);
	else if(leaf->category == T_FLOATLITERAL)
		// if we are a float, print the dval
		printf("%f\n", leaf->dval);
	else if (leaf->category == T_STRINGLITERAL)
		// if we are a string, print the sval
		printf("%s\n",leaf->sval);
	else
		// for anything else, don't print a value
		printf("\n");
}

void print_tree(tTree * tree){
	if (tree == NULL){
		// don't print empty trees
		printf("Tree is emtpy.\n");
	}else{
		// print the leaf of this branch
		print_token(tree->leaf);
		// now for every branch, print their leafs
		int i = 0;
		for( i=0; i < tree->nbranches; i++){
			if(tree->branches[i] != NULL){
				print_tree(tree->branches[i]);
			}
		}
	}
}


void delete_token(token * leaf){
	if(leaf == NULL)
		// we can't delete that that does not exist
		return;

	// we only need to worry about cleaning the values
	// that pointers are set to. since we can't remove
    // them after we've deleted the leaf. 
	if(leaf->filename)
		// remove filename if it exists
		free((char *)leaf->filename);
	if(leaf->text != NULL)
		// remove text if it exists
		free((char *)leaf->text);
	if(leaf->sval != NULL)
		// remove sval if it exists
		free((char *)leaf->sval);
}


void delete_tree(tTree *tree){
	if(tree == NULL){
		// delete the empty tree.
		free(tree);
		return;
	}else{
		// for all branches, delete recursively 
		int i = 0;
		for( i=0; i < tree->nbranches; i++){
			delete_tree(tree->branches[i]);
		}
		// throughly clean the leaf.
		delete_token(tree->leaf);
		// now remove the leaf
		free(tree->leaf);
		// finally remove the branch.
		free(tree);
		return;
	}
}
