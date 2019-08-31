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
	char * newString = malloc(strlen(string));

	// walk up to the "
	while(*stringPointer != '\"'){
		if(*stringPointer == '\0'){
			newString[len] = '\0';
			return newString;
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
	newString = realloc(newString, len+1);
	return newString;
}


token * create_token(int category, char * text, int colno, int lineno, char * filename, char * value){

	token * t = malloc(sizeof(struct token));
	t->category = category;
	t->text = malloc(strlen(text)+1);
	strcpy(t->text, text);	
	t->colno = colno;
	t->lineno = lineno;
	t->filename = malloc(sizeof(filename)+1);
	strcpy(t->filename, filename);	
	t->filename = filename;
	t->ival = (int)'\0';
	t->dval = (double)'\0';
	t->sval = malloc(1);
	if( category == T_INTLITERAL){
		t->ival = atoi(value);
	}else if(category == T_FLOATLITERAL){
		t->dval = atof(value);	
	}else if(category == T_STRINGLITERAL){
		char * val = clean_sval(value);
		t->sval = malloc(sizeof(val)+1);
		strcpy(t->sval, val);
	}
	return t;
}

tTree * push_to_tree( tTree * parent, token * leaf){
	tTree * t = malloc(sizeof(struct tTree));
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
	else
		printf("%s\n",leaf->sval);
}

void print_tree(tTree * tree){
	if (tree == NULL){
		printf("Tree is emtpy.\n");
	}else{
		print_token(tree->leaf);
		for( int i = 0; i < tree->nbranches; i++){
			if(tree->branches[i] != NULL){
				print_tree(tree->branches[i]);
			}
		}
	}
}
