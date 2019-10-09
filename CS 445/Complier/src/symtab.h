#ifdef SYMTAB_H
#define SYMTAB_H

/*
 * The Hash Table of entries
*/


#include "types.h"

/*
 *  Global Variables
*/
extern sym_table * globals;		/* pointer to the global scope */
extern sym_table * current;		/* pointer to current scope */


/*
 * Function Definitions.
*/
sym_table * newSymTable(int nbuckets);				/* Create a new symbol table */
void deleteSymTable(sym_table * table);				/* Destroy the symbol table */
char * installSym( sym_table * table, char * name, type * entry ); /* enter element in the table */
void printsymbols ( sym_table *st, int level );

void populate_symboltables ( tTree * node );
sym_table * lookup( sym_table * table, char * name);/* Retrieve symbol from table */


void pushscope(sym_table * stp);	/* switch to new scope */
void popscope();					/* go back to parent scope */

sym_entry * enter_newscope( char *s, int typ );		/* make a new scope and enter it */ 
sym_entry * insert_sym( sym_table * tb, char *s, type * typ);

#endif
