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
sym_entry * dovariabledeclarator( tTree * node, tTree * category, int, int );
void printvariable( char *name, char*type, int, int, char * , int);
char * getType( tTree * node );


sym_table * newSymTable ( int nbuckets ) {
	// create a new symbol table with the given number of buckets
	sym_table * new;
	new = balloc( sizeof( sym_table ) );
	new->table = balloc( nbuckets * sizeof( sym_entry * ) );
	int i =0; 
	// set the buckets to NULL to remove unset pointers
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
	// for each element in the table,
	for ( h = 0; h < tb->nBuckets; ++h ) {
		// for each node in the bucket
		for ( pretemp = tb->table[h]; pretemp != NULL; pretemp = temp ) {
			// free it up
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
	// look in the given symbol table for the specific variable.
	if ( !sb ){
		// if we're not given a table, return NULL
		return NULL;
	}
	int h;
	sym_entry * se;
	// get the hash of the variable name
	h = hash(sb, s);
	// look through the hash table for the variable
	for ( se = sb->table[h]; 
			se != NULL;
			se = se->next ) {
		if ( !strcmp( s, se->s ) ) {
			// if we find it, return the pointer to that entry
			return se;
		}
	}
	// if we don't find it, return NULL
	return NULL;
}


sym_entry * insert_sym ( sym_table * tb, char *s, type * typ ) {
	struct sym_entry * se;

	// check that the element doesn't already exist. 

	se = lookup(tb, s);
	if ( se != NULL ) {
		int size = 29 + sizeof( strlen( s ) );
		char * e = balloc( size ); 
		snprintf(e, size, "Re-declaration of variable `%s`", s);
		semanticerror( e, NULL );
		return se;
	}

	// if the element doesn't exists, we can add it.
	int h;
	h = hash(tb, s);
	se = balloc( sizeof( sym_entry ) );
	se->next = tb->table[h];
	se->table = tb;
	tb->table[h] = se;
	se->s = balloc(  strlen( s ) + 1 );
	strcpy(se->s , s);
	se->type = typ;
	tb->nEntries++;
	return se;
}


void pushscope( sym_table * stp ){
	stp->parent = current; current = stp;
}


void popscope(){
	current = current->parent;
}


sym_entry * enter_newscope( char *s, int typ){
	sym_table * new; 
	type * t;

	// based on the type of scope we need,
	// we'll change the number of buckets needed
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
	sym_entry * se = insert_sym( current, s, t );
	pushscope( new );
	return se;
}


void populate_symboltables ( tTree * node , int depth) {
	int i;
	int print_spaces = depth;
	if ( node == NULL ) return;
	/* pre-order part */
	switch ( node->prodrule ) {
		case ND_PACKAGE:
			if( show_symtab_tree ){
				printf("%*s--- symbol table for: package %s ---\n", print_spaces, "", node->branches[0]->leaf->text);		
			}
			// create a global scope 
			globals = newSymTable(100);
			globals->parent = NULL;
			type * pkg_typ= newType("", PACKAGE_TYPE);
			globals->scope = pkg_typ;
			current = globals;

			// auto insert fmt since we don't have import
			type * new = newType("fmt", PACKAGE_TYPE );
			sym_entry * fmt_se = insert_sym( current, "fmt", new );
			type * fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			// auto insert math since we don't have import
			new = newType("math", PACKAGE_TYPE );
			fmt_se = insert_sym( current, "math", new );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			// auto insert random since we don't have import
			new = newType("random", PACKAGE_TYPE );
			fmt_se = insert_sym( current, "random", new );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			// auto insert time since we don't have import
			new = newType("time", PACKAGE_TYPE );
			fmt_se = insert_sym( current, "time", new );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			break;
			
		case ND_XFNDCL:{
				// get a type ready for the return type
				type * func_ret;
				// build a new scope for this function
				sym_entry * func_se = enter_newscope( node->branches[0]->branches[0]->leaf->text, FUNC_TYPE );
				depth+=2;
				// determine the return type
				if ( node->branches[0]->branches[2] && node->branches[0]->branches[2]->prodrule == ND_FNRES ) {
					if( node->branches[0]->branches[2]->branches[0]->prodrule == ND_MAP){
						// for map return types
						func_ret = newType( getType( node->branches[0]->branches[2]->branches[0] ), node->branches[0]->branches[2]->branches[0]->prodrule);
						type * map_index_type = newType(getType(node->branches[0]->branches[2]->branches[0]->branches[0]), node->branches[0]->branches[2]->branches[0]->branches[0]->prodrule);
						type * map_element_type = newType(getType(node->branches[0]->branches[2]->branches[0]->branches[1]), node->branches[0]->branches[2]->branches[0]->branches[1]->prodrule);
						func_ret->u.m.index_type = map_index_type;
						func_ret->u.m.element_type = map_element_type;	
					} else if( node->branches[0]->branches[2]->branches[0]->prodrule == ND_OTHERTYPE ){
						// for array return types
	
						node->branches[0]->branches[2]->branches[0]->prodrule = ARRAY_TYPE;
						func_ret = newType( getType(node->branches[0]->branches[2]->branches[0]), node->branches[0]->branches[2]->branches[0]->prodrule);
						func_ret->u.a.size = node->branches[0]->branches[2]->branches[0]->branches[0]->leaf->ival;
						type * map_element_type = newType(getType(node->branches[0]->branches[2]->branches[0]->branches[1]), node->branches[0]->branches[2]->branches[0]->branches[1]->prodrule);
						func_ret->u.a.element_type = map_element_type;	
					} else {
						// if we're given a return type, use it
						func_ret = newType( getType( node->branches[0]->branches[2]->branches[0] ) , node->branches[0]->branches[2]->branches[0]->leaf->category );
					}
				} else {
					// if we're not given a return type, deafult to void
					func_ret = newType( "void", T_NULLLITERAL );
				}
				// set our return type to the sym_entry 
				func_se->type->u.f.ret = func_ret; 
				// now print if we need to
				if( show_symtab_tree ){
					printf("  %*s--- symbol table for: func %s  returns: %s ---\n", print_spaces, "", node->branches[0]->branches[0]->leaf->text, func_ret->name);
				}
			}
			break;
		case ND_TYPEDCL:
			// for structs, we will deine them with the type keyword
			if ( node->branches[1]->prodrule == ND_STRUCT ){
				enter_newscope( getType( node ), STRUCT_TYPE );
				depth+=2;
				if ( show_symtab_tree ){
					printf( "  %*s--- symbol table for: struct %s ---\n", print_spaces, "", getType( node ) );
				}
			}
			break;
		case ND_ARG_TYPE_LIST:
			// for function paramters
			if ( !( node->branches[1] ) || !( node->branches[1]->branches[1] ) ) {
				semanticerror("argument no type \n", NULL);
				fflush(stdout);
			}
			// This will point to the current parameter we're looking at
			tTree * temp = node;
			// This will point to the variable type
			tTree * typ = node;
			// 
			while( temp && temp->prodrule == ND_ARG_TYPE_LIST ) {
				// check for type changes in the parameters.
				if ( temp->nbranches > 1 && temp->branches[1]->nbranches > 1 ){
					typ = temp->branches[1]->branches[1];
				}
				// create a variable for each parameter
				dovariabledeclarator( temp->branches[1]->branches[0], typ, node->prodrule, 1 );
				printvariable( temp->branches[1]->branches[0]->leaf->text, getType(typ), node->prodrule, 1, 0, print_spaces);
				// move down the list
				temp = temp->branches[0];
			}				// create a variable for each type
			if ( temp->nbranches > 1 ){
				typ = temp->branches[1];
			}
			if ( temp && ( typ->prodrule == ND_OTHERTYPE  || typ->prodrule == ND_MAP ) ) {
				// This is for arrays and map
				dovariabledeclarator( temp->branches[0], typ, node->prodrule, 1 );
				printvariable( temp->branches[0]->leaf->text, getType(typ), node->branches[1]->prodrule, 1, node->branches[1]->branches[0]->leaf->text, print_spaces );
			} else if ( temp ){
				// create a variable for a parameter
				dovariabledeclarator( temp->branches[0], node->branches[1]->branches[1], node->prodrule, 1 );
				printvariable( temp->branches[0]->leaf->text, getType(typ), node->prodrule, 1, 0, print_spaces);
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
				// print out the variable when it is called.
				printf( "calling: %s", getType(node->branches[0]) );
				if ( ste->table != NULL) {
					if ( ste->table->scope ) {
						// if it has a scope, print out the variable was defined in.
						if ( ste->table->scope->basetype == FUNC_TYPE )
							printf( "\tLocal" );
						else if ( ste->table->scope->basetype == STRUCT_TYPE )
							printf( "\tStruct" );
						else if ( ste->table->scope->basetype == MAP_TYPE )
							printf( "\tMap" );
						else if ( ste->table->scope->basetype == ARRAY_TYPE )
							printf( "\tArray" );
						else if ( ste->type->basetype == PACKAGE_TYPE )
							printf( "\tFrom Package" );
						else if ( ste->type->basetype == FUNC_TYPE )
							printf( "\tFunction" );
					} else {
						printf( "\tGlobal" );
					}
					if ( ste->type )
						printf( "\t%s", typename( ste->type ) );
				}
				printf( "\n" );
			}
			break;
			}
	}
	/* visit branches */
	for ( i=0; i < node->nbranches; i++ ) {
		populate_symboltables( node->branches[i], depth );
	}
	/* post-order part*/
	switch ( node->prodrule ) {
		case ND_XFNDCL:
			if( show_symtab_tree )
				printf("  %*s---\n", print_spaces, "");
			// leave from the scope of the function
			popscope();
			break;
		case ND_TYPEDCL:
			if ( node->branches[1]->prodrule == ND_STRUCT ){
				if( show_symtab_tree )
					printf("  %*s---\n", print_spaces, "");
				// leave the scope of the struct
				popscope();
			}
			break;
		case ND_FILE:
			if( show_symtab_tree ){
				printf("%*s---\n", print_spaces, "");
			}
			// for the file we won't pop off the final scope
			break;
		case ND_ARG_TYPE:
				// for single parameters, create a variable
				dovariabledeclarator( node->branches[0], node->branches[1], node->prodrule, 1);
				printvariable( node->branches[0]->leaf->text, getType(node->branches[1]), node->prodrule, 1, 0, print_spaces);
			break;
		case ND_STRUCTDCL:
		case ND_VARDCL:
		case ND_CONSTDCL:
		case ND_VARDCL_ASSIGN:{
			// temp will be our way to go through the vardcl lists
			tTree * temp = node->branches[0];
			// check that the variable has a type
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
				// loop through the vardcl list
				while ( temp && ( temp->prodrule == ND_DCL_NAME_LIST || temp->prodrule == ND_NEW_NAME_LIST)) {
					dovariabledeclarator( temp->branches[1], node->branches[1], node->prodrule, 0 );
					printvariable( temp->branches[1]->leaf->text, getType(node->branches[1]), node->prodrule, 0, 0, print_spaces );
					temp = temp->branches[0];
				}
				// create a variable for each type
				if ( temp && (node->branches[1]->prodrule == ND_OTHERTYPE || node->branches[1]->prodrule == ND_MAP ) ) {
					// This is for arrays
					dovariabledeclarator( temp, node->branches[1], node->prodrule, 0 );
					printvariable( temp->leaf->text, getType(node->branches[1]->branches[1]), node->branches[1]->prodrule, 0, node->branches[1]->branches[0]->leaf->text, print_spaces );
				} else if ( temp ) {
					// This is for regular variables
					dovariabledeclarator( temp, node->branches[1], node->prodrule, 0 );
					printvariable( temp->leaf->text, getType(node->branches[1]), node->prodrule, 0, 0, print_spaces );
				}
			}
			break;
		}
	}
}


sym_entry * dovariabledeclarator( tTree * node, tTree * category, int prodrule, int param ) {
	// check that we have a node to make into a variable
	if ( node == NULL ){
		return NULL;
	}
	type * new;
	
	if ( category->prodrule == ND_MAP ) {
		new = newType(node->leaf->text, category->prodrule );
		type * map_index_type = newType(getType(category->branches[0]), category->branches[0]->prodrule);
		type * map_element_type = newType(getType(category->branches[1]), category->branches[1]->prodrule);
		new->u.m.index_type = map_index_type;
		new->u.m.element_type = map_element_type;	

		if( show_symtab_tree && show_symtab_tree_verbose ){
			printf("makeing map element\n");
			printf("index type is: %s\n", getType(category->branches[0]));
			printf("element type is: %s\n", getType(category->branches[1]));
		}
	} else if ( category->prodrule == ND_OTHERTYPE ) {
		new = newType(node->leaf->text, category->prodrule );
		type * map_index_type = newType(getType(category->branches[1]), category->branches[1]->prodrule);
		new->u.a.size = category->branches[0]->leaf->ival;
		new->u.a.element_type = map_index_type;
		if( show_symtab_tree && show_symtab_tree_verbose ){
			printf("makeing array element\n");
			printf("size is: %d\n", category->branches[0]->leaf->ival);
			printf("type is: %s\n", getType(category->branches[1]));
		}
	} else {
		new = newType(node->leaf->text, category->leaf->category);
	}
	new->cons = prodrule == ND_CONSTDCL;
	new->parameter = param;
	new->name = getType(category); 
	// insert the new symbol into the current hash table
	return insert_sym( current, node->leaf->text, new );
}


void printvariable( char * name, char * type, int prodrule, int param, char * arr_size, int spaces){
	// we won't print if we don't need to
	if ( ! show_symtab_tree )
		return;
	if ( prodrule == ND_MAP || prodrule == ND_OTHERTYPE) {
		// for arrays and maps
		printf("  %*s%s [ %s ] %s ", spaces, "", name, arr_size, type);
	} else {
		// for other types
		printf("  %*s%s %s ", spaces, "", name, type);
	}
	// for const variables lets say so
	if ( prodrule == ND_CONSTDCL )
		printf("const ");
	// for param variables state it.
	if ( param )
		printf("param ");
	printf("\n");
}

/*
void printsymbols( sym_table * st, int level ){
	// 
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
*/


void semanticerror( char *s, tTree * node ) {
	// return an error when we find one.
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
	// given a node, return a type to match
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
		case ND_MAP:{
			static char ret[30];
			sprintf(ret, "map [ %s ] %s ", getType(node->branches[0]), getType(node->branches[1]));
			return ret;
		}
		case ND_OTHERTYPE:{
			static char ret[30];
			sprintf(ret, "[ %s ] %s ", getType(node->branches[0]), getType(node->branches[1]));
			return ret;
		}
		case ARRAY_TYPE:{
			static char ret[30];
			sprintf(ret, "[ %s ] %s ", node->branches[0]->leaf->text, getType(node->branches[1]));
			return ret;
		}
		case ND_TYPEDCL:
			return getType(node->branches[0]);
	}
	return "";
}
