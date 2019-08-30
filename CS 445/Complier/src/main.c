/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 *
 *
*/
#include <stdlib.h>
#include <stdio.h>

extern FILE *yyin;
extern int yyparse();
extern int yylex();
extern char * yytext;
int errors;
FILE *outfile;
char *CurFileName;
extern int yydebug;

int main ( int argc, char **argv ){	
	if (argc == 1){
		yyin = stdin;
		yylex();
	}else{
		while (--argc > 0){
			if ((yyin = fopen(*++argv, "r")) == NULL){
				printf("cat: can't open %s\n", *argv);
				return 1;
			}else{
				yylex();
				printf("line: %s", yytext());
				fclose(yyin);
			}
		}
	}
}

