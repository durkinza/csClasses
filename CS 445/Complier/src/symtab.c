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
void semanticwarning( char *s, tTree * node );
sym_entry * doparamdeclarator(sym_table * list,  sym_entry * se_orig);
sym_entry * dovariabledeclarator( tTree * node, tTree * category, int, int );
void printvariable( char *name, char*type, int, int, char * , int);
char * getType( tTree * node );
int compareTypes(tTree * node);


sym_table * newSymTable ( int nbuckets ) {
	// create a new symbol table with the given number of buckets
	sym_table * new;
	new = balloc( sizeof( sym_table ) );
	new->table = balloc( nbuckets * sizeof( sym_entry * ) );
	int i = 0; 
	// set the buckets to NULL to remove unset pointers
	for (i = 0; i< nbuckets; i++) {
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


sym_entry * insert_sym ( sym_table * tb, char *s, type * typ, tTree * n ) {
	struct sym_entry * se;

	// check that the element doesn't already exist. 

	se = lookup(tb, s);
	if ( se != NULL ) {
		int size = 30 + strlen( s ) + strlen( n->leaf->filename );
		char * e = balloc( size ); 
		snprintf(e, size, "%s : %d : `%s` : Re-declaration", n->leaf->filename, n->leaf->lineno, s);
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
	sym_entry * se = insert_sym( current, s, t , NULL);
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
			globals->scope->name = "global";
			current = globals;

			// auto insert fmt since we don't have import
			type * new = newUnionType("fmt", newSymTable(1), PACKAGE_TYPE );
			sym_entry * fmt_se = insert_sym( current, "fmt", new, NULL );
			type * fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 
			// add Println function
			type * println_type = newUnionType("Println", newSymTable(1), FUNC_TYPE);
			// Add Println return type to be null
			println_type->u.f.ret = newType( "void", NULL_TYPE );
			// Add a linked list for the parameters
			println_type->u.f.params = newSymTable(1);
			// Set scope for the println function
			println_type->u.f.table = newSymTable(1);
	
			// Create a string parameter
			type * println_param_type = newType( "string", T_STRING );
			// Create an entry for the parameter
			sym_entry * println_param = balloc( sizeof( sym_entry ) );
			// fill entry's next 
			println_param->next = println_type->u.f.table->table[0];
			// set the name of the parameter
			println_param->s = balloc(  strlen( "message" ) + 1 );
			strcpy(println_param->s , "message");
			// Set the parameter's type
			println_param->type = println_param_type;
			// Set entry's scope to be the println's fuctions's scope
			println_param->table = println_type->u.f.table;
			// increment the number of entries in the println function
			println_type->u.f.table->nEntries++;
			// set the parameter to be a parameter
			println_param->type->parameter = 1;
			// setting the parameter name
			///println_param->type->name = "message"; 
			// insert the parameter as a symbol 'message' in the println function scope
			sym_entry * s = insert_sym( println_type->u.f.table, "message", println_param->type , NULL );
			// insert the symbol into the function parameters
			doparamdeclarator(println_type->u.f.params, s);
			// finally insert the function into the fmt package
			insert_sym(fmt_se->type->u.f.table, "Println", println_type, NULL );

			// auto insert math since we don't have import
			new = newUnionType("math", newSymTable(1), PACKAGE_TYPE );
			new = newType("math", PACKAGE_TYPE );
			fmt_se = insert_sym( current, "math", new, NULL );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			// auto insert random since we don't have import
			new = newUnionType("random", newSymTable(1), PACKAGE_TYPE );
			new = newType("random", PACKAGE_TYPE );
			fmt_se = insert_sym( current, "random", new, NULL );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			// auto insert time since we don't have import
			new = newUnionType("time", newSymTable(1), PACKAGE_TYPE );
			new = newType("time", PACKAGE_TYPE );
			fmt_se = insert_sym( current, "time", new, NULL );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			// auto insert built-in functions make and len

			new = newUnionType("make", newSymTable(1), FUNC_TYPE );
			new = newType("make", FUNC_TYPE );
			fmt_se = insert_sym( current, "make", new, NULL );
			fmt_ret = newType("", T_NULLLITERAL);
			fmt_se->type->u.f.ret = fmt_ret; 

			new = newUnionType("len", newSymTable(1), FUNC_TYPE );
			new = newType("len", FUNC_TYPE );
			fmt_se = insert_sym( current, "len", new, NULL );
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
				// finally add a linked list for parameters
				func_se->type->u.f.params = newSymTable(1);
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
				if ( show_symtab_tree ) {
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

			// This will point to the newly created parameter entry
			sym_entry * new_entry; 
			// 
			while( temp && temp->prodrule == ND_ARG_TYPE_LIST ) {
				// check for type changes in the parameters.
				if ( temp->nbranches > 1 && temp->branches[1]->nbranches > 1 ){
					typ = temp->branches[1]->branches[1];
				}
				// create a variable for each parameter
				new_entry = dovariabledeclarator( temp->branches[1]->branches[0], typ, node->prodrule, 1 );
				doparamdeclarator(current->scope->u.f.params,  new_entry);
				printvariable( temp->branches[1]->branches[0]->leaf->text, getType(typ), node->prodrule, 1, 0, print_spaces);
				// move down the list
				temp = temp->branches[0];
			}				// create a variable for each type
			if ( temp->nbranches > 1 ){
				typ = temp->branches[1];
			}
			if ( temp && ( typ->prodrule == ND_OTHERTYPE  || typ->prodrule == ND_MAP ) ) {
				// This is for arrays and map
				new_entry = dovariabledeclarator( temp->branches[0], typ, node->prodrule, 1 );
				doparamdeclarator(current->scope->u.f.params,  new_entry);
				printvariable( temp->branches[0]->leaf->text, getType(typ), node->branches[1]->prodrule, 1, node->branches[1]->branches[0]->leaf->text, print_spaces );
			} else if ( temp ){
				// create a variable for a parameter
				switch(typ->leaf->category){
						case T_NULL:
						case T_BOOLEAN:
						case T_INTEGER:
						case T_FLOAT64:
						case T_STRING:
						case T_NULLLITERAL:
						case T_BOOLLITERAL:
						case T_INTLITERAL:
						case T_STRINGLITERAL:
						case T_FLOATLITERAL:
						case T_VARIABLE:
						case T_HEXLITERAL:
							// This is for regular variables

							new_entry = dovariabledeclarator( temp->branches[0], node->branches[1]->branches[1], node->prodrule, 1 );
							doparamdeclarator(current->scope->u.f.params,  new_entry);
							printvariable( temp->branches[0]->leaf->text, getType(typ), node->prodrule, 1, 0, print_spaces);
							break;
						default:{

							// This is for declaring a variable as a custom type
							// lookup the type
							sym_entry * ste = NULL;
							sym_table * st = current;
							do {
								ste = lookup( st, temp->branches[1]->leaf->text );
								st = st->parent;
							} while ( !ste && st && st->nBuckets > 0);
							if ( !ste ) {
								semanticerror( "undeclared variable", node->branches[1] );
							} else {
								type * new = newUnionType(temp->branches[1]->leaf->text, ste->type->u.s.table, STRUCT_TYPE);
								new->cons = 0;
								new->parameter = 0;
								// insert the new symbol into the current hash table
								new_entry = insert_sym( current, temp->branches[0]->leaf->text, new, temp );
								doparamdeclarator(current->scope->u.f.params,  new_entry);

								printvariable( temp->branches[0]->leaf->text, getType(typ), ste->type->basetype, 0, 0, print_spaces );
							}
					}

					}
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
		case ND_ARG_TYPE:{
				sym_entry * new_entry;
				// for single parameters, create a variable
				new_entry = dovariabledeclarator( node->branches[0], node->branches[1], node->prodrule, 1);
				doparamdeclarator(current->scope->u.f.params,  new_entry);
				printvariable( node->branches[0]->leaf->text, getType(node->branches[1]), node->prodrule, 1, 0, print_spaces);
			break;
		}
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
					switch(node->branches[1]->leaf->category){
						case T_NULL:
						case T_BOOLEAN:
						case T_INTEGER:
						case T_FLOAT64:
						case T_STRING:
						case T_NULLLITERAL:
						case T_BOOLLITERAL:
						case T_INTLITERAL:
						case T_STRINGLITERAL:
						case T_FLOATLITERAL:
						case T_VARIABLE:
						case T_HEXLITERAL:
							// This is for regular variables
							dovariabledeclarator( temp, node->branches[1], node->prodrule, 0 );
							printvariable( temp->leaf->text, getType(node->branches[1]), node->prodrule, 0, 0, print_spaces );
							break;
						default:{

							// This is for declaring a variable as a custom type
							// lookup the type
							sym_entry * ste = NULL;
							sym_table * st = current;
							do {
								ste = lookup( st, node->branches[1]->leaf->text );
								st = st->parent;
							} while ( !ste && st && st->nBuckets > 0);
							if ( !ste ) {
								semanticerror( "undeclared variable", node->branches[1] );
							} else {
						
								type * new = newUnionType(temp->leaf->text, ste->type->u.s.table, STRUCT_TYPE);
								new->cons = 0;
								new->parameter = 0;
								// insert the new symbol into the current hash table
								insert_sym( current, temp->leaf->text, new, temp );

								printvariable( temp->leaf->text, getType(node->branches[1]), ste->type->basetype, 0, 0, print_spaces );
							}
						}
					}
				}
			}
			break;
		}
		case ND_PLUS:
		case ND_MINUS:
		case ND_DIVIDE:
		case ND_MULTIPLY:
		case ND_LTHAN:
		case ND_LTHANEQUAL:
		case ND_GTHAN:
		case ND_GTHANEQUAL:
		case ND_INCREMENT:
		case ND_DECREMENT:
		case ND_OROR:
		case ND_ANDAND:
		case ND_MOD:
		case ND_AND:
		case ND_ASSIGNMENT:
		case ND_EQUAL:
		case ND_NOT_EQUAL:
		case ND_NEGATE:
		case ND_PEXPR_NO_PAREN:
		case ND_PSEUDOCALL:
			compareTypes( node );
			break;
	}
}

sym_entry * doparamdeclarator(sym_table * tb,  sym_entry * se_orig) {
	// check that we have a node to make into a variable
	if ( se_orig == NULL || tb == NULL ){
		return NULL;
	}

	sym_entry *se;
	// if the element doesn't exists, we can add it.
	//int h;
	//h = tb->nEntries;
	se = balloc( sizeof( sym_entry ) );
	se->next = tb->table[0];
	tb->table[0] = se;
	se->table = se_orig->table;
	se->s = se_orig->s;
	se->type = se_orig->type;
	tb->nEntries++;
	return se;
}

sym_entry * dovariabledeclarator( tTree * node, tTree * category, int prodrule, int param ) {
	// check that we have a node to make into a variable
	if ( node == NULL ){
		return NULL;
	}
	type * new;
	
	if ( category->prodrule == ND_MAP ) {
		new = newType(node->leaf->text, MAP_TYPE );
		type * map_index_type = newType(getType(category->branches[0]), category->branches[0]->leaf->category);
		type * map_element_type = newType(getType(category->branches[1]), category->branches[1]->leaf->category);
		new->u.m.index_type = map_index_type;
		new->u.m.element_type = map_element_type;	

		if( show_symtab_tree && show_symtab_tree_verbose ){
			printf("makeing map element\n");
			printf("index type is: %s\n", getType(category->branches[0]));
			printf("element type is: %s\n", getType(category->branches[1]));
		}
	} else if ( category->prodrule == ND_OTHERTYPE ) {
		new = newType(node->leaf->text, category->prodrule );
		type * map_index_type = newType(getType(category->branches[1]), category->branches[1]->leaf->category);
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
	return insert_sym( current, node->leaf->text, new, node );
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

void semanticerror( char *s, tTree * node ) {
	semanticwarning(s, node);
	exit(3);
}
void semanticwarning( char *s, tTree * node ) {
	// return an error when we find one.
	while ( node && ( node->nbranches > 0 ) ) node = node->branches[0];
	if ( node ) {
		fprintf( stderr, "%s : %d : ", node->leaf->filename, node->leaf->lineno );
		fprintf( stderr, "`%s` : ", node->leaf->text);
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
		case ND_MAP:
		case MAP_TYPE:{
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


int compareTypes( tTree * node ) {
	tTree * a = node->branches[0];
	tTree * b = node->branches[1];

	sym_entry * a_entry = NULL;
	sym_entry * b_entry = NULL;
	if( b_entry == NULL ) {};

	int op_type = node->prodrule;
	int a_type, b_type;
	if ( a == NULL || b == NULL){
		return 0;
	}

	// get a's type
	if ( a && a->prodrule == 0 ) {
		// for literals 
		a_type = a->leaf->category;
	} else if ( a && a->nbranches > 1 && (a->prodrule != ND_PEXPR_NO_PAREN ) ) {
		// for return types of child expressions
		a_type = a->ret_type;
	} else {
		// Lets check if the element is already in the symbol table.
		sym_entry * ste = NULL;
		sym_table * st = current;
		if ( a->branches[0]->nbranches > 0 ) {
			// for maps, struct.key, arrays, package.function
			do {
				ste = lookup( st, a->branches[0]->branches[0]->leaf->text );
				st = st->parent;
			} while ( !ste && st && st->nBuckets > 0);

			a_entry = ste;
			a_type = a->ret_type;
		} else {
			// for regular variable name lookups
			do {
				ste = lookup( st, a->branches[0]->leaf->text );
				st = st->parent;
			} while ( !ste && st && st->nBuckets > 0);

			a_entry = ste;
			a_type = ste->type->basetype;
			switch ( a_type) {
				case MAP_TYPE:
					a_type = ste->type->basetype;
					break;
			}
		}
	}

	// get b's type
	if ( b && b->prodrule == 0 ) {
		b_type = b->leaf->category;
	} else if ( b && b->nbranches > 1 ) {
		b_type = b->ret_type;
	} else {
		// Lets check if the element is already in the symbol table.
		sym_entry * ste = NULL;
		sym_table * st = current;
		do {
			ste = lookup( st, b->branches[0]->leaf->text );
			st = st->parent;
		} while ( !ste && st && st->nBuckets > 0);
		b_entry = ste;
		b_type = ste->type->basetype;
	}

	switch ( op_type ) {
		case ND_PLUS:
		case ND_ADDEQ:
			switch ( a_type ) {
				case T_INTLITERAL:
				case T_INTEGER:
				case INT_TYPE:
					if ( b_type == T_INTLITERAL || b_type == INT_TYPE || b_type == T_INTEGER ) {
						node->ret_type = INT_TYPE;
						return 1;
					}
					semanticwarning("Right side of addition is not an integer.", node);
					return 0;
				case T_FLOATLITERAL:
				case T_FLOAT64:
				case FLOAT_TYPE:
					if ( b_type == T_FLOATLITERAL || b_type == FLOAT_TYPE || b_type == T_FLOAT64 ) {
						node->ret_type = FLOAT_TYPE;
						return 1;
					}
					semanticwarning("Right side of addition is not a float64.", node);
					return 0;
				case STRING_TYPE:
				case T_STRINGLITERAL:
				case T_STRING:
					if ( b_type == T_STRINGLITERAL || b_type == STRING_TYPE || b_type == T_STRING) {
						node->ret_type = STRING_TYPE;
						return 1;
					}
					semanticwarning("Right side of concatination is not a string.", node);
					return 0;
			}
			semanticwarning("Left side does not support addition or concatination.", node);
			node->ret_type = NULL_TYPE;
			return 0;
		case ND_DIVIDE:
			switch ( a_type ) {
				case T_INTLITERAL:
				case T_INTEGER:
				case INT_TYPE:
					switch( b_type ) {
						case T_INTLITERAL:
						case T_INTEGER:
						case INT_TYPE:
							node->ret_type = INT_TYPE;
							return 1;
					}
					semanticwarning("Right side of operation is not an integer.", node);
					node->ret_type = NULL_TYPE;
					return 0;
				case T_FLOATLITERAL:
				case T_FLOAT64:
				case FLOAT_TYPE:
					switch ( b_type ) {
						case T_FLOATLITERAL:
						case FLOAT_TYPE:
						case T_FLOAT64:
							node->ret_type = FLOAT_TYPE;
							return 1;
					}
					semanticwarning("Right side of operation is not a float64.", node);
					node->ret_type = NULL_TYPE;
					return 0;
			}
			semanticwarning("Left side of comparison must be an integer or float64", node);
			node->ret_type = NULL_TYPE;
			return 0;
		case ND_MULTIPLY:
		case ND_DECREMENT:
		case ND_MINUS:
		case ND_INCREMENT:
		case ND_SUBEQ:
			switch ( a_type ) {
				case T_INTLITERAL:
				case T_INTEGER:
				case INT_TYPE:
					switch( b_type ) {
						case T_INTLITERAL:
						case T_INTEGER:
						case INT_TYPE:
							node->ret_type = INT_TYPE;
							return 1;
					}
					semanticwarning("Right side of operation is not an integer.", node);
					node->ret_type = NULL_TYPE;
					return 0;
				case T_FLOATLITERAL:
				case T_FLOAT64:
				case FLOAT_TYPE:
					if ( b_type == T_FLOATLITERAL || b_type == FLOAT_TYPE || b_type == T_FLOAT64 ) {
						node->ret_type = FLOAT_TYPE;
						return 1;
					}
					semanticwarning("Right side of operation is not a float64.", node);
					node->ret_type = NULL_TYPE;
					return 0;
			}
			semanticwarning("Left side of comparison must be an integer or float64", node);
			node->ret_type = NULL_TYPE;
			return 0;
		case ND_LTHAN:
		case ND_GTHAN:
		case ND_LTHANEQUAL:
		case ND_GTHANEQUAL:
			switch ( a_type ) {
				case T_INTLITERAL:
				case T_INTEGER:
				case INT_TYPE:
					switch( b_type ) {
						case T_INTLITERAL:
						case T_INTEGER:
						case INT_TYPE:
							node->ret_type = BOOLEAN_TYPE;
							return 1;
					}
					semanticwarning("Right side of comparison is not an integer.", node);
					return 0;
				case T_FLOATLITERAL:
				case T_FLOAT64:
				case FLOAT_TYPE:
					if ( b_type == T_FLOATLITERAL || b_type == FLOAT_TYPE || b_type == T_FLOAT64 ) {
						node->ret_type = BOOLEAN_TYPE;
						return 1;
					}
					semanticwarning("Right side of comparison is not a float64.", node);
					node->ret_type = NULL_TYPE;
					return 0;
			}
			semanticwarning("Left side of comparison must be an integer or float64", node);
			node->ret_type = NULL_TYPE;
			return 0;
		case ND_ANDAND:
		case ND_OROR:
		case ND_XOR:
		case ND_NOT_EQUAL:
		case ND_NEGATE:
			switch ( a_type ) {
				case T_BOOLLITERAL:
				case T_BOOLEAN:
				case BOOLEAN_TYPE:
					switch( b_type ) {
						case T_BOOLLITERAL:
						case T_BOOLEAN:
						case BOOLEAN_TYPE:
							node->ret_type = BOOLEAN_TYPE;
							return 1;
						break;
						default:		
							semanticwarning("Right side of && is not a boolean.", node);
							node->ret_type = NULL_TYPE;
							return 0;
					}
			}
			semanticwarning("Left side of && is not a boolean.", node);
			node->ret_type = NULL_TYPE;
			return 0;
		case ND_PEXPR_NO_PAREN:{
			int a_asdf;
			if ( a_entry->type->basetype == ND_OTHERTYPE || a_entry->type->basetype == ARRAY_TYPE ) {
				node->ret_type = a_entry->type->u.a.element_type->basetype;
				a_asdf = INT_TYPE;
			}else if ( a_entry->type->basetype == MAP_TYPE || a_entry->type->basetype == ND_MAP ) {
				a_asdf = a_entry->type->u.m.index_type->basetype;
				node->ret_type = a_entry->type->u.m.element_type->basetype;
			}else if ( a_entry->type->basetype == PACKAGE_TYPE){
				node->ret_type = a_entry->type->u.f.ret->basetype;
				return 1;
			}else{
				// if we are looking at a struct
				sym_entry * ste = NULL;
				sym_table * st = a_entry->type->u.s.table;
				do {
					ste = lookup( st, b->leaf->text );
					st = st->parent;
				} while ( !ste && st && st->nBuckets > 0);
				if (!ste){
					semanticwarning("Subscript does not exist in struct.", node);
					return 0;
				}
				b_entry = ste;
				b_type = ste->type->basetype;
				node->ret_type = b_type;
				return 1;
			}
			switch ( a_asdf ) {
				case T_INTLITERAL:
				case T_INTEGER:
				case INT_TYPE:
					switch ( b_type ) {
						case T_INTLITERAL:
						case INT_TYPE:
						case T_INTEGER:
							return 1;
					}
					semanticwarning("Subscript is not an integer.", node);
					return 0;
				case T_FLOATLITERAL:
				case T_FLOAT64:
				case FLOAT_TYPE:
					switch ( b_type ) {
						case T_FLOATLITERAL:
						case FLOAT_TYPE:
						case T_FLOAT64:
							return 1;
					}
					semanticwarning("Subscript is not a float64.", node);
					return 0;
				case STRING_TYPE:
				case T_STRINGLITERAL:
				case T_STRING:
					switch ( b_type ) {
						case T_STRINGLITERAL:
						case STRING_TYPE:
						case T_STRING:
							return 1;
					}
					semanticwarning("Subscript is not a string.", node);
					return 0;
				case BOOLEAN_TYPE:
				case T_BOOLLITERAL:
				case T_BOOLEAN:
					switch ( b_type ) {
						case T_BOOLLITERAL:
						case BOOLEAN_TYPE:
						case T_BOOLEAN:
							return 1;
					}
					semanticwarning("Subscript is not a boolean.", node);
					return 0;
			}
			semanticwarning("Subscript is not the correct type.", node);
			return 0;
		}
		case ND_PSEUDOCALL:{
			if (a_entry->type->basetype == FUNC_TYPE){
				a_type = a_entry->type->u.f.ret->basetype;
			}
			if ( a->branches[0]->prodrule == 0 && strncmp(a->branches[0]->leaf->text, "make", 4) == 0) {
				// for `make` we will set the return type to be the parameter type, and return
				int g_ret = 0;
				tTree * given_param = NULL;
				given_param = node->branches[1];
				if ( given_param->prodrule == 0 ) {
					g_ret = given_param->leaf->category;
				} else if ( given_param->nbranches > 1 ) {
					g_ret = given_param->prodrule;
				} else {
					// Lets check if the element is already in the symbol table.
					sym_entry * ste = NULL;
					sym_table * st = current;
					do {
						ste = lookup( st, given_param->branches[0]->leaf->text );
						st = st->parent;
					} while ( !ste && st && st->nBuckets > 0);
					g_ret = ste->type->basetype;
					switch ( g_ret) {
						case FUNC_TYPE:
							g_ret = ste->type->u.f.ret->basetype;
							break;
						case MAP_TYPE:
							g_ret = ste->type->u.m.element_type->basetype;
							break;
						case ARRAY_TYPE:
							g_ret = ste->type->u.a.element_type->basetype;
							break;
						case STRUCT_TYPE:	 
							break;
					}
				}
				node->ret_type = g_ret;
				return 1;
			} else {
				node->ret_type = a_type;
			}
			int x = 0;
			int g_ret = 0;
			tTree * given_param = NULL;
			sym_entry * func_param = NULL;
			//printf("func_param: %s\n", a_entry->s);
			if ( a_entry->type->basetype == PACKAGE_TYPE ){
				//printf("looking at :%p\n", a_entry->type->u.f.table->table);
				a_entry = lookup(a_entry->type->u.f.table, a->branches[1]->leaf->text);
				if ( a_entry == NULL ){
					printf("function `%s` does not exist in package.\n", a->branches[1]->leaf->text);
					return 0;
				}
				func_param = a_entry->type->u.f.params->table[0];
			} else {
				func_param = a_entry->type->u.f.params->table[0];
			}
			if ( node->branches[1]->nbranches > 1) {
				// if we have an argument list
				// ie. foo(2, 3)
				given_param = node->branches[1]->branches[0];
			} else {
				// if we only have one parameter to a function call
				// ie. foo(1)
				given_param = node->branches[1];
			}

			//printf("number of expected params in %s is %d\n", a_entry->s, a_entry->type->u.f.params->nEntries);
			while ( x < a_entry->
					type->
					u.f.params->
					nEntries 
					&& func_param != NULL && given_param != NULL){
				x++;

				if ( given_param->prodrule == 0 ) {
					g_ret = given_param->leaf->category;
				} else if ( given_param->nbranches > 1 ) {
					g_ret = given_param->ret_type;
				} else {
					// Lets check if the element is already in the symbol table.
					sym_entry * ste = NULL;
					sym_table * st = current;
					do {
						ste = lookup( st, given_param->branches[0]->leaf->text );
						st = st->parent;
					} while ( !ste && st && st->nBuckets > 0);
					g_ret = ste->type->basetype;
				}
				//compare that parameter with the current parameter					
				switch ( func_param->type->basetype ) {
					case INT_TYPE:
					case T_INTEGER:
					case T_INTLITERAL:
						switch ( g_ret ) {
							case INT_TYPE:
							case T_INTEGER:
							case T_INTLITERAL:
								break;
							default:
								if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
									semanticwarning("Given parameter is not an int.", node->branches[0]->branches[1]);
								}else{
									semanticwarning("Given parameter is not an int.", node);
								}
								return 0;
						}
						break;
					case FLOAT_TYPE:
					case T_FLOAT64:
					case T_FLOATLITERAL:
						switch ( g_ret ) {
							case FLOAT_TYPE:
							case T_FLOAT64:
							case T_FLOATLITERAL:
								break;
							default:
								if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
									semanticwarning("Given parameter is not a float.", node->branches[0]->branches[1]);
								}else{
									semanticwarning("Given parameter is not a float.", node);
								}
								return 0;

						}
						break;
					case STRING_TYPE:
					case T_STRING:
					case T_STRINGLITERAL:
						switch ( g_ret ) {
							case STRING_TYPE:
							case T_STRING:
							case T_STRINGLITERAL:
								break;
							default:
								if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
									semanticwarning("Given parameter is not a string.", node->branches[0]->branches[1]);
								}else{
									semanticwarning("Given parameter is not a string.", node);
								}
								return 0;

						}
						break;
					case BOOLEAN_TYPE:
					case T_BOOLEAN:
					case T_BOOLLITERAL:
						switch ( g_ret ) {
							case BOOLEAN_TYPE:
							case T_BOOLEAN:
							case T_BOOLLITERAL:
								break;
							default:
								if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
									semanticwarning("Given parameter is not a boolean.", node->branches[0]->branches[1]);
								}else{
									semanticwarning("Given parameter is not a boolean.", node);
								}
								return 0;

						}
						break;
					default:
						if ( g_ret  != func_param->type->basetype ) {					
							if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
								semanticwarning("The expected parameter type is unkown.", node->branches[0]->branches[1]);
							}else{
								semanticwarning("The expected parameter type is unkown.", node);
							}
							return 0;
						}
				}
	
				// get the next expected parameter from the function
				func_param = func_param->next;
				// get the next given parameter
				if ( node->branches[1]->prodrule != ND_PEXPR_NO_PAREN  && node->branches[1]->nbranches > x ) {
					given_param = node->branches[1]->branches[x];
				}else{
					given_param = NULL;
				}
			}
			if ( func_param == NULL && given_param != NULL ) {
				if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
					semanticwarning("To many parameters given in call.", node->branches[0]->branches[1]);
				}else{
					semanticwarning("To many parameters given in call.", node);
				}
			}
			if ( func_param != NULL && given_param == NULL ) {
				if (node->nbranches > 1 && node->branches[0]->nbranches > 1){
					semanticwarning("Missing parameters in call.", node->branches[0]->branches[1]);
				}else{
					semanticwarning("Missing parameters in call.", node);
				}
			}

			return 0;
			break;
		}
		case ND_ASSIGNMENT:
			// check that the left side isn't constant
			if ( a_entry == NULL ) {
				semanticwarning("Left side of assignment is not a variable.", node);
				node->ret_type = NULL_TYPE;
				return 0;
			}
			if ( a_entry->type->cons == 1) {
				semanticwarning("Left side of assignment is a constant.", node);
				node->ret_type = NULL_TYPE;
				return 0;
			}
			switch ( a_type ) {
				case T_INTLITERAL:
				case T_INTEGER:
				case INT_TYPE:
					switch( b_type ) {
						case T_INTLITERAL:
						case T_INTEGER:
						case INT_TYPE:

							node->ret_type = INT_TYPE;
							return 1;
						default:
							semanticwarning("Right side of assignment is not an integer.", node);
							node->ret_type = NULL_TYPE;
							return 0;
					}
				case T_FLOATLITERAL:
				case T_FLOAT64:
				case FLOAT_TYPE:
					if ( b_type == T_FLOATLITERAL || b_type == FLOAT_TYPE || b_type == T_FLOAT64 ) {
						node->ret_type = FLOAT_TYPE;
						return 1;
					}
					semanticwarning("Right side of assignment is not a float64.", node);
					node->ret_type = NULL_TYPE;
					return 0;
				case STRING_TYPE:
				case T_STRINGLITERAL:
				case T_STRING:
					if ( b_type == T_STRINGLITERAL || b_type == STRING_TYPE || b_type == T_STRING) {
						node->ret_type = STRING_TYPE;
						return 1;
					}
					semanticwarning("Right side of assignment is not a string.", node);
					node->ret_type = NULL_TYPE;
					return 0;
				case T_BOOLLITERAL:
				case T_BOOLEAN:
				case BOOLEAN_TYPE:
					switch( b_type ) {
						case T_BOOLLITERAL:
						case T_BOOLEAN:
						case BOOLEAN_TYPE:
							node->ret_type = BOOLEAN_TYPE;
							return 1;
					}
					semanticwarning("Right side of assignment is not a boolean.", node);
					node->ret_type = NULL_TYPE;
					return 0;
				case ND_MAP:
				case MAP_TYPE:
					switch( b_type ) {
						case ND_MAP:
						case MAP_TYPE:
							node->ret_type = MAP_TYPE;
							return 1;
					}
					semanticwarning("Right side of assignment is not a map.", node);
					node->ret_type = NULL_TYPE;
					return 0;
			}
			if ( a_type == b_type ) {
				node->ret_type = a_type;
				return 1;
			}
			semanticwarning("Non matching types on assignment.", node);
			node->ret_type = NULL_TYPE;
			return 0;
		default:
			semanticwarning("Operation Unkown", node);
			node->ret_type = NULL_TYPE;
			return 0;	
	}
	return 0;
}
