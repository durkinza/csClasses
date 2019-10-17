#ifndef NONDET_H
#define NONDET_H

// Operator types
#define ND_PLUS				1000
#define ND_MINUS			1001
#define ND_DIVIDE			1002
#define ND_MULTIPLY			1003
#define ND_MOD				1004
#define ND_AND				1005
#define ND_NEGATE			1006
#define ND_XOR				1007
#define ND_ADDEQ			1008
#define ND_SUBEQ			1009
#define ND_ASSIGNMENT		1010
#define ND_INCREMENT		1011
#define ND_DECREMENT		1012
#define ND_OROR				1013
#define ND_ANDAND			1014
#define ND_EQUAL			1015
#define ND_NOT_EQUAL		1016
#define ND_COLAS			1017
#define ND_MAP				1019

// Packages
#define ND_FILE				1999
#define ND_PACKAGE			2000
#define ND_IMPORTS			2001
#define ND_IMPORT			2002
#define ND_IMPORT_STMT		2003
#define ND_IMPORT_STMT_LIST 2004
#define ND_IMPORT_HERE		2005
#define ND_IMPORT_PACKAGE	2006
#define ND_IMPORT_THERE		2007
#define ND_IMPORT_SAFETY	2008

// Body
#define ND_XDCL_LIST		3000
#define ND_XDCL				3001
#define ND_COMMON_DCL		3002
#define ND_CONSTDCL			3003
#define ND_CONSTDCL1		3004
#define ND_CONSTDCL_LIST	3005
#define ND_VARDCL			3006
#define ND_VARDCL_ASSIGN	3007
#define ND_VARDCL_LIST		3008
#define ND_TYPEDCL			3009
#define ND_TYPEDCL_LIST		3010
#define ND_DCL_NAME_LIST	3011
#define ND_DCL_NAME			3012
#define ND_NEW_NAME			3013
#define ND_NEW_NAME_LIST	3014
#define ND_HIDDEN_IMPORTSYM	3015
#define ND_STMT				3016
#define ND_STMT_LIST		3017
#define ND_COMPOUND_STMT	3018
#define ND_NON_DCL_STMT		3019
#define ND_INDCL			3023
#define ND_LOOP_BODY		3033
#define ND_RANGE_STMT		3034
#define ND_VAR_CALL			3050


#define ND_KEYVAL			3105
#define ND_BARE_COMPLITEXPR	3106
#define ND_COMPLITEXPR		3107
#define ND_KEYVAL_LIST		3108
#define ND_BRACED_KEYVAL_LIST	3109
#define ND_PACKNAME			3110
#define ND_INTERFACEDCL_LIST	3111

#define ND_IF				3200
#define ND_ELSE_IF			3201
#define ND_ELSE				3202
#define ND_ELSE_IF_LIST		3203
#define ND_IF_HEADER		3204

#define ND_FOR				3300
#define ND_FOR_STMT			3301
#define ND_FOR_HEADER		3302
#define ND_FOR_BODY			3303

#define ND_STRUCT			3400
#define ND_STRUCTDCL_LIST	3401	
#define ND_STRUCTDCL		3402	


// Functions
#define ND_XFNDCL			4000
#define ND_FNDCL			4001
#define ND_FNTYPE			4002
#define ND_FNLITERAL		4003
#define ND_FNBODY			4004
#define ND_FNRES			4005
#define ND_ARG_TYPE			4009
#define ND_ARG_TYPE_LIST	4010

// Expresions
#define ND_EXPR				5000
#define ND_EXPR_OR_TYPE_LIST	5001
#define ND_EXPR_LIST		5002
#define ND_UEXPR			5100
#define ND_PEXPR			5200
#define ND_PEXPR_NO_PAREN	5201
#define ND_PSEUDOCALL		5300


// Namings
#define ND_DOTNAME			6000
#define ND_POINTER			6001
#define ND_OTHERTYPE		6002
#define ND_COMPTYPE			6003	
#define ND_DDD				6004


char * prod_to_string(int prodrule);




#endif
