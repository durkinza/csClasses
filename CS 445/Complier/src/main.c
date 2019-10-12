/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 *
 *
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenTree.h"
#include "symtab.h"

// flex global vars
extern FILE *yyin;
extern int yyparse();
extern int yylex();
extern void yy_buffer_cleanup(int);
extern char * yytext;
extern int yydebug;

// symtab.h functions
extern void populate_symboltables ( tTree *, int depth );

// my global vars
int line_num = 1;
char * yyfilename;
token * gtoken = NULL;  
tTree * gtree = NULL;
tTree * gtail;// = gtree;

// paramater variables
int show_parse_tree = 0;
int show_symtab_tree = 0;
int show_symtab_tree_verbose = 0;
int show_parse_tree_verbose = 0;

int errors;
FILE *outfile;

FILE * open_file( char * filen ) {
	FILE * file;
    char *filename = malloc( strlen( filen ) + 4 );
	strcpy( filename, filen );
	if ( ( file = fopen( filename, "r" ) ) == NULL ) {
		if ( ( file = fopen( strcat( filename, ".g0" ), "r" ) ) == NULL ) {
			strcpy( filename, filen );
			if ( ( file = fopen( strcat( filename, ".go" ), "r" ) ) == NULL ) {
				strcpy( filename, filen );
				if ( ( file = fopen( strcat( filename, ".vgo" ), "r" ) ) == NULL ) {				
					printf( "ERROR(vgo): can't open %s\n", filen );
					exit( 1 );
				}
			} 
		}
	}
	strcpy( yyfilename, filename );
	free( filename );
	return file;
}

void printHelp() {
	printf( "\
vgo [<options>] <filename> [<filename>...] \n\
											\n\
    -h              This help screen\n\
    -d --debugging  Debugging. Show all tables/trees\n\
    -s --symtab     Show symbol table\n\
    -p --parse      Show parse tree\n\
    -v --verbose    Show verbose information\n\
	\n" );
}

int main ( int argc, char **argv ){	
	if ( argc == 1 ) {
		printHelp();
		exit(1);
	}else{
		// loop through all of the arguments as files
		int i = 0; 
		for( i=1; i< argc; i++ ) {
			if ( argv[i][0] == '-' ) {
				// if it starts with '-' then it's probably a flag parameter			
				if ( !strncmp( argv[i], "-h", 2 ) ) {
					printHelp();
					exit(0);
				} else if ( !strncmp( argv[i], "-s", 2) || !strncmp( argv[i], "--symtab", 8 ) ) {
					show_symtab_tree = 1;
				} else if ( !strncmp( argv[i], "-p", 2) || !strncmp( argv[i], "--parse", 7 ) ) {
					show_parse_tree = 1;
				} else if ( !strncmp( argv[i], "-d", 2) || !strncmp( argv[i], "--debugging", 11 ) ) {
					show_symtab_tree = 1;
					show_parse_tree = 1;
					show_parse_tree_verbose = 1;
					show_symtab_tree_verbose = 1;
				} else if ( !strncmp( argv[i], "-v", 2) || !strncmp( argv[i], "--verbose", 9 ) ) {
					show_parse_tree_verbose = 1;
					show_symtab_tree_verbose = 1;
				} else {
					fprintf(stderr, "Argument not found `%s`\n", argv[i]);
					printHelp();
					exit(1);
				}
				
			} else {
				// copyt the filename over to a string we can manipulate
				yyfilename=malloc( ( strlen( argv[i] )+4 ) );
				// try to open the file
				// if that fails, open the file+.go
				if ( ( yyin = open_file( argv[i] ) ) != NULL ) {
					// keep track of returned categories
					// only continue to parse the file while we haven't had
					// an error, or we haven't reached the end of the file
					// parse the next lexime
					yyparse();
					line_num = 1;
					// Let the user know what file we parsed
					printf( "File: %s\n", yyfilename );
					// start printing leximes
					if ( gtree == NULL ) {
						// If we have no tokens, say so.
						printf( "No Tokens\n" );
					} else {
						if ( show_parse_tree )					
							print_tree( gtree );
						populate_symboltables ( gtree , 0);
						delete_tree( gtree );
					}
					// make some padding at bottom
					//printf("\n\n");
					// clean up our variables
					gtree = NULL;
					gtail = NULL;
					// close the file
					fclose( yyin );
					yyin = NULL;
				} else {
					
					printf("arg: %s\n", argv[i]);
					printf("ERROR(vgo): can't open %s\n", argv[i]);
					return 1;
				}
				// clean up our filename variable
				free(yyfilename);
				yyfilename=NULL;
			}
		}
		// cleanup after flex
		yy_buffer_cleanup(1);
	}
	return 0;
}
