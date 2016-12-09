#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
	NUMBER = 258,
	STR = 259,
	Variable = 260,
	INT = 261,
	VOID = 262,
	WHILE = 263,
	RETURN = 264,
	PRINT = 265,
	SCAN = 266,
	IF = 267,
	ELSE = 268,
	CMP = 269,
	ASSIGN = 270,
	EOL = 271
};

extern int yylex();
extern int yylval;
extern char* yytext;

int tok;

void advance()
{
	tok = yylex();
	printf("tok:%s\n", yytext);
}

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
	char* ivalue;
	char* nodeType;
	past left;
	past mid;
	past right;
};

past newastNode()
{
	past node = malloc(sizeof(ast));
	if (node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past astprogram();
past astex_decl();
past astdecl_or_stmt();
past astdecl_list();
past astintstr_list();
past astdecl();
past astparameter_list();
past aststmt();
past aststmt_list();
past astparameter();
past astexpr_stmt();
past astexpr_list();
past astid_list();
past astmul_expr();
past astprimary_expr();
past astexpr();
past astcmp_expr();
past astadd_expr();
past astNUMBER(char* value);
past astSTRING(char* value);
past astID(char* value);
past astTYPE(char* value);
past newList(past left, past right);
past newResult(past left, past mid, past right);




past astNUMBER(char* value)
{
	past var = newastNode();
	var->nodeType = "intValue";
	var->ivalue = value;
	return var;
}
past astSTRING(char* value)
{
	past var = newastNode();
	var->nodeType = "STRING";
	var->ivalue = value;
	return var;
}
past astID(char* value)
{
	past var = newastNode();
	var->nodeType = "ID";
	var->ivalue = value;
	return var;
}
past astTYPE(char* value)
{
	past var = newastNode();
	var->nodeType = "TYPE";
	var->ivalue = value;
	return var;
}
past newList(past left, past right)
{
	past var = newastNode();
	var->nodeType = "expr";
	var->left = left;
	var->right = right;
	return var;
}
past newResult(past left, past mid, past right)
{
	past var = newastNode();
	var->nodeType = "result";
	var->left = left;
	var->mid = mid;
	var->right = right;
	return var;
}

past astprogram()
{
	advance();
	past list = NULL;
	list = newList(NULL, astex_decl());
	while (tok!= '\0')
	{
		advance();
		list = newList(list, astex_decl());
	}
	return list;
}
past astex_decl()
{
	past result = NULL;
	past t = astTYPE(yytext);
	advance();
	past d = astdecl();
	past ds = astdecl_or_stmt();
	result = newResult(t, d, ds);
	return result;
}
past astdecl_or_stmt()
{
	past list = NULL;
	if (tok == '{')
	{
		advance();
		if (tok == '}')
			advance();
		else
		{
			list = newList(NULL, aststmt_list());
			advance();
		}
	}
	if (tok == ',')
	{
		advance();
		list = newList(NULL, astdecl_list());
		advance();
	}
	if (tok == ';')
		advance();
	return list;
}
past astdecl_list()
{
	past list = newList(NULL, astdecl());
	while (tok == ',')
	{
		advance();
		list = newList(list, astdecl());
	}
	return list;
}
past astintstr_list()
{
	past list = NULL;
	if (tok == NUMBER)
	{
		advance();
		past list = newList(NULL, astNUMBER(yytext));

	}
	if (tok == STR)
	{
		advance();
		past list = newList(NULL, astSTRING(yytext));
	}
	while (tok == ',')
	{
		advance();
		if (tok == NUMBER)
		{
			advance();
			list = newList(list, astNUMBER(yytext));
		}
		if (tok == STR)
		{
			advance();
			list = newList(list, astSTRING(yytext));
		}
	}
	return list;
}
past astdecl()
{
	past result = newResult(NULL, NULL, astID(yytext));
	advance();
	if (tok == '=')
	{
		advance();
		result = newResult(NULL, astexpr(), astID(yytext));

	}
	if (tok == '(')
	{
		advance();
		if (tok == ')')
			advance();
		else
		{
			result = newResult(NULL, astparameter_list(), astID(yytext));
			advance();

		}
	}
	if (tok == '[')
	{
		advance();
		if (tok == ']')
		{
			advance();
			if (tok == '=')
			{
				advance();
				result = newResult(NULL, astintstr_list(), astID(yytext));
				if (tok == '}')
					advance();

			}
		}
		else
		{
			result = newResult(NULL, astexpr(), astID(yytext));
			advance();
			result = newResult(astintstr_list(), astexpr(), astID(yytext));
			if (tok == '}')
				advance();

		}
	}
	return result;
}
past astparameter_list()
{
	past list = newList(NULL, astparameter());
	while (tok == ',')
	{
		advance();
		list = newList(list, astparameter());
	}
	return list;
}
past aststmt()
{
	past result = NULL;
	if (tok == INT || tok == STR || tok == VOID)
	{
		result = newResult(NULL, NULL, astTYPE(yytext));
		advance();
		result = newResult(NULL, astdecl_list(), astTYPE(yytext));
	}
	if (tok == '{')
	{
		advance();
		result = newResult(NULL, NULL, astexpr_stmt());
		advance();
	}
	if (tok == IF)
	{
		advance();
		result = newResult(NULL, NULL, astexpr());
		advance();
		result = newResult(NULL, aststmt(), astexpr());
		if (tok == ELSE)
		{
			advance();
			result = newResult(aststmt(), aststmt(), astexpr());
		}
	}
	if (tok == WHILE)
	{
		advance();
		result = newResult(NULL, NULL, astexpr());
		advance();
		result = newResult(NULL, aststmt(), astexpr());
	}
	if (tok == RETURN)
	{
		advance();
		if (tok == ';')
			advance();
		else
		{
			result = newResult(NULL, NULL, astexpr());
			advance();
		}
	}
	if (tok == PRINT)
	{
		advance();
		if (tok == ';')
			advance();
		else
		{
			result = newResult(NULL, NULL, astexpr_list());
			advance();
		}
	}
	return result;
}
past aststmt_list()
{
	advance();
	past st = aststmt();
	past list = newList(NULL, st);
	while (tok != '\0')
	{
		advance();
		st = aststmt();
		list = newList(list, st);
	}
	return list;
}
past astparameter()
{
	past list = NULL;
	list = newList(NULL, astTYPE(yytext));
	advance();
	list = newList(astID(yytext), astTYPE(yytext));
	advance();

	return list;
}

past astYPE(){

	past result = NULL;
	if (tok == INT){
		advance();
		result = newList(NULL,astTYPE(yytext));
	}
	else if (tok == STR){
		advance();
		result = newList(NULL, astTYPE(yytext));
	}
	else if (tok == VOID){
		advance();
		result = newList(NULL, astTYPE(yytext));
	}
	return result;
}
past astexpr_stmt()
{
	past list = NULL;
	if (tok == ';')
		advance();
	else
	{
		list = newList(NULL, astexpr());
		advance();
	}
	return list;
}
past astexpr_list()
{
	past list = newList(NULL, astexpr());
	while (tok == ',')
	{
		advance();
		list = newList(list, astexpr());
	}
	return list;
}
past astid_list()
{
	past list = newList(NULL, astID(yytext));
	while (tok == ',')
	{
		advance();
		list = newList(list, astID(yytext));
	}
	return list;
}
past astmul_expr()
{
	past list = newList(NULL, astprimary_expr());
	while (tok == '*' || tok == '/' || tok == '%')
	{
		advance();
		list = newList(list, astprimary_expr());
	}
	if (tok == '-')
	{
		advance();
		list = newList(NULL, astprimary_expr());
	}
	return list;
}
past astprimary_expr()
{
	past result = NULL;
	if (tok == Variable)
	{
		result = newResult(NULL, NULL, astID(yytext));
		advance();
		if (tok == '(')
		{
			advance();
			if (tok == ')')
				advance();
			else
			{
				result = newResult(NULL, astexpr_list(), astID(yytext));
				advance();
			}
		}
		if (tok = ASSIGN)
		{
			advance();
			result = newResult(NULL, astexpr(), astID(yytext));
		}
		if (tok == '=')
		{
			advance();
			result = newResult(NULL, astexpr(), astID(yytext));
		}
		if (tok == '[')
		{
			advance();
			result = newResult(NULL, astexpr(), astID(yytext));
			advance();
			if (tok == '=')
			{
				advance();
				result = newResult(astexpr(), astexpr(), astID(yytext));
			}
		}
	}
	if (tok == '(')
	{
		advance();
		result = newResult(NULL, NULL, astexpr());
		advance();
	}
	if (tok == NUMBER)
	{
		result = newResult(NULL, NULL, astNUMBER(yytext));
		advance();
	}
	if (tok == STR)
	{
		result = newResult(NULL, NULL, astSTRING(yytext));
		advance();
	}
	return result;
}
past astexpr()
{
	past list = newList(NULL, astcmp_expr());
	return list;
}
past astcmp_expr()
{
	past list = newList(NULL, astadd_expr());
	while (tok == CMP)
	{
		advance();
		list = newList(list, astadd_expr());
	}
	return list;
}
past astadd_expr()
{
	past list = newList(NULL, astmul_expr());
	while (tok == '+' || tok == '-')
	{
		advance();
		list = newList(list, astmul_expr());
	}
	return list;
}


void showast(past node, int nest)
{
	if (node == NULL)
		return;
	int i = 0;
	for (i = 0; i < nest; i++)
		printf("  ");
		printf("%s %c\n", node->nodeType);
		showast(node->left, nest + 1);
		showast(node->mid, nest + 1);
		showast(node->right, nest + 1);
}
int main()
{

	past rr = astprogram();
	showast(rr, 0);
	return 0;
}
