

#include "nonDet.h"
#include "../_build/yacc.tab.h"
char * prod_to_string(int prodrule){
	switch(prodrule){
		case T_SEMICOLON:
			return ";";
		case T_LPAREN:
			return "(";
		case T_RPAREN:
			return ")";
		case T_ASSIGNMENT:
			return "assign";
		case T_EQUAL:
			return "compare";
		case T_ANDAND:
			return "&&";
		case T_FUNC:
			return "func";
		case T_IF:
			return "if";
		case T_FOR:
			return "for";
		case T_IMPORT:
			return "import";
		case T_RETURN:
			return "return";
		case T_PACKAGE:
			return "package";
		case T_ELSE:
			return "else";
		case T_VAR:
			return "var";
		case T_MINUS:
			return "minus";
		case T_PLUS:
			return "plus";
	



		case ND_PLUS:
			return "plus";
		case ND_MINUS:
			return "minus";
		case ND_DIVIDE:
			return "divide";
		case ND_MULTIPLY:
			return "multiply";
		case ND_MOD:
			return "mod";
		case ND_ASOP:
			return "asop";
		case ND_COLAS:
			return "colas";
		case ND_ASSIGNMENT:
			return "assignement";
		case ND_INCREMENT:
			return "Increment";
		case ND_DECREMENT:
			return "dectement";
		case ND_OROR:
			return "||";

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
			return "import safety";

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
		case ND_HIDDEN_IMPORTSYM:
			return "hidden_importsym";
		case ND_STMT:
			return "stmt";
		case ND_STMT_LIST:
			return "stmt_list";
		case ND_COMPOUND_STMT:
			return "compount_stmt";
		case ND_NON_DCL_STMT:
			return "non_dcl_stmt";
		case ND_INDCL:
			return "indcl";

		
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
		
		
		case ND_EXPR:
			return "expr";
		case ND_UEXPR:
			return "uexpr";
		case ND_PEXPR:
			return "pseudo_expression";
		case ND_PEXPR_NO_PAREN:
			return "pseudo_expression_no_paren";		
		case ND_PSEUDOCALL:
			return "pseudocall";


		case ND_DOTNAME:
			return "dotname";
		case ND_POINTER:
			return "pointer";
		case ND_OTHERTYPE:
			return "other_type";


		default:
			return "prodrule. Not implemented yet";
		
	}	
}
