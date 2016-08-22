%{
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG	1
%}

%union {
	int int_value;
	double double_value;
}

%token <double_value> DOUBLE_LITERAL
%token ADD SUB MUL DIV CR LEFT_BRACKETS RIGHT_BRACKETS 
%type <double_value> expression term primary_expression

%%
line_list
: line
|line_list line
;

line
: expression CR
{
	printf(">>%lf\n", $1);
}
;

expression
: term
| expression ADD term
{
	$$ = $1 + $3;
}
|expression SUB term
{
	$$ = $1 - $3;
}
;

term
: primary_expression
| term MUL primary_expression
{
	$$ = $1 * $3;
}
| term DIV primary_expression
{
	$$ = $1 / $3;
}
;

primary_expression
: DOUBLE_LITERAL
| LEFT_BRACKETS expression RIGHT_BRACKETS
{
	$$ = $2;
}
| SUB primary_expression
{
	$$ = -$2;
}
;
%%

int yyerror(char const* str){
	extern char* yytext;
	fprintf(stderr, "parse error near %s\n");
	return 0;
}

int main(void) {
	extern int yyparse(void);
	extern FILE* yyin;
	yyin=stdin;
	if (yyparse()){
		fprintf(stderr, "Error! Error! Error!\n");
		exit(1);
	}
}
