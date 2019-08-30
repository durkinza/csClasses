// will be thrown away when we use bison to generate the yytab.h

#ifndef YYTAB_H
#define YYTAB_H


// basic types
#define T_NULL 258
#define T_BOOLEAN 259
#define T_INTEGER 260
#define T_FLOAT64 261
#define T_STRING 262

// Literals 
#define T_NULLLITERAL 290
#define T_BOOLLITERAL 291
#define T_INTLITERAL 292
#define T_STRINGLITERAL 293
#define T_FLOATLITERAL 294
#define T_VARIABLE 295
#define T_CHARLITERAL 296 // not used


// reserved words
#define T_FUNC 300
#define T_MAP 301
#define T_STRUCT 302
#define T_ELSE 303
#define T_PACKAGE 304
#define T_CONST 305
#define T_IF 306
#define T_TYPE 307
#define T_FOR 308
#define T_IMPORT 309
#define T_RETURN 310
#define T_VAR 311

// non reserved words ( NOT IN vG0 )
#define T_BREAK 400
#define T_DEFAULT 401
#define T_INTERFACE 402
#define T_SELECT 403
#define T_CASE 405
#define T_DEFER 406
#define T_GO 407
#define T_CHAN 408
#define T_GOTO 409
#define T_SWITCH 410
#define T_FALLTHROUGH 411
#define T_RANGE 412
#define T_CONTINUE 413
#define T_WHILE 414

// operatrs
#define T_PLUS 500		/* +	*/
#define T_MINUS 501		/* -	*/
#define T_DIVIDE 502	/* /	*/
#define T_MULTIPLY 503	/* *	*/
#define T_MOD 504		/* %	*/
#define T_AND 505		/* &&	*/
#define T_OR 506		/* ||	*/
#define T_GTHAN 507		/* >	*/
#define T_LTHAN 508		/* <	*/
#define T_GTHANEQUAL 509/* >=	*/
#define T_LTHANEQUAL 510/* <=	*/
#define T_EQUAL 511		/* ==	*/
#define T_NOT_EQUAL 512 /* !=	*/
#define T_NEGATE 513	/* !	*/
#define T_LPAREN 514	/* (	*/
#define T_RPAREN 515	/* )	*/
#define T_LBRACK 516	/* [	*/
#define T_RBRACK 517	/* ]	*/
#define T_ASSIGNMENT 516/* =	*/
#define T_INCREMENT	517	/* +=	*/
#define T_DECREMENT	518	/* -=	*/

// For comments
#define T_COMMENT 600	/* //	*/


// For symbols not supported in vg0
#define T_NOTDEFINED 601	
/* 
&     &=    |      |=     
^     *=    ^=     <-
<<    /=    <<=    :=
>>    %=    >>=    :
&^    &^=
*/


#define BAD_TOKEN 999

#endif
