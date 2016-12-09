/*
============================================================================
Name        : rdparser.c
Author      :
Version     :
Copyright   : Your copyright notice
Description : Hello World in C, Ansi-style
============================================================================
*/

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


int external_declaration();
int program();
int type();
int declarator();
int decl_or_stmt();
int statement_list();
int declarator_list();
int intstr_list();
int expr();
int parameter_list();
int parameter();
int id_list();
int expr_statement();
int expression_statement();
int cmp_expr();
int add_expr();
int mul_expr();
int primary_expr();
int statement();
int expr_list();

int tok;


void advance()
{
	tok = yylex();
	printf("tok: %s\n", yytext);
}


//program
//: external_declaration
//| program external_declaration
//;
int program(){
	external_declaration();
	while (tok != '\0'){
		external_declaration();
	}
	printf("yes\n");
}

//external_declaration
//: type declarator decl_or_stmt
//;
int external_declaration(){
	while (tok != '\0'){
		type();
		declarator();
		decl_or_stmt();
		advance();
	}
}

//decl_or_stmt
//: '{' statement_list '}'
//| '{' '}'
//| ',' declarator_list ';'
//| ';'
//;
int decl_or_stmt(){
	if (tok == '{'){
		advance();
		if (tok == '}'){}
		else{
			statement_list();
		}
	}
	else if (tok == ','){
		advance();
		declarator_list();
	}
	else if (tok == ';'){}
}

//declarator_list
//: declarator
//| declarator_list ',' declarator
//;
int declarator_list(){
	declarator();
	advance();
	while (tok == ','){
		declarator();
	}
}

//intstr_list
//: NUMBER | STRING
//| intstr_list ',' NUMBER
//| intstr_list ',' STRING
//;
int intstr_list(){
	if (tok == NUMBER || tok == STR){
		while (tok == ','){
			advance();
			if (tok == NUMBER || tok == STR){}
		}
	}
}

//declarator
//: ID
//| ID '=' expr
//| ID '(' parameter_list ')'
//| ID '(' ')'
//| ID '[' expr ']'
//| ID '[' ']'
//| ID '[' expr ']' '=' '{' intstr_list '}'
//| ID '[' ']' '=' '{' intstr_list '}'
//;
int declarator(){
	if (tok == Variable){
		advance();
		if (tok == '='){
			advance();
			expr();
		}
		else if (tok == '('){
			advance();
			if (tok == ')'){}
			else{
				parameter_list();
			}
		}
		else if (tok == '['){
			advance();
			if (tok == ']'){
				advance();
				if (tok == '='){
					advance();
					if (tok == '{'){
						advance();
						intstr_list();
					}
				}
			}
			else {
				expr();
				advance();
				if (tok == ']'){
					advance();
					if (tok == '{'){
						advance();
						intstr_list();
					}
				}
			}
		}
	}
}

//parameter_list
//: parameter
//| parameter_list ',' parameter
//;
int parameter_list(){
	parameter();
	while (tok == ','){
		advance();
		parameter();
	}
}

//parameter
//: type ID
//;
int parameter(){
	if (type(tok) == 1){}
	if (tok == Variable){}
}

//type
//: INT
//| STR
//| VOID
//;
int type(){
	if (tok == INT){}
	if (tok == STR){}
	if (tok == VOID){}
}

//statement
//: type declarator_list ';'
//| '{' statement_list '}'
//| expr_statement                
//| IF '(' expr ')' statement
//| IF '(' expr ')' statement ELSE statement
//| WHILE '(' expr ')' statement
//| RETURN ';'
//| RETURN expr ';'
//| PRINT ';'
//| PRINT expr_list ';'
//| SCAN id_list ';'
//;
int statement(){
	if (tok == INT || tok == STR || tok == VOID){
		advance();
		declarator_list();
	}
	else if (tok == '{'){
		advance();
		statement_list();
	}
	else if (tok == IF){
		advance();
		if (tok == '('){
			advance();
			expr();
			if (tok == ')'){
				advance();
				statement();
				if (tok == ELSE){
					advance();
					statement();
				}
			}
		}
	}
	else if (tok == WHILE){
		advance();
		if (tok == '('){
			advance();
			expr();
			if (tok == ')'){
				advance();
				statement();
			}
		}
	}
	else if (tok == RETURN){
		advance();
		if (tok == ';'){}
		else expr();
	}
	else if (tok == PRINT){
		advance();
		if (tok == ';'){}
		else expr_list();
	}
	else if (tok == SCAN){
		advance();
		id_list();
	}
	else expr_statement();
}

//statement_list
//: statement
//| statement_list statement
//;
int statement_list(){
	statement();
	while (tok != '\0'){
		advance();
		statement();
	}
}

//expression_statement
//: ';'
//| expr ';'
//;
int expression_statement(){
	if (tok == ';'){}
	else{
		advance();
		expr();
	}
}

//expr
//: cmp_expr
//;
int expr(){
	cmp_expr();
}

//cmp_expr
//: add_expr
//| cmp_expr CMP add_expr
//;
int cmp_expr(){
	add_expr();
	while (tok == CMP){
		advance();
		add_expr();
	}
}

//add_expr
//: mul_expr
//| add_expr '+' mul_expr
//| add_expr '-' mul_expr
//;
int add_expr(){
	mul_expr();
	while (tok == '+' || tok == '-'){
		advance();
		mul_expr();
	}
}

//mul_expr
//: primary_expr
//| mul_expr '*' primary_expr
//| mul_expr '/' primary_expr
//| mul_expr '%' primary_expr
//| '-' primary_expr
//;
int mul_expr(){
	primary_expr();
	while (tok == '*' || tok == '/' || tok == '%'){
		advance();
		primary_expr();
	}
	if (tok == '-'){
		advance();
		primary_expr();
	}
}

//primary_expr
//: ID '(' expr_list ')'
//| ID '(' ')'
//| '(' expr ')'
//| ID
//| NUMBER
//| STRING
//| ID ASSIGN expr
//| ID '=' expr
//| ID '[' expr ']'
//| ID '[' expr ']' '=' expr
//;
int primary_expr(){
	if (tok == Variable){
		advance();
		if (tok == '('){
			advance();
			if (tok == ')'){}
			else expr_list();
		}
	}
	else if (tok == '('){
		advance();
		expr();
	}
	else if (tok == NUMBER){}
	else if (tok == STR){}
	else if (tok == Variable){
		advance();
		if (tok == ASSIGN){
			advance();
			expr();
		}
		else if (tok == '='){
			advance();
			expr();
		}
		else if (tok == '['){
			advance();
			expr();
			if (tok == ']'){
				advance();
				if (tok == '='){
					advance();
					expr();
				}
			}
		}
	}
}

//expr_list
//: expr
//| expr_list ',' expr
//;
int expr_list(){
	expr();
	while (tok == ','){
		advance();
		expr();
	}
}

//id_list
//: ID
//| id_list ',' ID
//;
int id_list(){
	if (tok == Variable){
		while (tok == ','){
			advance();
			if (tok == Variable){}
		}
	}
}

//expr_statement
//: ';'
//| expr ';'
//;
int expr_statement(){
	if (tok == ';'){}
	else expr();
}


int main()
{

	advance();
	program();
	return 0;
}
