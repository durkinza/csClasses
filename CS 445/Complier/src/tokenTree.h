#ifndef TOKEN_H
#define TOKEN_H
#define MAX_BRANCHES 30
typedef struct token {
	int category;	/* the integer code returned by yylex */
	char *text;		/* the actual string(lexeme) matched */
	int colno;		/* the column number on which the token occurs */
	int lineno;		/* the line number on which the token occurs */
	char *filename;	/* the source file in which the token occurs */
	int ival;		/* for integer constants, store binary value here */
	double dval;	/* for real constants, store binary value here*/
	char *sval;		/* for string constants, malloc space, de-escape, store
						the string (less quotes and after escapes) here */	
} token;
typedef struct tTree {
	int prodrule;		/* prodrule is the action of this tree (ex. '+' for addition) */
	int nbranches;
	int ret_type; /* return types for expr */
	struct tTree *branches[MAX_BRANCHES];
	struct token *leaf;
} tTree;

token * create_token(int category, char *text, int colno, int lineno, char *filename, char * value);
token * quick_token(int category);
tTree * token_as_tree(int category);

// all branches must be of (tTree *) type
tTree * create_tree(int prodrule, int nbranches, ...);

tTree * push_to_tree(token * leaf);

void delete_tree(tTree * tree);
void delete_trees(int count, ...);
void delete_token(token * leaf);
void print_token(token * leaf, int depth);
void print_prodrule(int prodrule, int nbranches, int depth);
void print_tree(tTree * tree);
void _print_tree(tTree *tree, int depth);
char * clean_sval(char * string);
#endif
