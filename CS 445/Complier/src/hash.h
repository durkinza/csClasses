#ifndef HASH_H
#define HASH_H

#include "types.h"

typedef struct hashTable {
	int nbuckets;
	hashEntry ** table;
} hashTable;

typedef struct hashEntry {
	char * name;
	type * type;
	hashEntry * next;
}


hashTable * newHashTable(int nbuckets);

type * lookup( hashTable table, char *s);
type * insert( hashTable table, char *s, type * type);

void PrintHashTable();

#endif  /*  HASH_H  */
