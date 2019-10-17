#ifndef TYPE_H
#define TYPE_H



// The list of base types
#define NULL_TYPE		100000
#define BOOLEAN_TYPE	100001
#define INT_TYPE		100002
#define FLOAT_TYPE		100003
#define STRING_TYPE		100004

#define FUNC_TYPE		100100
#define MAP_TYPE		100200
#define ARRAY_TYPE		100300
#define STRUCT_TYPE		100400
#define PACKAGE_TYPE	100500
typedef struct type {
	// the basetype will tell us what type of struct to use
	// types INT, FLOAT, String, etc. Will not use the union
	int basetype;
	
	// flags are set to tell us about any special properties of this element. 
	// if the variable is a constant
	int cons;
	// if the variable was set as a parameter
	int parameter;

	
	char * name;
	// This union will hold all structs for each type that
	// needs more info
	union{
		
		// Maps link a starting type to another type.
		struct map{
			// the type to take as an input
			struct type * index_type;
			// the type to give as an output
			struct type * element_type;
			// the hashtable of values;
			struct sym_table * values;
		}m;

		// Array link a starting type to another type.
		struct array{
			// the number of elements in the array
			int size;// Not needed, since nentries is in sym_table
			// The type of elements in the array
			struct type * element_type;
			// linked list of values
			struct sym_table * values;
		}a;

		// structs can hold an internal set of types
		struct struc{
			// the varibale table for the struct
			struct sym_table * table;
		}s;

		// Global / Functions can hold an internal symbol table
		struct function{
			// return type of the function.
			// Global won't use this.
			struct type * ret;
			// local variables in the function tbale;
			struct sym_table * table;
		}f;

		struct package {
			// for packages
			struct sym_table * table;
		}p;

		// hold token for literal element
		struct tTree * t;
	}u;

} type;

typedef struct sym_table {
	int nBuckets;				/* # of buckets */
	int nEntries;				/* # of total elements in the table */
	struct sym_table * parent;	/* The parent scope */
	struct type * scope;		/* The type we blong to */
	struct sym_entry ** table;	/* Hash table of elements*/
} sym_table;


/*
 * Each entry in the table.
*/
typedef struct sym_entry {
	struct sym_table * table;	/* Our parent symbol entry */
	char *s;					/* The string / key / name of this element */
	struct type * type;			/* The type / value of this entry */
	struct sym_entry * next;	/* The pointer to the next entry in the liked list */
} sym_entry;




void * balloc(int size);
type * newType(char *s, int btype);
type * newUnionType(char *s, struct sym_table * typ, int btype);
char * typename(type * t);

#endif
