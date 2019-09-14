// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file
// (a copy is at www2.cs.uidaho.edu/~jeffery/courses/445/go/LICENSE).

/*
 * Go language grammar adapted from Go 1.2.2.
 *
 * The Go semicolon rules are:
 *
 *  1. all statements and declarations are terminated by semicolons.
 *  2. semicolons can be omitted before a closing ) or }.
 *  3. semicolons are inserted by the lexer before a newline
 *      following a specific list of tokens.
 *
 * Rules #1 and #2 are accomplished by writing the lists as
 * semicolon-separated lists with an optional trailing semicolon.
 * Rule #3 is implemented in yylex.
 */

%{
#include <stdio.h>
//#include <libc.h>
#include <stdlib.h>

#include "../src/tokenTree.h"
extern int yylex();
extern void yyerror(const char *s);
%}
/* write out a header file containing the token defines */
%defines

%union{
	struct tTree * node;
}


%token <node> LASOP LCOLAS

/* keywords */
%token <node> /*LBREAK LCASE LCHAN LCONST LCONTINUE */LDDD
%token <node>/*LDEFAULT LDEFER LELSE */LFALL LFOR /*LFUNC*/ LGO LGOTO
%token <node> LIF LIMPORT LINTERFACE LMAP LNAME
%token <node> LPACKAGE LRANGE LRETURN LSELECT LSTRUCT LSWITCH
%token <node> LTYPE LVAR


/* operators */
%token <node> /*LANDAND*/ LANDNOT LBODY LCOMM /*LDEC LEQ LGE LGT*/
%token <node> LIGNORE /*LINC LLE*/ LLSH /*LLT LNE LOROR */LRSH

/*
LANDAND = T_ANDAND		&&
LRETURN = T_RETURN		return
LBREAK	= T_BREAK		break
LCASE	= T_CASE		case
LCHAN	= T_CHAN		
LCONTINUE = T_CONTINUE	continue
'='		= T_ASSIGNMENT	=
LEQ		= T_EQUAL		==
LLT		= T_LTHAN		<
LGT		= T_GTHAN		>
LLE		= T_LTHANEQUAL	<=
LGE		= T_GTHANEQUAL	>=
*/


/**
 **  my tokens 
**/

/* basic types */
%token <node>	T_NULL T_BOOLEAN T_INTEGER T_FLOAT64 T_STRING

/* Literals */
%token <node> T_NULLLITERAL T_BOOLLITERAL T_INTLITERAL T_STRINGLITERAL T_FLOATLITERAL
%token <node> T_VARIABLE T_HEXLITERAL

/* reserved words */
%token <node> T_FUNC T_MAP T_STRUCT T_ELSE T_PACKAGE T_CONST T_IF T_TYPE T_FOR T_IMPORT
%token <node> T_RETURN T_VAR

/* non reserved words */
%token <node> T_BREAK T_DEFAULT T_INTERFACE T_SELECT T_CASE T_DEFER T_GO T_CHAN 
%token <node> T_GOTO T_SWITCH T_FALLTHROUGH T_RANGE T_CONTINUE T_WHILE 
/* non-reserved words that still need to be added to flex.l */
%token <node> T_COLON T_AND T_DOLLAR

/* operators */
%token <node> T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_MOD T_ANDAND T_OR T_GTHAN T_LTHAN 
%token <node> T_GTHANEQUAL T_LTHANEQUAL T_EQUAL T_NOT_EQUAL T_NEGATE T_LPAREN T_RPAREN 
%token <node> T_RBRACK T_LBRACK T_LCURL T_RCURL T_ASSIGNMENT T_INCREMENT T_DECREMENT 
%token <node> T_DOT T_SEPERATOR T_SEMICOLON 

/* tokens that yacc shouldn't see */
%token <node> T_COMMENT T_NLINE
/* For symbols not supported in vgo */
%token <node> T_NOTDEFINED BAD_TOKEN


/**
 ** Types
**/

%type <node> lbrace import_here
%type <node> sym packname
%type <node> oliteral

%type <node> stmt ntype
%type <node> arg_type
/*%type <node> case caseblock*/

%type <node> compound_stmt dotname embed expr complitexpr bare_complitexpr
%type <node> expr_or_type
%type <node> fndcl hidden_fndcl fnliteral
%type <node> for_body for_header for_stmt if_header if_stmt non_dcl_stmt
%type <node> interfacedcl keyval labelname name
%type <node> name_or_type non_expr_type
%type <node> new_name dcl_name oexpr typedclname
%type <node> onew_name
%type <node> osimple_stmt pexpr pexpr_no_paren
%type <node> pseudocall range_stmt /*select_stmt*/
%type <node> simple_stmt
%type <node> /*switch_stmt*/ uexpr
%type <node> xfndcl typedcl start_complit

%type <node> xdcl fnbody fnres loop_body dcl_name_list
%type <node> new_name_list expr_list keyval_list braced_keyval_list expr_or_type_list xdcl_list
%type <node> oexpr_list/* caseblock_list*/ elseif elseif_list else stmt_list oarg_type_list_ocomma arg_type_list
%type <node> interfacedcl_list vardcl vardcl_list structdcl structdcl_list
%type <node> common_dcl constdcl constdcl1 constdcl_list typedcl_list

%type <node> convtype comptype dotdotdot
%type <node> indcl interfacetype structtype ptrtype
%type <node> recvchantype non_recvchantype othertype fnret_type fntype

%type <node> hidden_importsym hidden_pkg_importsym

%type <node> hidden_constant hidden_literal hidden_funarg
%type <node> hidden_interfacedcl hidden_structdcl

%type <node> hidden_funres
%type <node> ohidden_funres
%type <node> hidden_funarg_list ohidden_funarg_list
%type <node> hidden_interfacedcl_list ohidden_interfacedcl_list
%type <node> hidden_structdcl_list ohidden_structdcl_list

%type <node> hidden_type hidden_type_misc hidden_pkgtype
%type <node> hidden_type_func
%type <node> hidden_type_recv_chan hidden_type_non_recv_chan
%type <node> LLITERAL

%left		LCOMM	/* outside the usual hierarchy; here for good error messages */

%left		T_OROR
%left		T_ANDAND /*LANDAND*/
/*%left		LEQ LNE LLE LGE LLT LGT*/
%left		T_EQUAL T_NOT_EQUAL T_LTHANEQUAL T_GTHANEQUAL T_LTHAN T_GTHAN
%left		T_PLUS T_MINUS '|' '^'
%left		T_MULTIPLY T_DIVIDE T_MOD T_AND LLSH LRSH LANDNOT

/*
 * manual override of shift/reduce conflicts.
 * the general form isi that we assign a precedence
 * to the token being shifted and then introduce
 * NotToken with lower precedence or PreferToToken with higher
 * and annotate the reducing rule accordingly.
 */
%left		NotPackage
%left		T_PACKAGE

%left		NotParen
%left		T_LPAREN


%left		T_RPAREN
%left		PreferToRightParen

%define parse.error verbose

%%
file
	: package imports xdcl_list 
	;

package
	: %empty 
	| NotPackage 
		{
			yyerror("package statement must be first");
			exit(1);
		}
	| T_PACKAGE sym T_SEMICOLON
	;

imports
	: %empty
	| imports import T_SEMICOLON
	;

import
	: LIMPORT import_stmt
	| LIMPORT T_LPAREN import_stmt_list osemi T_RPAREN
	| LIMPORT T_LPAREN T_RPAREN
	;

import_stmt
	: import_here import_package import_there
	;

import_stmt_list
	: import_stmt
	| import_stmt_list T_SEMICOLON import_stmt
	;

import_here
	: LLITERAL
	| sym LLITERAL
	| T_DOT LLITERAL
	;

import_package
	: T_PACKAGE LNAME import_safety T_SEMICOLON
	| %empty
	;

import_safety
	: %empty
	|	LNAME 
	;

import_there
	: hidden_import_list '$' '$'
	| %empty
	;

/*
 * declarations
 */

xdcl
	: %empty {
			yyerror("empty top-level declaration");
			//$$ = nil;
		}
	| common_dcl {printf("test\n");}
	| xfndcl {printf("test\n");}
	| non_dcl_stmt
		{
			yyerror("non-declaration statement outside function body");
			//$$ = nil;
		}
	| error
		{
			yyerror("xdcl error");
			//$$ = nil;
		}
	;

common_dcl
	: LVAR vardcl
	| LVAR T_LPAREN vardcl_list osemi T_RPAREN
	| LVAR T_LPAREN T_RPAREN
	| lconst constdcl
	| lconst T_LPAREN constdcl osemi T_RPAREN
	| lconst T_LPAREN constdcl T_SEMICOLON constdcl_list osemi T_RPAREN
	| lconst T_LPAREN T_RPAREN
	| LTYPE typedcl
	| LTYPE T_LPAREN typedcl_list osemi T_RPAREN
	| LTYPE T_LPAREN T_RPAREN
	;

lconst
	: T_CONST
	;

vardcl
	: dcl_name_list ntype
	| dcl_name_list ntype T_ASSIGNMENT expr_list
	| dcl_name_list T_ASSIGNMENT expr_list
	;

constdcl
	: dcl_name_list ntype T_ASSIGNMENT expr_list
	| dcl_name_list T_ASSIGNMENT expr_list
	;

constdcl1
	: constdcl
	| dcl_name_list ntype
	| dcl_name_list
	;

typedclname
	: sym
	{
		// the name becomes visible right here, not at the end
		// of the declaration.
	}
	;

typedcl
	: typedclname ntype
	;

simple_stmt
	: expr
	| expr LASOP expr
	| expr_list T_ASSIGNMENT expr_list
	| expr_list LCOLAS expr_list
	| expr T_INCREMENT
	| expr T_DECREMENT
	;
/*
case
	: T_CASE expr_or_type_list T_COLON
	| T_CASE expr_or_type_list T_ASSIGNMENT expr T_COLON
	| T_CASE expr_or_type_list LCOLAS expr T_COLON
	| T_DEFAULT T_COLON
	;
*/
compound_stmt
	:T_LCURL stmt_list T_RCURL
	;

/*caseblock:
	case
		{
			// If the last token read by the lexer was consumed
			// as part of the case, clear it (parser has cleared yychar).
			// If the last token read by the lexer was the lookahead
			// leave it alone (parser has it cached in yychar).
			// This is so that the stmt_list action doesn't look at
			// the case tokens if the stmt_list is empty.
			yylast = yychar;
		}
	stmt_list
		{
			int last;
	
			// This is the only place in the language where a statement
			// list is not allowed to drop the final semicolon, because
			// it's the only place where a statement list is not followed 
			// by a closing brace.  Handle the error for pedantry.
	
			// Find the final token of the statement list.
			// yylast is lookahead; yyprev is last of stmt_list
			last = yyprev;
	
			if(last > 0 && last != T_SEMICOLON && yychar != T_RCURL)
				yyerror("missing statement after label");
		}

caseblock_list
	: %empty
	| caseblock_list caseblock
	;
*/
loop_body
	: LBODY
		stmt_list T_RCURL
	;

range_stmt
	: expr_list T_ASSIGNMENT LRANGE expr
	| expr_list LCOLAS LRANGE expr
	;

for_header
	: osimple_stmt T_SEMICOLON osimple_stmt T_SEMICOLON osimple_stmt
	| osimple_stmt
	| range_stmt
	;

for_body
	: for_header loop_body
	;

for_stmt
	: T_FOR for_body
	;

if_header
	: osimple_stmt
	| osimple_stmt T_SEMICOLON osimple_stmt
	;

/* IF cond body (ELSE IF cond body)* (ELSE block)? */
if_stmt
	: T_IF
		if_header
		loop_body
		elseif_list else
	;

elseif
	: T_ELSE T_IF 
		if_header loop_body
	;

elseif_list
	: %empty
	| elseif_list elseif
	;

else
	: %empty
	|	T_ELSE compound_stmt
	;
/*
switch_stmt
	: LSWITCH
		if_header
		LBODY caseblock_list T_RCURL
	;

select_stmt
	:LSELECT
		LBODY caseblock_list T_RCURL
	;
*/
/*
 * expressions
 */
expr
	: uexpr
	| expr T_OROR expr
	| expr T_ANDAND expr
	| expr T_EQUAL expr
	| expr T_NOT_EQUAL expr
	| expr T_LTHAN expr
	| expr T_LTHANEQUAL expr
	| expr T_GTHANEQUAL expr
	| expr T_GTHAN expr
	| expr T_PLUS expr
	| expr T_MINUS expr
	| expr '|' expr
	| expr '^' expr
	| expr T_MULTIPLY expr
	| expr T_DIVIDE expr
	| expr T_MOD expr
	| expr T_AND expr
	| expr LANDNOT expr
	| expr LLSH expr
	| expr LRSH expr
	 /* not an expression anymore, but left in so we can give a good error */
	| expr LCOMM expr
	;

uexpr
	: pexpr
	| T_MULTIPLY uexpr
	| T_AND uexpr
	| T_PLUS uexpr
	| T_MINUS uexpr
	| T_NEGATE uexpr
	| '~' uexpr
		{
			yyerror("the bitwise complement operator is ^");
		}
	| '^' uexpr
	| LCOMM uexpr
	;

/*
 * call-like statements that
 * can be preceded by 'defer' and 'go'
 */
pseudocall
	: pexpr T_LPAREN T_RPAREN
	| pexpr T_LPAREN expr_or_type_list ocomma T_RPAREN
	| pexpr T_LPAREN expr_or_type_list LDDD ocomma T_RPAREN
	;

pexpr_no_paren
	: LLITERAL
	| name
	| pexpr T_DOT sym
	| pexpr T_DOT T_LPAREN expr_or_type T_RPAREN
	| pexpr T_DOT T_LPAREN LTYPE T_RPAREN
	| pexpr T_LBRACK expr T_RBRACK
	| pexpr T_LBRACK oexpr T_COLON oexpr T_RBRACK
	| pexpr T_LBRACK oexpr T_COLON oexpr T_COLON oexpr T_RBRACK
	| pseudocall
	| convtype T_LPAREN expr ocomma T_RPAREN
	| comptype lbrace start_complit braced_keyval_list T_RCURL
	| pexpr_no_paren T_LCURL start_complit braced_keyval_list T_RCURL
	| T_LPAREN expr_or_type T_RPAREN T_LCURL start_complit braced_keyval_list T_RCURL
		{
			yyerror("cannot parenthesize type in composite literal");
		}
	| fnliteral
	;

start_complit
	: %empty {
		// composite expression.
		// make node early so we get the right line number.
		}
	;

keyval
	: expr T_COLON complitexpr
	;

bare_complitexpr
	: expr
	| T_LCURL start_complit braced_keyval_list T_RCURL
	;

complitexpr
	: expr
	| T_LCURL start_complit braced_keyval_list T_RCURL
	;

pexpr
	: pexpr_no_paren
	| T_LPAREN expr_or_type T_RPAREN
	;

expr_or_type
	: expr
	| non_expr_type	%prec PreferToRightParen

name_or_type
	: ntype
	;
lbrace
	: LBODY
	| T_LCURL
	;

/*
 * names and types
 *	newname is used before declared
 *	oldname is used after declared
 */
new_name
	: sym
	;

dcl_name
	: sym
	;

onew_name
	: %empty
	| new_name
	;

sym
	: LNAME
	| hidden_importsym
	| '?'
	;

hidden_importsym
	: '@' LLITERAL T_DOT LNAME
	| '@' LLITERAL T_DOT '?'
	;

name
	: sym	%prec NotParen
	;

labelname
	: new_name
	;

/*
 * to avoid parsing conflicts, type is split into
 *	channel types
 *	function types
 *	parenthesized types
 *	any other type
 * the type system makes additional restrictions,
 * but those are not implemented in the grammar.
 */
dotdotdot
	: LDDD
		{
			yyerror("final argument in variadic function missing type");
		}
	| LDDD ntype
	;

ntype
	: recvchantype
	| fntype
	| othertype
	| ptrtype
	| dotname
	| T_LPAREN ntype T_RPAREN
	;

non_expr_type
	: recvchantype
	| fntype
	| othertype
	| T_MULTIPLY non_expr_type
	;

non_recvchantype
	: fntype
	| othertype
	| ptrtype
	| dotname
	| T_LPAREN ntype T_RPAREN
	;

convtype
	: fntype
	| othertype
	;

comptype
	: othertype
	;

fnret_type
	: recvchantype
	| fntype
	| othertype
	| ptrtype
	| dotname
	;

dotname
	: name
	| name T_DOT sym
	;

othertype
	: T_LBRACK oexpr T_RBRACK ntype
	| T_LBRACK LDDD T_RBRACK ntype
	| T_CHAN non_recvchantype
	| T_CHAN LCOMM ntype
	| T_MAP T_LBRACK ntype T_RBRACK ntype
	| structtype
	| interfacetype
	;

ptrtype
	: T_MULTIPLY ntype
	;

recvchantype
	: LCOMM T_CHAN ntype
	;

structtype
	: LSTRUCT lbrace structdcl_list osemi T_RCURL
	| LSTRUCT lbrace T_RCURL
	;

interfacetype
	: LINTERFACE lbrace interfacedcl_list osemi T_RCURL
	| LINTERFACE lbrace T_RCURL
	;

/*
 * function stuff
 * all in one place to show how crappy it all is
 */
xfndcl
	: T_FUNC fndcl fnbody
	;

fndcl
	: sym T_LPAREN oarg_type_list_ocomma T_RPAREN fnres
	| T_LPAREN oarg_type_list_ocomma T_RPAREN sym T_LPAREN oarg_type_list_ocomma T_RPAREN fnres
	;

hidden_fndcl
	: hidden_pkg_importsym T_LPAREN ohidden_funarg_list T_RPAREN ohidden_funres
	| T_LPAREN hidden_funarg_list T_RPAREN sym T_LPAREN ohidden_funarg_list T_RPAREN ohidden_funres
	;

fntype
	: T_FUNC T_LPAREN oarg_type_list_ocomma T_RPAREN fnres
	;

fnbody
	: %empty
	| T_LCURL stmt_list T_RCURL
	;

fnres
	:  %empty %prec NotParen
	| fnret_type
	| T_LPAREN oarg_type_list_ocomma T_RPAREN
	;

fnlitdcl
	: fntype
	;

fnliteral
	: fnlitdcl lbrace stmt_list T_RCURL
	| fnlitdcl error
	;

/*
 * lists of things
 * note that they are left recursive to conserve yacc stack. they need to
 * be reversed to interpret correctly
 */
xdcl_list
	: %empty
	| xdcl_list xdcl T_SEMICOLON
	;

vardcl_list
	: vardcl
	| vardcl_list T_SEMICOLON vardcl
	;

constdcl_list
	: constdcl1
	| constdcl_list T_SEMICOLON constdcl1
	;

typedcl_list
	: typedcl
	| typedcl_list T_SEMICOLON typedcl
	;

structdcl_list
	: structdcl
	| structdcl_list T_SEMICOLON structdcl
	;

interfacedcl_list
	: interfacedcl
	| interfacedcl_list T_SEMICOLON interfacedcl
	;

structdcl
	: new_name_list ntype oliteral
	| embed oliteral
	| T_LPAREN embed T_RPAREN oliteral
	| T_MULTIPLY embed oliteral
	| T_LPAREN T_MULTIPLY embed T_RPAREN oliteral
	| T_MULTIPLY T_LPAREN embed T_RPAREN oliteral
	;

packname
	: LNAME
	| LNAME T_DOT sym
	;

embed
	: packname
	;

interfacedcl
	: new_name indcl
	| packname
	| T_LPAREN packname T_RPAREN
	;

indcl
	: T_LPAREN oarg_type_list_ocomma T_RPAREN fnres
	;

/*
 * function arguments.
 */
arg_type
	: name_or_type
	| sym name_or_type
	| sym dotdotdot
	| dotdotdot
	;

arg_type_list
	: arg_type
	| arg_type_list T_SEPERATOR arg_type
	;

oarg_type_list_ocomma
	: %empty
	| arg_type_list ocomma
	;

/*
 * statement
 */
stmt
	: %empty
	| compound_stmt
	| common_dcl
	| non_dcl_stmt
	| error
	;

non_dcl_stmt
	: simple_stmt
	| for_stmt
	| if_stmt
	| labelname T_COLON	stmt
	| LFALL
	| T_BREAK onew_name
	| T_CONTINUE onew_name
	| LGO pseudocall
	| T_DEFER pseudocall
	| LGOTO new_name
	| T_RETURN oexpr_list
	;
/*
	| switch_stmt
	| select_stmt
*/

stmt_list
	: stmt
	| stmt_list T_SEMICOLON stmt
	;

new_name_list
	: new_name
	| new_name_list T_SEPERATOR new_name
	;

dcl_name_list
	: dcl_name
	| dcl_name_list T_SEPERATOR dcl_name
	;

expr_list
	: expr
	| expr_list T_SEPERATOR expr
	;

expr_or_type_list
	: expr_or_type
	| expr_or_type_list T_SEPERATOR expr_or_type
	;

/*
 * list of combo of keyval and val
 */
keyval_list
	: keyval
	| bare_complitexpr
	| keyval_list T_SEPERATOR keyval
	| keyval_list T_SEPERATOR bare_complitexpr
	;

braced_keyval_list
	: %empty
	| keyval_list ocomma
	;

/*
 * optional things
 */
osemi
	: %empty
	| T_SEMICOLON
	;

ocomma
	: %empty
	| T_SEPERATOR
	;

oexpr
	: %empty
	| expr
	;

oexpr_list
	: %empty
	| expr_list
	;

osimple_stmt
	: %empty
	| simple_stmt
	;

ohidden_funarg_list
	: %empty
	| hidden_funarg_list
	;

ohidden_structdcl_list
	: %empty
	| hidden_structdcl_list
	;

ohidden_interfacedcl_list
	: %empty
	| hidden_interfacedcl_list
	;

oliteral
	: %empty
	| LLITERAL
	;

/*
 * import syntax from package header
 */
hidden_import
	: LIMPORT LNAME LLITERAL T_SEMICOLON
	| LVAR hidden_pkg_importsym hidden_type T_SEMICOLON
	| T_CONST hidden_pkg_importsym T_ASSIGNMENT hidden_constant T_SEMICOLON
	| T_CONST hidden_pkg_importsym hidden_type T_ASSIGNMENT hidden_constant T_SEMICOLON
	| LTYPE hidden_pkgtype hidden_type T_SEMICOLON
	| T_FUNC hidden_fndcl fnbody T_SEMICOLON
	;

hidden_pkg_importsym
	: hidden_importsym
	;

hidden_pkgtype
	: hidden_pkg_importsym
	;

/*
 *  importing types
 */

hidden_type
	: hidden_type_misc
	| hidden_type_recv_chan
	| hidden_type_func
	;

hidden_type_non_recv_chan
	: hidden_type_misc
	| hidden_type_func
	;

hidden_type_misc
	: hidden_importsym
	| LNAME
	| T_LBRACK T_RBRACK hidden_type
	| T_LBRACK LLITERAL T_RBRACK hidden_type
	| T_MAP T_LBRACK hidden_type T_RBRACK hidden_type
	| LSTRUCT T_LCURL ohidden_structdcl_list T_RCURL
	| LINTERFACE T_LCURL ohidden_interfacedcl_list T_RCURL
	| T_MULTIPLY hidden_type
	| T_CHAN hidden_type_non_recv_chan
	| T_CHAN T_LPAREN hidden_type_recv_chan T_RPAREN
	| T_CHAN LCOMM hidden_type
	;

hidden_type_recv_chan
	: LCOMM T_CHAN hidden_type
	;

hidden_type_func
	: T_FUNC T_LPAREN ohidden_funarg_list T_RPAREN ohidden_funres
	;

hidden_funarg
	: sym hidden_type oliteral
	| sym LDDD hidden_type oliteral
	;

hidden_structdcl
	: sym hidden_type oliteral
	;

hidden_interfacedcl 
	: sym T_LPAREN ohidden_funarg_list T_RPAREN ohidden_funres
	| hidden_type
	;

ohidden_funres
	: %empty
	| hidden_funres
	;

hidden_funres
	: T_LPAREN ohidden_funarg_list T_RPAREN
	| hidden_type
	;

/*
 *  importing constants
 */

hidden_literal
	: LLITERAL
	| T_MINUS LLITERAL
	| sym
	;

hidden_constant
	: hidden_literal
	| T_LPAREN hidden_literal T_PLUS hidden_literal T_RPAREN
	;

hidden_import_list
	: %empty
	| hidden_import_list hidden_import
	;

hidden_funarg_list
	: hidden_funarg
	| hidden_funarg_list T_SEPERATOR hidden_funarg
	;

hidden_structdcl_list
	: hidden_structdcl
	| hidden_structdcl_list T_SEMICOLON hidden_structdcl
	;

hidden_interfacedcl_list
	: hidden_interfacedcl
	| hidden_interfacedcl_list T_SEMICOLON hidden_interfacedcl
	;

LLITERAL
	: T_NULLLITERAL 
	| T_BOOLLITERAL 
	| T_INTLITERAL 
	| T_STRINGLITERAL 
	| T_FLOATLITERAL
	;

%%

