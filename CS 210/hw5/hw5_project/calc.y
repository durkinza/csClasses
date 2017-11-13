%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sym.h"
sym pi = {.value=PI, .name="pi", .next=NULL};
%}

%union {
    double dval;
    struct sym * symptr;
}

%token <symptr> NAME
%token <dval> NUMBER
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
    : NAME '=' expression { $1->value = $3; }
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

struct sym * sym_add(char * s, double val){
	struct sym * tmp= (sym *) malloc(sizeof(sym));
	tmp->value=val;
	tmp->name=s;
	tmp->next=sym_tbl;
	sym_tbl = tmp;
	return tmp;
}

struct sym * sym_lookup(char * s)
{
	char * p;
	struct sym * sp = sym_tbl;
	while(sp != NULL){
		if(sp->name && strcmp(sp->name, s) == 0)
			return sp;
			
		sp = sp->next;
	}
	//yyerror("symbol doesn't exist");
	return sym_add(s, 0.0);
	return NULL; /* unreachable */
}
/*
struct sym * sym_lookup(char * s)
{
    char * p;
    struct sym * sp;

    for (sp=sym_tbl; sp < &sym_tbl[NSYMS]; sp++)
    {
        if (sp->name && strcmp(sp->name, s) == 0)
            return sp;

        if (sp->name)
            continue;

        sp->name = strdup(s);
        return sp; 
    }
   
    yyerror("Too many symbols");
    exit(-1);
    return NULL; //unreachable
}*/

