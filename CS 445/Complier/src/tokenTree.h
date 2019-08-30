#ifndef TOKEN_H
#define TOKEN_H
#define MAX_BRANCHES 9
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
   int prodrule;
   int nbranches;
   struct tTree *branches[MAX_BRANCHES];
   struct token *leaf;
} tTree;

token create_token(int category, char *text, int colno, int lineno, char *filename, int ival, double dval, char *sval);
tTree create_tree(int prodrule, token * leaf, int nbranches, tTree *tree[MAX_BRANCHES]);
tTree add_token_to_tree(tTree *tree, token *leaf);
tTree add_tree_to_tree(tTree *tree, tTree *branch);

void print_tree(tTree * tree);
void _print_tree(tTree *tree, int depth);
#endif
