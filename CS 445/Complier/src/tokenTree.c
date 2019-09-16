/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * Classs CS 445
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tokenTree.h"
#include "../_build/yacc.tab.h"
#include "../src/nonDet.h"

extern char * yyfilename;
extern int line_num;
extern char * yytext;
extern char * prod_to_string(int prodrule);
extern char * cat_to_string(int category);

char * clean_sval(char * string){
	int len = 0;
	char * stringPointer = string;
	// create a space for the new string
	char * newString = malloc(strlen(string)+1);

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
	}else if(category == T_BOOLLITERAL){
		if(strcmp(value, "true") !=  0){
			t->ival = 0;
		}else{
			t->ival = 1;
		}
	}
	return t;
}

token * quick_token(int category){
	return create_token(category, yytext, 0, line_num, yyfilename, yytext);
}

tTree * token_as_tree(int category){
	tTree * tree = malloc(sizeof(tTree));
	tree->nbranches = 0;
	tree->prodrule = 0;
	tree->leaf = quick_token(category);
	return tree;
}

tTree * create_tree(int prodrule, int nbranches, ...){
	// all extened parameters should be of type (tTree *)
	// first lets create a token for this element
	tTree * tree = malloc(sizeof(tTree));
	tree->nbranches = nbranches;
	tree->prodrule = prodrule;

	// referenced https://www.thegeekstuff.com/2017/05/c-variadic-functions/
	// for variadic function layout
	va_list branches;
	va_start( branches, nbranches );
	int i;
	for (i=0; i<nbranches; i++){
		tree->branches[i] = va_arg(branches, tTree *);
	}
	
	// optional va_end call. 
	va_end( branches );

	return tree;
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

void print_prodrule(int prodrule, int nbranches, int depth){
	int i;
	for (i=0; i<depth; i++){
		printf("  ");
	}
	printf("%s:%d\n", prod_to_string(prodrule), nbranches);
}

void print_token(token * leaf, int depth){
	// print out basic info from the token
	int i;
	for (i=0; i< depth; i++){
		printf("  ");
	}
	/*printf("%-10d %-20s %-5d\t %-17s ", leaf->category, leaf->text, leaf->lineno, leaf->filename);
	*/	
	if(leaf->category == T_INTLITERAL)
		// if we are an int, print the ival
		printf("%d:%d\n", leaf->category, leaf->ival);
	else if(leaf->category == T_FLOATLITERAL)
		// if we are a float, print the dval
		printf("%d:%f\n", leaf->category, leaf->dval);
	else if (leaf->category == T_STRINGLITERAL)
		// if we are a string, print the sval
		printf("%d:%s\n", leaf->category, leaf->text);
	else
		// for anything else, don't print a value
		printf("%d:%s\n", leaf->category, leaf->text);
	
}

void _print_tree(tTree * tree, int depth){
	if(tree->prodrule == 0){
		// print the leaf of this branch
		print_token(tree->leaf, depth);
	}else{
		print_prodrule(tree->prodrule, tree->nbranches, depth);
	}
	// now for every branch, print their leafs
	int i = 0;
	for( i=0; i < tree->nbranches; i++){
		if(tree->branches[i] != NULL){
			_print_tree(tree->branches[i], depth+1);
		}
	}
}

void print_tree(tTree * tree){

	if (tree == NULL){
		// don't print empty trees
		printf("Tree is emtpy.\n");
	}else{
		if(tree->prodrule == 0){
			// print the leaf of this branch
			print_token(tree->leaf, 0);
		}else{
			print_prodrule(tree->prodrule, tree->nbranches, 0);
		}
		// now for every branch, print their leafs
		int i = 0;
		for( i=0; i < tree->nbranches; i++){
			if(tree->branches[i] != NULL){
				_print_tree(tree->branches[i], 1);
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
		if(tree->prodrule == 0){
			// throughly clean the leaf.
			delete_token(tree->leaf);
			// now remove the leaf
			free(tree->leaf);
		}
		// finally remove the branch.
		free(tree);
		return;
	}
}

void delete_trees(int count, ...){
	
	va_list trees;
	va_start( trees, count );
	int i;
	for (i=0; i<count; i++){
		delete_tree(va_arg(trees, tTree *));
	}
	
	// optional va_end call. 
	va_end( trees );
}
