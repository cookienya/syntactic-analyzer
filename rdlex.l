%{
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
int yylval;

%}


%%


"int" { return INT;}
"void" { return VOID;}
"while" { return WHILE;}
"return" { return RETURN;}
"print" { return PRINT;}
"scan" {return SCAN;}
"if" { return IF;}
"else" { return ELSE;}
"cmp" { return CMP;}
"assign" { return ASSIGN;}

"+" |
"-" |
"*" |
"/" |
"q" |
"{" |
"}" |
"," | 
";" |
"=" |
"(" |
"<" |
")" |
"[" |
"]" { return yytext[0]; }

\".*\" { return STR;}

[0-9]+ {return NUMBER;}

\n	{ return EOL; }

[a-zA-Z_][a-zA-Z0-9]* {return Variable;}

[ \t] { /* ignore whitespace */ }

%%

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}
int yywrap()
{
	return 1;
}
