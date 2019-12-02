#include <stdio.h>
#include "../src/tokenTree.h"
#include "../src/nonDet.h"

char * outfilename;
FILE *outfile;

void codegen( struct tTree * t ) {
	int i;
	if (t==NULL) return;

   /*
    * this is a post-order traversal, so visit children first
    */
	for(i=0;i<t->nbranches;i++)
		codegen(t->branches[i]);

   /*
    * back from children, consider what we have to do with
    * this node. The main thing we have to do, one way or
    * another, is assign t->code
    */
	switch (t->prodrule) {
		case ND_PLUS: {
			struct instr *g;
			t->code = concat( t->branches[0]->code, t->branches[1]->code );
			g = gen( ND_PLUS, t->address, t->branches[0]->address, 
				t->branches[1]->address );
			t->code = concat( t->code, g );
//			printf("add\n");
			break;
		}
		case ND_VARDCL:
			insert_address( t->branches[0]->leaf->text, t->branches[0]->address->region, t->branches[0]->address->offset );
			break;
		case ND_FNDCL:{
			// for leafs
			// insert addres into table
			insert_address( t->branches[0]->leaf->text, t->branches[0]->address->region, t->branches[0]->address->offset );
			break;
		}
		/*
		 * ... really, a bazillion cases, up to one for each
		 * production rule (in the worst case)
		 */
		default:
			/* default is: concatenate our children's code */
			//fprintf(outfile, "in default\n");
			//if( t->leaf != NULL)
			//printf("default looking at: %s\n", t->leaf->text);
			t->code = NULL;
			//printf("t->code: %p\n", t->code);
/*			for(i=0;i<t->nbranches;i++)
				t->code = concat(
					t->code, 
					t->branches[i]->code);
*/		}
}

