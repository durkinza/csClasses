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
	char * newString = malloc(strlen(string)+1);

	// walk up to the "
	while(*stringPointer != '\"'){
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
	newString[len] = '\0';
	return realloc(newString, sizeof(char)*len+1);
}

token * create_token(int category, char * text, int colno, int lineno, char * filename, char * value){
	// create a token 
	token * t = malloc(sizeof(token));
	// fill in the token values
	t->category = category;
	t->text = malloc((strlen(text)+1)*sizeof(char));
	strcpy(t->text, text);
	t->colno = colno;
	t->lineno = lineno;
	t->filename = malloc((strlen(filename)+1)*sizeof(char));
	strcpy(t->filename, filename);	
	// for now we will set sval to empty values
	t->sval = NULL;
	// now, based on the category of the token,
	// we will set the ival, dval or sval accordingly
	if( category == T_INTLITERAL){
		t->ival = atoi(value);
	}else if(category == T_FLOATLITERAL){
		t->dval = atof(value);	
	}else if(category == T_STRINGLITERAL){
		// for sval, convert \n (and the like) to the actual characters
		char * val = clean_sval(value);
		t->sval = malloc(strlen(val)+sizeof(char));
		strncpy(t->sval, val, strlen(val)+sizeof(char));
		free(val);
	}
	return t;
}

tTree * push_to_tree( tTree * parent, token * leaf){
	tTree * t = malloc(sizeof(tTree));
	t->nbranches = 0;
	t->prodrule = 0;
	t->leaf = leaf;
	
	if( parent == NULL){
		parent = t;
	}else{
		parent->branches[parent->nbranches] = t;
		parent->nbranches++;
	}
	return t;
}

void print_token(token * leaf){
	printf("%-10d %-20s %-5d\t %-17s", leaf->category, leaf->text, leaf->lineno, leaf->filename);
	if(leaf->category == T_INTLITERAL)
		printf("%d\n",leaf->ival);
	else if(leaf->category == T_FLOATLITERAL)
		printf("%f\n", leaf->dval);
	else if (leaf->category == T_STRINGLITERAL)
		printf("%s\n",leaf->sval);
	else
		printf("\n");
}

void print_tree(tTree * tree){
	if (tree == NULL){
		printf("Tree is emtpy.\n");
	}else{
		print_token(tree->leaf);
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
		return;
	if(leaf->filename)
		free((char *)leaf->filename);
	if(leaf->text != NULL)
		free((char *)leaf->text);
	if(leaf->sval != NULL)
		free((char *)leaf->sval);
}


void delete_tree(tTree *tree){
	if(tree == NULL){
		free(tree);
		return;
	}else{
		int i = 0;
		for( i=0; i < tree->nbranches; i++){
			delete_tree(tree->branches[i]);
		}
		delete_token(tree->leaf);
		free(tree->leaf);
		free(tree);
		return;
	}
}
