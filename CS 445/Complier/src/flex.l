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

#include "../_build/yacc.tab.h"
#include "../src/tokenTree.h"
#include "../src/nonDet.h"

void comment();
void _yyerror(char *, int);
void yyerror(char *);
tTree * make_token();
extern int line_num;
int can_have_semi = 0;
int swapped_nline = 0;
char lastid[256];

extern char *yyfilename;
int yychar = -1;
extern int errors;

// use a global variable to create a token object
extern token *gtoken;
extern tTree *gtree;
extern tTree * gtail;
%}
%option nounput
%%

"/*"					{ /* eat the comment */ _yyerror("C comments not allowed in VGo", 1); /*comment();*/}
\/\/[^\n\r]*			{ /* eat the comment */  }

^{WSP}#.*\n				{ _yyerror("g0 has no #preprocessor direcives. Are you feeding it C/C++ code?", 1); }


\n              		{ yylval.node = make_token(T_NLINE); line_num++; if(swapped_nline == 1){can_have_semi=0;swapped_nline=0;return T_SEMICOLON; }else{/* eat the newline */} }
[ \t\f]+        		{ /* eat the whitespace */ }

"boolean"       		{ yylval.node = make_token(T_BOOLEAN); return T_BOOLEAN; }
"bool"		       		{ yylval.node = make_token(T_BOOLEAN); return T_BOOLEAN; }
"int"           		{ yylval.node = make_token(T_INTEGER); return T_INTEGER; }
"double"				{ yylval.node = make_token(T_FLOAT64);  return T_FLOAT64; }
"float64"				{ yylval.node = make_token(T_FLOAT64); return T_FLOAT64; }
"string"        		{ yylval.node = make_token(T_STRING); return T_STRING; }

"func"					{ yylval.node = make_token(T_FUNC);return T_FUNC; }
"map"					{ yylval.node = make_token(T_MAP); return T_MAP; }
"struct"				{ yylval.node = make_token(LSTRUCT); return LSTRUCT; }
"else"					{ yylval.node = make_token(T_ELSE); return T_ELSE; }
"package"				{ yylval.node = make_token(T_PACKAGE); return T_PACKAGE; }
"const"					{ yylval.node = make_token(T_CONST); return T_CONST; }
"if"            		{ yylval.node = make_token(T_IF); return T_IF; }
"type"					{ yylval.node = make_token(LTYPE); return LTYPE; }
"for"           		{ yylval.node = make_token(T_FOR); return T_FOR; }
"import"				{ yylval.node = make_token(LIMPORT); return LIMPORT; }
"return"				{ yylval.node = make_token(T_RETURN); return T_RETURN; }
"var"					{ yylval.node = make_token(LVAR); return LVAR; }

"default"				{ _yyerror("Go operator not in VGo", 1); return T_DEFAULT; }
"interface"				{ _yyerror("Go operator not in VGo", 1); return T_INTERFACE; }
"select"				{ _yyerror("Go operator not in VGo", 1); return T_SELECT; }
"case"					{ _yyerror("Go operator not in VGo", 1); return T_CASE; }
"defer"					{ _yyerror("Go operator not in VGo", 1); return T_DEFER; }
"go"					{ _yyerror("Go operator not in VGo", 1); return T_GO; }
"chan"					{ _yyerror("Go operator not in VGo", 1); return T_CHAN; }
"goto"					{ _yyerror("Go operator not in VGo", 1); return T_GOTO; }
"switch"				{ _yyerror("Go operator not in VGo", 1); return T_SWITCH; }
"fallthrough"			{ _yyerror("Go operator not in VGo", 1); return T_FALLTHROUGH; }
"range"					{ _yyerror("Go operator not in VGo", 1); return T_RANGE; }
"continue"				{ _yyerror("Go operator not in VGo", 1); return T_CONTINUE; }
"while"					{ _yyerror("Go operator not in VGo", 1); return T_WHILE; }
"break"					{ _yyerror("Go operator not in VGo", 1); return T_BREAK; }
":"						{ _yyerror("Go operator not in VGo", 1); return T_COLON; }
"float32"				{ _yyerror("Go operator not in VGo", 1); return T_DEFAULT; }
"int32"					{ _yyerror("Go operator not in VGo", 1); return T_DEFAULT; }


"true"					{ yylval.node = make_token(T_BOOLLITERAL); return T_BOOLLITERAL; }
"false"					{ yylval.node = make_token(T_BOOLLITERAL); return T_BOOLLITERAL; }
"null"					{ yylval.node = make_token(T_NULLLITERAL); return T_NULLLITERAL; }
{LETTER}({LETTER}|{DIGIT})*				{ yylval.node = make_token(LNAME); return LNAME; }
"0x"{HEXD}+				{ yylval.node = make_token(T_HEXLITERAL); return T_HEXLITERAL; }
{DIGIT}+{IS}?			{ yylval.node = make_token(T_INTLITERAL); return T_INTLITERAL; }
{SLITERAL}				{ yylval.node = make_token(T_STRINGLITERAL); return T_STRINGLITERAL; }
{DLITERAL}              { yylval.node = make_token(T_STRINGLITERAL); return T_STRINGLITERAL; }
{DIGIT}+{EXPONENT}{FS}?					{ yylval.node = make_token(T_FLOATLITERAL); return T_FLOATLITERAL; }
{DIGIT}*"."{DIGIT}+({EXPONENT})?{FS}?	{ yylval.node = make_token(T_FLOATLITERAL); return T_FLOATLITERAL; }
{DIGIT}+"."{DIGIT}*({EXPONENT})?{FS}?	{ yylval.node = make_token(T_FLOATLITERAL); return T_FLOATLITERAL; }


"+"						{ yylval.node = make_token(T_PLUS); return T_PLUS; }
"-"						{ yylval.node = make_token(T_MINUS); return T_MINUS; }
"/"						{ yylval.node = make_token(T_DIVIDE);  return T_DIVIDE; }
"*"						{ yylval.node = make_token(T_MULTIPLY); return T_MULTIPLY; }
"%"						{ yylval.node = make_token(T_MOD); return T_MOD; }
"^"						{ yylval.node = make_token('^'); return (int)'^';}
"~"						{ yylval.node = make_token('~'); return (int)'~';}
"&&"					{ yylval.node = make_token(T_ANDAND); return T_ANDAND; }
"&"						{ yylval.node = make_token(T_AND); return T_AND; }
"||"					{ yylval.node = make_token(T_OROR); return T_OROR; }
"|"						{ yylval.node = make_token(T_OR); return T_OR; }
">"						{ yylval.node = make_token(T_GTHAN); return T_GTHAN; }
"<"						{ yylval.node = make_token(T_LTHAN); return T_LTHAN; }
">="					{ yylval.node = make_token(T_GTHANEQUAL); return T_GTHANEQUAL; }
"<="					{ yylval.node = make_token(T_LTHANEQUAL); return T_LTHANEQUAL; }
"=="					{ yylval.node = make_token(T_EQUAL); return T_EQUAL; }
"!="					{ yylval.node = make_token(T_NOT_EQUAL); return T_NOT_EQUAL; }
"!"						{ yylval.node = make_token(T_NEGATE); return T_NEGATE; }
"("						{ yylval.node = make_token(T_LPAREN); return T_LPAREN; }
")"						{ yylval.node = make_token(T_RPAREN); return T_RPAREN; }
"["						{ yylval.node = make_token(T_LBRACK); return T_LBRACK; }
"]"						{ yylval.node = make_token(T_RBRACK); return T_RBRACK; }
"{"						{ yylval.node = make_token(T_LCURL); return T_LCURL; }
"}"						{ yylval.node = make_token(T_RCURL); return T_RCURL; }
"="						{ yylval.node = make_token(T_ASSIGNMENT); return T_ASSIGNMENT; }
"++"					{ yylval.node = make_token(T_INCREMENT); return T_INCREMENT; }
"--"					{ yylval.node = make_token(T_DECREMENT); return T_DECREMENT; }
"+="					{ yylval.node = make_token(T_ADDEQ); return T_ADDEQ; }
"-="					{ yylval.node = make_token(T_SUBEQ); return T_SUBEQ; }
"."						{ yylval.node = make_token(T_DOT); return T_DOT; }
","						{ yylval.node = make_token(T_SEPERATOR); return T_SEPERATOR; }
";"						{ yylval.node = make_token(T_SEMICOLON); return T_SEMICOLON; }
"<<"					{ yylval.node = make_token(LLSH); return LLSH; }
">>"					{ yylval.node = make_token(LRSH); return LRSH; }


.                       { _yyerror("lexical error", 1); }

%%


// Mostly based on main function in notes by Dr. Jeffery
tTree * make_token(int category){
	switch(category){
		case LNAME:
		case T_RCURL:
		case T_RBRACK:
		case T_RPAREN:
		case T_INTLITERAL:
		case T_HEXLITERAL:
		case T_NULLLITERAL:
		case T_BOOLLITERAL:
		case T_FLOATLITERAL:
		case T_STRINGLITERAL:
		case T_INCREMENT:
		case T_DECREMENT:
		case T_FLOAT64:
		case T_STRING:
		case T_INTEGER:
		case T_BOOLEAN:
			can_have_semi = 1;
			swapped_nline = 0;
			break;
		case T_NLINE:
			if(can_have_semi == 1){
				// if we need to add a semicolon, let's do so
				category = T_SEMICOLON;
				strcpy(yytext,";\0");
				swapped_nline = 1;
			}else{
				// we don't need to make a token for \n
				return NULL;
			}
			break;
		default:
			can_have_semi = 0;
			swapped_nline = 0;
	}

	// check that the lexime was good
	if(category != 0 && category != T_COMMENT && category != T_NLINE){
		// if the lexime was good, add it to the tree
		gtoken = create_token(category, yytext, 0, line_num, yyfilename, yytext);
		gtail = NULL;
		gtail = push_to_tree(gtoken);

		// if we don't have a head, set it to look at the tail. 
		// this should only run when we first get a tail value.
		return gtail;
	}else{
		return NULL;
	}
}

void yy_buffer_cleanup(int clean_yytext){
	// flex doesn't cleanup after itself
	// So this function is the cleanup.
	if(yytext != NULL && clean_yytext == 1){
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
	if(!YY_CURRENT_BUFFER || !clean_yytext)
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

void yyerror(char *s){
	_yyerror(s, 2);
}
void _yyerror(char *s, int err) {
	// lets say there was an error
	errors++;

	// print the error to the error file
	fprintf(stderr, "\n%s: \n", s);

	if (yyfilename)
		// if we have a file, say what file the error was in.
		fprintf(stderr, "\nfile \"%s\"\n", yyfilename );

	// print the error line and token
	fprintf(stderr, "line %d, token = \"%s\"\n", line_num, yytext);
	// before we leave, let's cleanup a bit
	yy_buffer_cleanup(0);
	exit(err);
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
				// c style comments aren't allowed
				// so we won't count the lines
				//line_num++;
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
				_yyerror("Unterminated comment", 1);
				exit(-(++errors));
			}
		}
	}
}

int yywrap() { return 1; }

