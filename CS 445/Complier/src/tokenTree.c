/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * Classs CS 445
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include "tokenTree.h"
#include "yytab.h"

token create_token(int category, char * text, int colno, int lineno, char * filename, int ival, double dval, char * sval){

	token * t = malloc(sizeof(struct token));
	t->category = category;
	t->text = text;
	t->colno = colno;
	t->lineno = lineno;
	t->filename = filename;
	t->ival = ival;
	t->dval = dval;
	t->sval = sval;
}


