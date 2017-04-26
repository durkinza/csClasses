/* Hash.cpp
 *
 *  Hash table implementation from:
 *  Kernighan & Ritchie, The C Programming Language,
 *     Second Edition, Prentice-Hall, 1988.
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

using namespace std;

#include "hash.h"

const int HASH_TABLE_SIZE = 49957;
static NListPtr hashTable[HASH_TABLE_SIZE];

    //  Prototypes
static char *Strdup( const char * );  //  in cstring, but....



/*  Hash
 *  Generate hash value for string s
 */

unsigned Hash( char *s )
{
    unsigned hashVal;
    
    for( hashVal = 0 ; *s != '\0' ; s++ )
        hashVal = *s + 31 * hashVal;
        
    return  hashVal % HASH_TABLE_SIZE;
}


/*  Lookup
 *  Look for s in hashTable
 */

NListPtr Lookup( char *s , int &x)
{
    NListPtr np;
	x=0;
    for( np = hashTable[Hash(s)] ; np != NULL ; np = np->next )
    {
		x++;
        if( strcmp(s, np->name) == 0 )
            return np;    //  found
    }
    
    return NULL;          //  not found
}

/* search
 * look for (word) in hash table
 */

void search( char *w ){
	int x = 0;
	NListPtr np = Lookup(w, x);
	if(np != NULL){
		cout << np->name << " Found in " << x << " probe(s)" << endl;
	}else{
		cout << w << " could not be found" << endl;
	}
}


/*  Insert
 *  Put (name, defn) in hash table
 */
 
NListPtr Insert( char *name, char *defn )
{
    unsigned hashVal;
    NListPtr np;
	int temp; 
    if( (np = Lookup(name, temp)) == NULL )  // not found
    {
        np = (NListPtr) malloc(sizeof(*np));
        if( np == NULL || (np->name = Strdup(name)) == NULL )
            return NULL;
        hashVal = Hash(name);
        np->next = hashTable[hashVal];
        hashTable[hashVal] = np;
    }
    else
    {      //  remove previous definition
        free( (void *)np->defn );
    }
    
    if( (np->defn = Strdup(defn)) == NULL )
        return NULL;
    
    return np;
}


/*  PrintHashTable
 *  Print the hash table contents
 */

void PrintHashTable()
{
    NListPtr np;

    cout << "Hash table contents:" << endl;
    cout << "--------------------\n" << endl;

	int max = 0;
	int max_index = 0;
	int min = -1; 
	int min_index = 0;
	int many_empty = 0;
	
    for( int i = 0 ; i < HASH_TABLE_SIZE ; i++ ){
        np = hashTable[i];
		int j=0;
        while( np != NULL ){
			 j++;
             //cout << setw(3) << i << "-" << j << ":    ";
             //cout << np->name << ", " << np->defn;
             //cout << endl;
             np = np->next;
        }
		cout << "b[" << i << "] has " << j << " word(s) | ";
		if(j < min || min == -1){
			min = j;
			min_index = i;
		}
		if(j == 0){
			many_empty++;
		}
		if(j > max){
			max = j;
			max_index = i;
		}
    }
    cout << "\n--------------------\n" << endl;
	cout << "Max bucket size: " << max << " on bucket "<< max_index << endl; 
	cout << "Min bucket size: " << min << " on bucket " << min_index << endl;
	cout << many_empty << " buckets are empty "<< endl << endl;
}


/*  Strdup
 *  Make a duplicate copy of s
 */

static char *Strdup( const char *s )
{
    char *p;
    
    p = (char *) malloc(strlen(s)+1);  /*  +1 for '\0'  */
    if( p != NULL )
        strcpy(p,s);

    return p;
}
