#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "symtab.h"

// better malloc
void * balloc( int n ){
	char *p = malloc( n );
	if( p == NULL ){
		fprintf( stderr, "Out of Memory for request of %d bytes.\n", n );
		exit( -1 );
	}
	return (void *)p;
}

type * newType( char * s, int btype ){
	type * new = (type *) balloc( sizeof( type ) );
	new->basetype = btype;
	new->name = balloc( strlen(s)+1 );
	strcpy(new->name, s);
	return new;
}

type * newUnionType(char * s, struct sym_table * typ, int btype ){
	type * new = newType(s, btype);
	switch(btype){
		case FUNC_TYPE:
			new->u.f.table = typ;
			break;
		case STRUCT_TYPE:
			new->u.s.table = typ;
			break;
		case MAP_TYPE:
			new->u.m.values = typ;
			break;
		case ARRAY_TYPE:
			new->u.a.values = typ;
			new->u.a.size = 0;
			break;
	}
	return new;
}

char * typename( type * t ){
	if(!t){
		return "(NULL)";
	}
	return t->name;
}
