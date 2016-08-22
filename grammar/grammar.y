%{
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG	1
%}

%union {
	char char_value;
}
%token <char_value> UNTERMINAL TERMINAL COLON OR CR EMPTY_RESULT
%type  <char_value> product_left product_right

%%
line_list
: line 
| line line_list
;

line
:product_left COLON product_right CR
{
	printf ("Right product!\n");
}
;
product_left
:UNTERMINAL
{
	extern char *yytext;
	printf ("product_left:%s\n", yytext);
}
;
product_right
:TERMINAL
|UNTERMINAL
|TERMINAL product_right
|UNTERMINAL product_right
|TERMINAL OR product_right
|UNTERMINAL OR product_right
|EMPTY_RESULT
;

%%

int yyerror(char const* str){
	extern char* yytext;
	fprintf(stderr, "parse error!\n");
	return 0;
}

int main(void){
	extern int yyparse(void);
	extern FILE* yyin;
	yyin = stdin;

	if (yyparse()){
		fprintf(stderr, "Error!\n");
		exit(1);
	}
	return 0;
}
