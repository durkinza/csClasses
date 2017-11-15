%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sym.h"
%}

%union {
    double dval;
    struct sym * symptr;
}

%token <symptr> NAME
%token <dval> NUMBER
%token <void> PRINT
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type <dval> expression

%%
statement_list
    : statement '\n'
    | statement_list statement '\n'
    ;

statement
    : PRINT {printSymTbl();}
	| NAME '=' expression { if($1->edit)$1->value = $3;else{yyerror("assign to const");}}
    | expression { printf("= %g\n", $1); }
    ;

expression
    : expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { if($3)$$ = $1 / $3;else{yyerror("divide by zero");} }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')' { $$ = $2; }
    | NUMBER
    | NAME { $$ = $1->value;}
    ;

%%
main(){
	sym_add("PI", PI, 0);
	sym_add("PHI", PHI, 0);
	return yyparse();
}
struct sym * sym_add(char * s, double val, int edit){
	struct sym * tmp= malloc(sizeof(sym));				// clear out an empty symbol
	struct sym * sp = sym_tbl;							// symbol previous
	struct sym * sc = sym_tbl;							// symbol current
	tmp->value=val;
	tmp->name=strdup(s);
	tmp->edit=edit;
	if(sym_tbl == NULL){								// if no symbols exist, start with the current one
		tmp->next = NULL;								
		sym_tbl = tmp;
		return tmp;
	}
	if(asciiOrder(tmp->name, sym_tbl->name)){			// if new symbol should go first,
		tmp->next = sc;									// set new->next to the head
		sym_tbl = tmp;									// set head to new
		return tmp;
	}
	while(sc != NULL									// loop through the symbols until the correct location is found, or the end is reached
		&& asciiOrder(sc->name, tmp->name)
	){
		sp = sc;										// set previous to current
		sc = sc->next;									// set current to the next symbol
	}
	sp->next = tmp;										// set previous symbol's next to new
	tmp->next = sc;										// set new->next to continue through
	return tmp;
}
void printSymTbl(){
	struct sym * sp1 = sym_tbl;
	struct sym * sp2 = sym_tbl;
	int counter = 0;
	while(sp1 != NULL){										// loop through to count how many symbols there are
		counter++;
		sp1=sp1->next;
	}	
	printf("num-syms: %d\n", counter);						// print out how many are in the list
	while(sp2 != NULL){										// until at the end of the symbols
		printf("\t%s => %f\n", sp2->name, sp2->value);		//     print out the symbol and it's value
		sp2=sp2->next;										//     go to next symbol
	}
}

struct sym * sym_lookup(char * s)
{
	char * p;
	struct sym * sp = sym_tbl;
	while(sp != NULL){
		if(sp->name && strcmp(sp->name, s) == 0){
			return sp;
		}
			
		sp = sp->next;
	}
	return sym_add(s, 0.0, 1);
}
int asciiOrder(char *s1, char *s2){
	int i=0;
	int s1L = strlen(s1);
	int s2L = strlen(s2);
	while(i < s1L && i < s2L){
		if((int)s1[i] < (int)s2[i]){ // if s1 is first, say so
			return 1; // return s1 is first
		}
		if((int)s1[i] > (int)s2[i]){
			return 0; // return s2 is first
		}
		i++;
	}
	if(s1L > s2L){
		return 0; // if s1 is longer it is last
	}
	return 1; // if they are equal or s2 is longer, s1 is first
}
