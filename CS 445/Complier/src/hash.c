/* 
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "types.h"


// Generates a hash which has up the given number of buckets 
unsigned int hash( char *s, int buckets ){

    unsigned hashVal;
    
    for( hashVal = 0 ; *s != '\0' ; s++ )
        hashVal = *s + 31 * hashVal;
        
    return  hashVal % buckets;
}

hashtable * newHashTable(int nbuckets){
	hashTable * new = malloc( sizeof( hashTable ) );
	new->table = malloc( nbuckets * sizeof( hashEntry ) );
}


hashEntry * _lookup (hashTable table, char *s){
	unsigned int h = hash(s, table->nbuckets);
	// get the linked list from the hash table
	hashEntry * temp = table->table[h];

	// check that the table isn't empty
	if (temp != NULL){
		// traverse the list until we find the element
		// or until we reach the end.
		while(temp != NULL && strcmp(temp->name, s) != 0){
			temp = temp->next;
		}
	}
	return temp;
}

type * lookup( hashTable table, char * s ){
	hashEntry temp = _lookup( table, s );
	if( temp != NULL ){
		return temp->type;
	}
	return NULL;
}

type * insert( hashTable table, char *s, type * type ){
	unsigned int h = hash( s, table->nbuckets );
	hashEntry temp = _lookup( table, s );
	if ( temp == NULL ){
		// if the element is new.
		hashEntry * new = malloc( sizeof( hashEntry ) );
		new -> type = type;
		new -> name = strcpy;
		// add the element to the top of the liked list
		new -> next = hashTable->table[h];
		// move the poiter to be at the top.
		hashTable->table.table[h] = new;
	}else{
		// if the element already exists in the table;
		// remove the old element
		free( temp->type );
		// set the new element
		temp->type = type;	
	}		
}

void printHashTable( hashtable table ){
	printf("printing of hash table no implemented.\n");
}
