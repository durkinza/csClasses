#ifndef TAC_H
#define TAC_H
#include "../src/tokenTree.h"
#include "../src/nonDet.h"

typedef struct addr {
	int region, offset;
	char * name;
	struct addr * next;
} addr;

addr * start_address;

/* Regions: */
#define R_GLOBAL 20010 /* can assemble as relative to the pc */
#define R_LOCAL  20020 /* can assemble as relative to the ebp */
#define R_CLASS  20030 /* can assemble as relative to the 'this' register */
#define R_LABEL  20040 /* pseudo-region for labels in the code region */
#define R_CONST  20050 /* pseudo-region for immediate mode constants */

typedef struct instr {
	int opcode;
	struct addr * dest, * src1, * src2;
	struct instr * next;
} instr;
/* Opcodes, per lecture notes */
#define O_ADD   30010
#define O_SUB   30020
#define O_MUL   30030
#define O_DIV   30040
#define O_NEG   30050
#define O_ASN   30060
#define O_ADDR  30070
#define O_LCONT 30080
#define O_SCONT 30090
#define O_GOTO  30100
#define O_BLT   30110
#define O_BLE   30120
#define O_BGT   30130
#define O_BGE   30140
#define O_BEQ   30150
#define O_BNE   30160
#define O_BIF   30170
#define O_BNIF  30180
#define O_PARM  30190
#define O_CALL  30200
#define O_RET   30210
/* declarations/pseudo instructions */
#define D_GLOB  30510
#define D_PROC  30520
#define D_LOCAL 30530
#define D_LABEL 30540
#define D_END   30550

instr * gen( int, struct addr *, struct addr * , struct addr * );
instr * concat( struct instr *, struct instr * );


void insert_address( char *, int, int );
#endif
