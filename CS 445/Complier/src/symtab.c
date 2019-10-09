/* 
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * 
*/
#define default_bucket_size 70

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"
#include "nonDet.h"
#include "tokenTree.h"
#include "types.h"
#include "../_build/yacc.tab.h"

extern int errors;
extern void * balloc( int );

extern int show_symtab_tree;
extern int show_symtab_tree_verbose;

sym_table * globals;
sym_table * current;


void semanticerror( char *s, tTree * node );
void dovariabledeclarator( tTree * node, int category, int, int );
void printvariable( char *name, char*type, int, int, char * );
char * getType( tTree * node );



sym_table * newSymTable ( int nbuckets ) {
	sym_table * new;
	new = balloc( sizeof( sym_table ) );
	new->table = balloc( nbuckets * sizeof( sym_entry * ) );
	int i =0; 
	for (i=0; i< nbuckets; i++){
		new->table[i] = NULL;
	}
	new->nBuckets = nbuckets;
	new->nEntries = 0;
	
	
	return new;
}

void deleteSymTable( sym_table * tb ) {
	int h;
	sym_entry * temp;
	sym_entry * pretemp;
	for ( h = 0; h < tb->nBuckets; ++h ) {
		for ( pretemp = tb->table[h]; pretemp != NULL; pretemp = temp ) {
			temp = pretemp->next;
			free( pretemp->s );
			free( pretemp );
		}
		free( tb->table );
	}
	// cast to char * so free will free the whole struct
	free( (char *)tb );
}

int hash ( sym_table * tb,  char * s ) {
	// register tells gcc to keep the variable in a register, 
	// because it will be accessed frequently.
	register int h = 0;
	register char c;
	while ( ( c = *s++ ) ) {
		h += c & 0377;
		h *= 37;
	}
	if ( h < 0 ) { h = -h; }
	return h % tb->nBuckets; 
}

sym_entry * lookup ( sym_table * sb, char * s ) {
	if ( !sb ){
		return NULL;
	}
	int h;
	sym_entry * se;
	h = hash(sb, s);
	for ( se = sb->table[h]; 
			se != NULL;
			se = se->next ) {
		if ( !strcmp( s, se->s ) ) {
			return se;
		}
	}
	return NULL;
}

int insert_sym ( sym_table * tb, char *s, type * typ ) {
	struct sym_entry * se;

	// check that the element doesn't already exist. 

	se = lookup(tb, s);
	if ( se != NULL ) {
		int size = 29 + sizeof( strlen( s ) );
		char * e = balloc( size ); 
		snprintf(e, size, "Re-declaration of variable `%s`", s);
		semanticerror( e, NULL );
		return 0;
	}

	// if the element doesn't exists, we can add it.
	int h;
	h = hash(tb, s);
	se = balloc( sizeof( sym_entry ) );
	se->next = tb->table[h];
	se->table = tb;
	tb->table[h] = se;
	se->s = balloc( sizeof( strlen( s ) + 1 ) );
	strcpy(se->s , s);
	se->type = typ;
	tb->nEntries++;
	return 1;
}

void pushscope( sym_table * stp ){
	stp->parent = current; current = stp;
}
void popscope(){
	current = current->parent;
}

void enter_newscope( char *s, int typ){
	sym_table * new; 
	type * t;

	//t = (typ == CLASS_TYPE) ? newclass(s, new) : newmethod(NULL, NULL, new) ;
	switch ( typ ) {
		case ARRAY_TYPE:
			new = newSymTable( 1 );
			t = newUnionType( s, new, typ );
			break;
		case MAP_TYPE:
		case STRUCT_TYPE:
		case FUNC_TYPE:
		default:
			new = newSymTable( default_bucket_size );
			t = newUnionType( s, new, typ );
			break;
	}
	
	new->scope = t;
	insert_sym( current, s, t );
	pushscope( new );
}

void populate_symboltables ( tTree * node ) {
	int i;
	if ( node == NULL ) return;
	/* pre-order part */
	switch ( node->prodrule ) {
		case ND_PACKAGE:
			if( show_symtab_tree ){
				printf("--- symbol table for: package %s ---\n", node->branches[0]->leaf->text);		
			}
			globals = newSymTable(100);
			globals->parent = NULL;
			current = globals;
			// auto insert fmt since we don't have import
			type * new = newType("fmt", T_NULLLITERAL );
			insert_sym( current, "fmt", new );
			break;
		case ND_XFNDCL:
			if( show_symtab_tree ){
				printf("---\n\n");
				printf("--- symbol table for: func %s ---\n", node->branches[0]->branches[0]->leaf->text);
			}
			enter_newscope( node->branches[0]->branches[0]->leaf->text, FUNC_TYPE );
			break;
		case ND_TYPEDCL:
			if ( node->branches[1]->prodrule == ND_STRUCT ){
				if ( show_symtab_tree ){
					printf("---\n\n");
					printf( "--- symbol table for: struct %s ---\n", getType( node ) );
				}
				enter_newscope( getType( node ), STRUCT_TYPE );
			}
			break;
		case ND_ARG_TYPE_LIST:
			if ( !( node->branches[1] ) || !( node->branches[1]->branches[1] ) ) {
				semanticerror("argument no type \n", NULL);
				fflush(stdout);
			}
			tTree * temp = node;
			while( temp && temp->prodrule == ND_ARG_TYPE_LIST ) {
				dovariabledeclarator( temp->branches[1]->branches[0], node->branches[1]->branches[1]->leaf->category, node->prodrule, 1 );
				printvariable( temp->branches[1]->branches[0]->leaf->text, getType(node->branches[1]->branches[1]), node->prodrule, 1, 0);
				temp = temp->branches[0];
			}
			if ( temp ){
				dovariabledeclarator( temp->branches[0], node->branches[1]->branches[1]->leaf->category, node->prodrule, 1 );
				printvariable( temp->branches[0]->leaf->text, getType(node->branches[1]->branches[1]), node->prodrule, 1, 0);
			}
			return;

		case ND_VAR_CALL:{
			// Lets check if the element is already in the symbol table.
			sym_entry * ste = NULL;
			sym_table * st = current;
			do {
				ste = lookup( st, node->branches[0]->leaf->text );
				st = st->parent;
			} while ( !ste && st && st->nBuckets > 0);
			if ( !ste ) {
				semanticerror( "undeclared variable", node );
			}else if( show_symtab_tree && show_symtab_tree_verbose ){
				printf( "calling: %s", getType(node->branches[0]) );
				if ( ste->table ) {
					if ( ste->table->scope ) {
						if ( ste->table->scope->basetype == FUNC_TYPE )
							printf( "\tLocal" );
						else if ( ste->table->scope->basetype == STRUCT_TYPE )
							printf( "\tStruct" );
						else if ( ste->table->scope->basetype == MAP_TYPE )
							printf( "\tMap" );
						else if ( ste->table->scope->basetype == ARRAY_TYPE )
							printf( "\tArray" );
					} else {
						printf( "\tGlobal" );
					}
					if ( ste->type )
						printf( "\t%s", typename( ste->type ) );
					printf( "\n" );
				}
			}
			break;
			}
	}
	/* visit branches */
	for ( i=0; i < node->nbranches; i++ ) {
		populate_symboltables( node->branches[i] );
	}
	/* post-order part*/
	switch ( node->prodrule ) {
		case ND_XFNDCL:
			popscope();
			break;
		case ND_TYPEDCL:
			if ( node->branches[1]->prodrule == ND_STRUCT ){
				popscope();
			}
			break;
		case ND_FILE:
			if( show_symtab_tree ){
				printf("---\n");
			}
			break;
		case ND_ARG_TYPE:
				dovariabledeclarator( node->branches[0], node->branches[1]->leaf->category, node->prodrule, 1);
				printvariable( node->branches[0]->leaf->text, getType(node->branches[1]), node->prodrule, 1, 0);
			break;
		case ND_STRUCTDCL:
		case ND_VARDCL:
		case ND_CONSTDCL:
		case ND_VARDCL_ASSIGN:{
			// TODO: Allow assignment with type.
			// ie. var x int = 0
			// add variable to the symbol table
			tTree * temp = node->branches[0];
			if( !( node->branches[1] ) 
				||
				( 
					node->branches[1]->prodrule == ND_OTHERTYPE 
					&&
					!( node->branches[1]->branches[1] )

				)
			) {
				semanticerror("vardcl/constdcl no type \n", NULL);
				fflush(stdout);
			} else{
				while ( temp && temp->prodrule == ND_DCL_NAME_LIST ) {
					dovariabledeclarator( temp->branches[1], node->branches[1]->leaf->category, node->prodrule, 0 );
					printvariable( temp->branches[1]->leaf->text, getType(node->branches[1]), node->prodrule, 0, 0 );
					temp = temp->branches[0];
				}
				if ( temp && ( node->branches[1]->prodrule == ND_OTHERTYPE  || node->branches[1]->prodrule == ND_MAP ) ) {
					// This is for arrays
					dovariabledeclarator( temp, node->branches[1]->branches[1]->leaf->category, node->prodrule, 0 );
					printvariable( temp->leaf->text, getType(node->branches[1]->branches[1]), node->branches[1]->prodrule, 0, node->branches[1]->branches[0]->leaf->text );

				} else if ( temp ) {
					// This is for regular variables
					dovariabledeclarator( temp, node->branches[1]->leaf->category, node->prodrule, 0 );
					printvariable( temp->leaf->text, getType(node->branches[1]), node->prodrule, 0, 0);
				}
			}
			break;
		}
	}
}
void dovariabledeclarator( tTree * node, int category, int prodrule, int param ) {
	if ( node == NULL ){
		return;
	}
	
	type * new = newType(node->leaf->text, category);
	new->cons = prodrule == ND_CONSTDCL;
	new->parameter = param;
	
	insert_sym( current, node->leaf->text, new );
}

void printvariable( char * name, char * type, int prodrule, int param, char * arr_size){
	if ( ! show_symtab_tree )
		return;
	if ( prodrule == ND_MAP || prodrule == ND_OTHERTYPE) {
		// for arrays
		printf("   %s [%s] %s ", name, arr_size, type);
	} else {
		// for other types
		printf("   %s %s ", name, type);
	}
	if ( prodrule == ND_CONSTDCL )
		printf("const ");
	if ( param )
		printf("param ");
	printf("\n");
}


void printsymbols( sym_table * st, int level ){
	int i, j;
	sym_entry * ste;
	if ( st == NULL ) return;
	for ( i=0; i < st->nBuckets; i++ ) {
		for ( ste = st->table[i]; ste; ste = ste->next ){
			for ( j = 0; j < level; j++ ) {
				printf( "  " );
			}
			printf( "%s\n", ste->s );
			if ( !ste->type ) continue;
			switch ( ste->type->basetype ) {
				case FUNC_TYPE:
					printsymbols( ste->type->u.f.table, level+1 );
					break;
				case STRUCT_TYPE:
					printsymbols( ste->type->u.s.table, level+1 );
					break;
				case MAP_TYPE:
					printsymbols( ste->type->u.m.values, level+1 );
					break;
				case ARRAY_TYPE:
					printsymbols( ste->type->u.a.values, level+1 );
					break;
			}	
		}
	}
}

void semanticerror( char *s, tTree * node ) {
	while ( node && ( node->nbranches > 0 ) ) node = node->branches[0];
	if ( node ) {
		fprintf( stderr, "%s:%d: ", node->leaf->filename, node->leaf->lineno );
		fprintf( stderr, "`%s` :", node->leaf->text);
	}
	fprintf(stderr, "%s", s);
	if ( node && node->prodrule == LNAME ) fprintf(stderr, " %s", node->leaf->text );
	fprintf(stderr, "\n");
	exit(3);
}

char * getType( tTree * node ) {
	switch( node->prodrule ) {
		case 0:
			switch( node->leaf->category ) {
				case T_FLOATLITERAL:
				case T_FLOAT64:
					return "float64";
				case T_INTLITERAL:
				case T_INTEGER:
					return "int";
				case T_STRING:
				case T_STRINGLITERAL:
					return "string";
				case T_BOOLLITERAL:
				case T_BOOLEAN:
					return "boolean";
				case T_NULLLITERAL:
					return "null";
			}
			return node->leaf->text;
		case ND_STRUCT:
			return "struct";
		case ND_TYPEDCL:
			return node->branches[0]->leaf->text;
	}
	return "";
}
