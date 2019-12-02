#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/tac.h"
#include "../src/types.h"
instr * gen( int op, struct addr * a1, struct addr * a2, struct addr * a3 ) {
	instr * rv = malloc( sizeof( struct instr ) );
	if ( rv == NULL ) {
		fprintf(stderr, "out of memory\n");
		exit(4);
	}
	rv->opcode = op;
	rv->dest = a1;
	rv->src1 = a2;
	rv->src2 = a3;
	rv->next = NULL;
	return rv;
}

instr * copylist( struct instr * l ) {
	if (l == NULL) 
		return NULL;
	instr * lcopy = gen( l->opcode, l->dest, l->src1, l->src2 );
	lcopy->next = copylist( l->next );
	return lcopy;
}

instr * append( struct instr * l1, struct instr * l2 ) {
	if (l1 == NULL) 
		return l2;
	instr * ltmp = l1;
	while( ltmp->next != NULL ) {
		ltmp = ltmp->next;
	}
	ltmp->next = l2;
	return l1;
}

instr * concat( struct instr * l1, struct instr * l2 ) {
   return append( copylist( l1 ), l2 );
}


void insert_address(char * name, int region, int offset){
	addr * address = balloc( sizeof( addr ) );
	address->region = region;
	address->offset = offset;
	address->next = NULL;
	address->name =  strdup(name);
	//printf( "inserting %s with region: %d offset %d\n", name, region, offset );
	
	// insert the address into the address ist
	if ( start_address == NULL ) {
		start_address = address;
	} else {
		// add the address to the end of the list
		addr * temp;
		temp = start_address;
		while ( temp->next != NULL ) {
			temp = temp->next;
		}
		temp->next = address;
	}
	
}
