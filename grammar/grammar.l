%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef LR1
#include "y.tab.h"
#else
#include "grammar.h"
#endif

#define MAX_YYTEXT_LENGTH       8192

int yywrap(void){
	return 1;
}

static void copy_yytext(){
	yylval.str_value = (char*)malloc((strnlen(yytext, MAX_YYTEXT_LENGTH)+1));
	strncpy(yylval.str_value, yytext, MAX_YYTEXT_LENGTH+1);
	*(yylval.str_value+MAX_YYTEXT_LENGTH+1) = 0;
}
%}

%%
[\n]		return CR;
[ \t]		;
[a-z_]+[0-9]*	{
	copy_yytext();
	return UNTERMINAL;
}
[A-Z_]+		{
	copy_yytext();
	return TERMINAL;
}
":"		return COLON;
"|"		return OR;
.		{
	copy_yytext();
	return TERMINAL;
}
%%
