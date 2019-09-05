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

#include "../src/yytab.h"
#include "../src/tokenTree.h"

void comment();
void yyerror();
void make_token();
int line_num = 1;
char lastid[256];

extern char *yyfilename;
int yychar = -1;
extern int errors;



// use a global variable to create a token object
extern token *gtoken;
extern tTree *gtree;
%}
%option nounput
%%

"/*"					{ comment(); return yychar;}
\/\/[^\n\r]*			{ /* eat the comment */ }

^{WSP}#.*\n				{ yyerror("g0 has no #preprocessor direcives. Are you feeding it C/C++ code?"); }

\n              		{ line_num++;/* eat the newline */ }
[ \t\f]+        		{ /* eat the whitespace */ }

"boolean"       		{ make_token(T_BOOLEAN); return T_BOOLEAN; }
"int"           		{ make_token(T_INTEGER); return T_INTEGER; }
"double"				{ make_token(T_FLOAT64);  return T_FLOAT64; }
"float64"				{ make_token(T_FLOAT64); return T_FLOAT64; }
"string"        		{ make_token(T_STRING); return T_STRING; }

"func"					{ make_token(T_FUNC);return T_FUNC; }
"map"					{ make_token(T_MAP); return T_MAP; }
"struct"				{ make_token(T_STRUCT); return T_STRUCT; }
"else"					{ make_token(T_ELSE); return T_ELSE; }
"package"				{ make_token(T_PACKAGE); return T_PACKAGE; }
"const"					{ make_token(T_CONST); return T_CONST; }
"if"            		{ make_token(T_IF); return T_IF; }
"type"					{ make_token(T_TYPE); return T_TYPE; }
"for"           		{ make_token(T_FOR); return T_FOR; }
"import"				{ make_token(T_IMPORT); return T_IMPORT; }
"return"				{ make_token(T_RETURN); return T_RETURN; }

"default"				{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_DEFAULT; }
"interface"				{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_INTERFACE; }
"select"				{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_SELECT; }
"case"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_CASE; }
"defer"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_DEFER; }
"go"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_GO; }
"chan"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_CHAN; }
"goto"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_GOTO; }
"switch"				{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_SWITCH; }
"fallthrough"			{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_FALLTHROUGH; }
"range"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_RANGE; }
"continue"				{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_CONTINUE; }
"while"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_WHILE; }
"break"					{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); return T_BREAK; }
":"						{ yyerror("Go operator not in VGo"); return T_NOTDEFINED; make_token( T_NOTDEFINED); }


"true"					{ make_token(T_BOOLLITERAL); return T_BOOLLITERAL; }
"false"					{ make_token(T_BOOLLITERAL); return T_BOOLLITERAL; }
"null"					{ make_token(T_NULLLITERAL); return T_NULLLITERAL; }
{LETTER}({LETTER}|{DIGIT})*				{ make_token(T_VARIABLE); return T_VARIABLE; }
"0x"{HEXD}+				{ make_token(T_HEXLITERAL); return T_HEXLITERAL; }
{DIGIT}+{IS}?			{ make_token(T_INTLITERAL); return T_INTLITERAL; }
{SLITERAL}				{ make_token(T_INTLITERAL); return T_INTLITERAL; }
{DLITERAL}              { make_token(T_STRINGLITERAL); return T_STRINGLITERAL; }
{DIGIT}+{EXPONENT}{FS}?					{ make_token(T_FLOATLITERAL); return T_FLOATLITERAL; }
{DIGIT}*"."{DIGIT}+({EXPONENT})?{FS}?	{ make_token(T_FLOATLITERAL); return T_FLOATLITERAL; }
{DIGIT}+"."{DIGIT}*({EXPONENT})?{FS}?	{ make_token(T_FLOATLITERAL); return T_FLOATLITERAL; }


"+"						{ make_token(T_PLUS); return T_PLUS; }
"-"						{ make_token(T_MINUS); return T_MINUS; }
"/"						{ make_token(T_DIVIDE);  return T_DIVIDE; }
"*"						{ make_token(T_MULTIPLY); return T_MULTIPLY; }
"%"						{ make_token(T_MOD); return T_MOD; }
"&&"					{ make_token(T_AND); return T_AND; }
"||"					{ make_token(T_OR); return T_OR; }
">"						{ make_token(T_GTHAN); return T_GTHAN; }
"<"						{ make_token(T_LTHAN); return T_LTHAN; }
">="					{ make_token(T_GTHANEQUAL); return T_GTHANEQUAL; }
"<="					{ make_token(T_LTHANEQUAL); return T_LTHANEQUAL; }
"=="					{ make_token(T_EQUAL); return T_EQUAL; }
"!="					{ make_token(T_NOT_EQUAL); return T_NOT_EQUAL; }
"!"						{ make_token(T_NEGATE); return T_NEGATE; }
"("						{ make_token(T_LPAREN); return T_LPAREN; }
")"						{ make_token(T_RPAREN); return T_RPAREN; }
"["						{ make_token(T_LBRACK); return T_LBRACK; }
"]"						{ make_token(T_RBRACK); return T_RBRACK; }
"{"						{ make_token(T_LCURL); return T_LCURL; }
"}"						{ make_token(T_RCURL); return T_RCURL; }
"="						{ make_token(T_ASSIGNMENT); return T_ASSIGNMENT; }
"+="					{ make_token(T_INCREMENT); return T_INCREMENT; }
"-="					{ make_token(T_DECREMENT); return T_DECREMENT; }
"."						{ make_token(T_DOT); return T_DOT; }
","						{ make_token(T_SEPERATOR); return T_SEPERATOR; }
";"						{ make_token(T_SEMICOLON); return T_SEMICOLON; }


.                       { yyerror("lexical error"); }

%%


// Mostly based on main function in notes by Dr. Jeffery


void make_token(int category){
	// let's make a token, and set it to the global variable.
	gtoken = create_token(category, yytext, 0, line_num, yyfilename, yytext);
}

void yy_buffer_cleanup(){
	// flex doesn't cleanup after itself
	// So this function is the cleanup.
	if(yytext != NULL){
		// if yytext exists, free it
		free(yytext);
		yytext = NULL;
	}
	if(yyfilename != NULL){
		// if yyfilename exists, free it
		free(yyfilename);
		yyfilename = NULL;
	}
	if(gtree != NULL){
		// if gtree exists, free it
		delete_tree(gtree);
		gtree = NULL;
	}
	if(yyin != NULL)
		fclose(yyin);
	if(!YY_CURRENT_BUFFER)
		// if there is no Buffer, we're done
		return;
	// for every buffer, lets clear it.
	int i = (int)yy_buffer_stack_top;
	for(i= (int)yy_buffer_stack_top; i>=0; i--){
		free(yy_buffer_stack[i]);
	}
	// finally clear the stack of buffers
	free(yy_buffer_stack);
}


void yyerror(char *s) {
	// lets say there was an error
	errors++;

	// print the error to the error file
	fprintf(stderr, "%s: ", s);

	if (yyfilename)
		// if we have a file, say what file the error was in.
		fprintf(stderr, "\nfile \"%s\"\n", yyfilename );

	// print the error line and token
	fprintf(stderr, "line %d, token = \"%s\"\n", line_num, yytext);
	// before we leave, let's cleanup a bit
	yy_buffer_cleanup();
	exit(1);
}

void comment() {
	
	int splat_seen = 0;

	while(1) {
		int ch = input();
		switch (ch) {
			default:
				splat_seen = 0;
				break;
			case '\n':
				splat_seen = 0;
				line_num++;
				break;
			case '*':
				splat_seen = 1;
				break;
			case '/':
				if(splat_seen){
					yychar = T_COMMENT;
					return;
				}else
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
