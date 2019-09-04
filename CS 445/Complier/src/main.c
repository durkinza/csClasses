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

FILE * open_file(char * filen){
	FILE * file;
    char *filename = malloc(strlen(filen)+4);
	strcpy(filename, filen);
	if ( (file = fopen(filename, "r")) == NULL ){
		if ( (file = fopen(strcat(filename, ".g0"), "r")) == NULL ){
			strcpy(filename, filen);
			if ( (file = fopen(strcat(filename, ".go"), "r")) == NULL ){				
				strcpy(filename, filen);
				if ( (file = fopen(strcat(filename, ".vgo"), "r")) == NULL ){				
					printf("ERROR(vgo): can't open %s\n", filen);
					exit(1);
				}
			} 
		}
	}
	strcpy(yyfilename, filename);
	free(filename);
	return file;
}

int main ( int argc, char **argv ){	
	if (argc == 1){
		yyin = stdin;
		yylex();
	}else{
		// loop through all of the arguments as files
		int i = 0; 
		for(i=1;i< argc; i++){
			// copyt the filename over to a string we can manipulate
			yyfilename=malloc((strlen(argv[i])+4));
			//strcpy(yyfilename, argv[i]);
			// try to open the file
			// if that fails, open the file+.go
			if ( (yyin = open_file(argv[i])) != NULL){
				// keep track of returned categories
				int cate = -1;
				// only continue to parse the file while we haven't had
				// an error, or we haven't reached the end of the file
				while (cate != 0 && cate != 999 && cate != 601){
					// parse the next lexime
					cate = yylex();
					// check that the lexime was good
					if(cate != 0 && gtoken != NULL && cate != 600){
						// if the lexime was good, add it to the tree
						gtail = push_to_tree(gtail, gtoken);
					}
					// if we don't have a head, set it to look at the tail. 
					// this should only run when we first get a tail value.
					if(gtree == NULL && gtail != NULL) gtree = gtail; // Setting the head to the first tail
				}
				// Let the user know what file we parsed
				printf("File: %s", yyfilename);
				// print the header
				printf("\n\nCategory   Text\t\t        LineNo\t FileName\t   iVal/dVal/sVal\n");
				printf("-----------------------------------------------------------------------\n");
				// start printing leximes
				if(gtail == NULL){
					// If we have no tokens, say so.
					printf("No Tokens\n");
				} else {
					print_tree(gtree);
				}
				// make some padding at bottom
				printf("\n\n");
				// clean up our variables
				delete_tree(gtree);
				gtree = NULL;
				gtail = NULL;
				// close the file
				fclose(yyin);
				yyin = NULL;
			}else{
				
				printf("arg: %s\n", argv[i]);
				printf("ERROR(vgo): can't open %s\n", argv[i]);
				return 1;
			}
			// clean up our filename variable
			free(yyfilename);
			yyfilename=NULL;
		}
		// cleanup after flex
		yy_buffer_cleanup();
	}
	return 0;
}
