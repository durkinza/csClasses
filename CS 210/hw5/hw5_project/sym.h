#ifndef SYMTBL_H
#define SYMTBL_H

#define PI		(3.14159)
#define PHI		(1.61803)

#define NSYMS   (3)
typedef struct sym {
    char * name;
    double value;
	struct sym *next;
	int edit;
} sym;
sym * sym_tbl;
struct sym * sym_lookup(char *);
struct sym *  sym_add(char * s, double val, int edit);
void printSymTbl();
int asciiOrder(char * s1, char *s2);
#endif /* SYMTBL_H */
