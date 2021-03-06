
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nonDet.h"
#include "../_build/yacc.tab.h"
char * prod_to_string(int prodrule){
	switch(prodrule){
		case T_ANDAND:
			return "&&";
		case T_EQUAL:
			return "equal_comparisson";
		case T_NOT_EQUAL:
			return "not_equal";
		case T_LTHAN:
			return "<";
		case T_GTHAN:
			return ">";
		case T_LTHANEQUAL:
			return "<=";
		case T_GTHANEQUAL:
			return ">=";	
		case T_RETURN:
			return "return";


		// Operator types
		case T_PLUS:
		case ND_PLUS:
			return "plus";
		case T_MINUS:
		case ND_MINUS:
			return "minus";
		case ND_DIVIDE:
			return "divide";
		case ND_MULTIPLY:
			return "multiply";
		case ND_MOD:
			return "mod";
		case ND_ADDEQ:
			return "+=";
		case ND_SUBEQ:
			return "-=";
		case ND_COLAS:
			return "colas";
		case ND_ASSIGNMENT:
			return "assignment";
		case ND_INCREMENT:
			return "increment";
		case ND_DECREMENT:
			return "detriment";
		case ND_OROR:
		case T_OROR:
			return "||";
		case ND_MAP:
			return "map";


		// Packages
		case ND_FILE:
			return "file";
		case ND_PACKAGE:
			return "package";
		case ND_IMPORTS:
			return "imports";
		case ND_IMPORT:
			return "import";
		case ND_IMPORT_STMT:
			return "import_stmt";
		case ND_IMPORT_STMT_LIST:
			return "import_stmt_list";
		case ND_IMPORT_HERE:
			return "import_here";
		case ND_IMPORT_PACKAGE:
			return "import_package";
		case ND_IMPORT_THERE:
			return "import_there";
		case ND_IMPORT_SAFETY:
			return "import_safety";


		// Body
		case ND_XDCL_LIST:
			return "xdcl_list";
		case ND_XDCL:
			return "xdcl";
		case ND_COMMON_DCL:
			return "common_dcl";
		case ND_CONSTDCL:
			return "constdcl";
		case ND_CONSTDCL_LIST:
			return "constdcl_list";
		case ND_VARDCL:
			return "variable_declaration";
		case ND_VARDCL_ASSIGN:
			return "variable_declaration_and_assignment";
		case ND_VARDCL_LIST:
			return "variable_declaration_list";
		case ND_TYPEDCL:
			return "type_declaration";
		case ND_TYPEDCL_LIST:
			return "type_declaration_list";
		case ND_DCL_NAME_LIST:
			return "dcl_name_list";
		case ND_DCL_NAME:
			return "dcl_name";
		case ND_NEW_NAME:
			return "new_name";
		case ND_NEW_NAME_LIST:
			return "new_name_list";
		case ND_HIDDEN_IMPORTSYM:
			return "hidden_importsym";
		case ND_STMT:
			return "stmt";
		case ND_STMT_LIST:
			return "stmt_list";
		case ND_COMPOUND_STMT:
			return "compound_stmt";
		case ND_NON_DCL_STMT:
			return "non_dcl_stmt";
		case ND_INDCL:
			return "indcl";
		case ND_LOOP_BODY:
			return "loop_body";
		case ND_RANGE_STMT:
			return "Range_stmt";

		case ND_VAR_CALL:
			return "variable Call";

		case ND_KEYVAL:
			return "keyval";
		case ND_BARE_COMPLITEXPR:
			return "bare_complitexpr";
		case ND_COMPLITEXPR:
			return "complitexpr";
		case ND_KEYVAL_LIST:
			return "keyval_list";
		case ND_BRACED_KEYVAL_LIST:
			return "braced_keyval_list";
		case ND_PACKNAME:
			return "packname";

		case ND_IF:
			return "if";
		case ND_ELSE_IF:
			return "else_if";
		case ND_ELSE:
			return "else";
		case ND_ELSE_IF_LIST:
			return "else_if_list";
		case ND_IF_HEADER:
			return "if_header";
		
		case ND_FOR:
			return "for";
		case ND_FOR_STMT:
			return "for_stmt";
		case ND_FOR_HEADER:
			return "for_header";
		case ND_FOR_BODY:
			return "for_body";
			
		case ND_STRUCT:
			return "struct";
		case ND_STRUCTDCL_LIST:
			return "structdcl_list";
		case ND_STRUCTDCL:
			return "structdcl";


		// Functions
		case ND_XFNDCL:
			return "xfndcl";
		case ND_FNDCL:
			return "function_declaration";		
		case ND_FNTYPE:
			return "function_type";
		case ND_FNLITERAL:
			return "fnliteral";	
		case ND_FNBODY:
			return "function_body";
		case ND_FNRES:
			return "function_response";
		case ND_ARG_TYPE:
			return "arg_type";
		case ND_ARG_TYPE_LIST:
			return "arg_type_list";
		
		// Expressions
		case ND_EXPR:
			return "expr";
		case ND_EXPR_OR_TYPE_LIST:
			return "expression_type_list";
		case ND_UEXPR:
			return "uexpr";
		case ND_PEXPR:
			return "pseudo_expression";
		case ND_PEXPR_NO_PAREN:
			return "pseudo_expression_no_paren";		
		case ND_PSEUDOCALL:
			return "pseudocall";

		// Namings
		case ND_DOTNAME:
			return "dotname";
		case ND_POINTER:
			return "pointer";
		case ND_OTHERTYPE:
			return "other_type";
		case ND_COMPTYPE:
			return "comptype";
	}	
	static char ret[30];
	sprintf(ret, "%d:not_implemented_yet", prodrule);
	return ret;
}
