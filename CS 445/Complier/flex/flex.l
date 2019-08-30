OCTET		[0-7]
DIGIT		[0-9]
HEXD		[A-Fa-f0-9]
LETTER		[A-Za-z_]
EXPONENT	[Ee][+-]?{DIGIT}+
WSP			[ \t\f]*
DLITERAL	\"(\\.|[^\\"])*\"
SLITERAL	'(\\.|[^\\'])+' 
FS  (f|F|l|L)
IS	(u|U|l|L)

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "src/yytab.h"
#include "src/tokenTree.h"
//#include "../src/main"

void comment();
void yyerror();
void make_token();
int line_num = 1;
char lastid[256];

// use a global variable to create a token object
tTree *gtoken;
%}
%option nounput
%%

"/*"					{ comment(); }
\/\/[^\n\r]*			{ /* eat the comment */ }

^{WSP}#.*\n				{ yyerror("g0 has no #preprocessor direcives. Are you feeding it C/C++ code?"); }

\n              		{ /* eat the newline */ }
[ \t\f]+        		{ /* eat the whitespace */ }

"null"          		{ make_token(T_NULL); return T_NULL; }
"boolean"       		{ make_token(T_BOOLEAN); return T_BOOLEAN; }
"int"           		{ make_token(T_INTEGER); return T_INTEGER; }
"double"				{ return T_FLOAT64; }
"float64"				{ return T_FLOAT64; }
"string"        		{ return T_STRING; }

"func"					{ make_token(T_FUNC);return T_FUNC; }
"map"					{ return T_MAP; }
"struct"				{ return T_STRUCT; }
"else"					{ return T_ELSE; }
"package"				{ return T_PACKAGE; }
"const"					{ return T_CONST; }
"if"            		{ return T_IF; }
"type"					{ return T_TYPE; }
"for"           		{ return T_FOR; }
"import"				{ return T_IMPORT; }
"return"				{ return T_RETURN; }

"break"					{ return T_NOTDEFINED; return T_BREAK; } 
"default"				{ return T_NOTDEFINED; return T_DEFAULT; }
"interface"				{ return T_NOTDEFINED; return T_INTERFACE; }
"select"				{ return T_NOTDEFINED; return T_SELECT; }
"case"					{ return T_NOTDEFINED; return T_CASE; }
"defer"					{ return T_NOTDEFINED; return T_DEFER; }
"go"					{ return T_NOTDEFINED; return T_GO; }
"chan"					{ return T_NOTDEFINED; return T_CHAN; }
"goto"					{ return T_NOTDEFINED; return T_GOTO; }
"switch"				{ return T_NOTDEFINED; return T_SWITCH; }
"fallthrough"			{ return T_NOTDEFINED; return T_FALLTHROUGH; }
"range"					{ return T_NOTDEFINED; return T_RANGE; }
"continue"				{ return T_NOTDEFINED; return T_CONTINUE; }
"while"					{ return T_NOTDEFINED; return T_WHILE; }
"break"					{ return T_NOTDEFINED; return T_BREAK; }


"true"					{ return T_BOOLLITERAL; }
"false"					{ return T_BOOLLITERAL; }
"null"					{ return T_NULLLITERAL; }
{LETTER}({LETTER}|{DIGIT})*				{ return T_VARIABLE; }
{DIGIT}+{IS}?			{ return T_INTLITERAL; }
{SLITERAL}				{ return T_INTLITERAL; return T_CHARLITERAL; }
{DLITERAL}              { return T_STRINGLITERAL; }
{DIGIT}+{EXPONENT}{FS}?					{ return T_FLOATLITERAL; }
{DIGIT}*"."{DIGIT}+({EXPONENT})?{FS}?	{ return T_FLOATLITERAL; }
{DIGIT}+"."{DIGIT}*({EXPONENT})?{FS}?	{ return T_FLOATLITERAL; }



"+"						{ return T_PLUS; }
"-"						{ return T_MINUS; }
"/"						{ return T_DIVIDE; }
"*"						{ return T_MULTIPLY; }
"%"						{ return T_MOD; }
"&&"					{ return T_AND; }
"||"					{ return T_OR; }
">"						{ return T_GTHAN; }
"<"						{ return T_LTHAN; }
">="					{ return T_GTHANEQUAL; }
"<="					{ return T_LTHANEQUAL; }
"=="					{ return T_EQUAL; }
"!="					{ return T_NOT_EQUAL; }
"!"						{ return T_NEGATE; }
"("						{ return T_LPAREN; }
")"						{ return T_RPAREN; }
"["						{ return T_LBRACK; }
"]"						{ return T_RBRACK; }
"="						{ return T_ASSIGNMENT; }
"+="					{ return T_INCREMENT; }
"-="					{ return T_DECREMENT; }


.                       { yyerror("lexical error"); }

%%


// Mostly based on main function in notes by Dr. Jeffery


char *yyfilename = NULL;
int yychar;
int errors;


void make_token(int category){
	printf("found: %s", yytext);
	//create_token(category, yytext(), yyloc.firstcol, yyloc.firstline, yyfilename, yyval, yy);
}



void yyerror(char *s) {
   errors++;

   fprintf(stderr, "%s: ", s);

   if (yyfilename)
      fprintf(stderr, "file \"%s\", ", yyfilename );

   fprintf(stderr, "line %d, token = \"%s\"\n", yylineno, yytext);
   exit(1);
}

void comment() {
	
	int splat_seen = 0;

	for(;;) {
		int ch = input();
		switch (ch) {
			default:
				splat_seen = 0;
				break;
			case '*':
				splat_seen = 1;
				break;
			case '/':
				if(splat_seen)
					return;
				else
					splat_seen = 0;
					break;
			case 0: 
			{
				yychar = BAD_TOKEN;
				yyerror("Unterminated comment");
				exit(-(++errors));
			}
		}
	}
}

int yywrap() { return 1; }
