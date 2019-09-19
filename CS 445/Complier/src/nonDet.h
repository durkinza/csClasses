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
#define ND_ASOP				1008
#define ND_COLAS			1009
#define ND_ASSIGNMENT		1010
#define ND_INCREMENT		1011
#define ND_DECREMENT		1012
#define ND_OROR				1013
#define ND_ANDAND			1014
#define ND_EQUAL			1015
#define ND_NOT_EQUAL		1016

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
#define ND_CONSTDCL_LIST	3004
#define	ND_VARDCL			3005
#define ND_VARDCL_ASSIGN	2019
#define	ND_VARDCL_LIST		3006
#define ND_TYPEDCL			3007
#define ND_TYPEDCL_LIST		3008
#define ND_DCL_NAME_LIST	3009
#define ND_DCL_NAME			3010
#define ND_NEW_NAME			3011
#define ND_HIDDEN_IMPORTSYM	3012
#define ND_STMT				3013
#define ND_STMT_LIST		3014
#define ND_COMPOUND_STMT	3015
#define ND_NON_DCL_STMT		3016
#define ND_INDCL			3020

#define ND_IF				3100
#define ND_ELSE_IF			3101
#define ND_ELSE				3102
#define ND_ELSE_IF_LIST		3103


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
#define ND_UEXPR			5100
#define ND_PEXPR			5200
#define ND_PEXPR_NO_PAREN	5201
#define ND_PSEUDOCALL		5300


// Namings
#define ND_DOTNAME			6000
#define ND_POINTER			6001
#define ND_OTHERTYPE		6002


char * prod_to_string(int prodrule);




#endif
