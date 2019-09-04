/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 *
 *
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenTree.h"

// flex global vars
extern FILE *yyin;
extern int yyparse();
extern int yylex();
extern void yy_buffer_cleanup();
extern char * yytext;
extern int yydebug;

// my global vars
char * yyfilename;
token * gtoken = NULL;  
tTree * gtree = NULL;
tTree * gtail;// = gtree;

int errors;
FILE *outfile;

int main ( int argc, char **argv ){	
	if (argc == 1){
		yyin = stdin;
		yylex();
	}else{
		while (--argc > 0){
			if ((yyin = fopen(*++argv, "r")) == NULL && (yyin = fopen(strcat(*argv,".go"), "r")) == NULL){
				printf("cat: can't open %s\n", *argv);
				return 1;
			}else{
				int error = -1;
				yyfilename = *argv;
				
				while (error != 0 && error != 999 && error != 601){
					error = yylex();
					if(error != 0 && gtoken != NULL && error != 600){
						gtail = push_to_tree(gtail, gtoken);
					}
					if(gtree == NULL && gtail != NULL) gtree = gtail; // Setting the head to the first tail
				}
				printf("file: %s", yyfilename);
				printf("\n\nCategory   Text\t\t        LineNo\t FileName\t   iVal/dVal/sVal\n");
				printf("-----------------------------------------------------------------------\n");
				if(gtail == NULL){
					printf("No Tokens\n");
				} else {
					print_tree(gtree);
				}
				printf("\n\n");
				delete_tree(gtree);
				gtree = NULL;
				gtail = NULL;
				fclose(yyin);
			}
		}
		free(yytext);
		yy_buffer_cleanup();
	}
	return 0;
}
