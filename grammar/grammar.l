%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "y.tab.h"

#define MAX_YYTEXT_LENGTH       8192

int yywrap(void){
	return 1;
}

static void copy_yytext(TOKEN* token){
	token->value = (char*)malloc((strnlen(yytext, MAX_YYTEXT_LENGTH)+1));
	strncpy(token->value, yytext, MAX_YYTEXT_LENGTH+1);
	*(token->value+MAX_YYTEXT_LENGTH+1) = 0;
}
%}
%%
[\n]		return CR;
[ \t]		;
[a-z_]+[0-9]*	{
	TOKEN* token =  (TOKEN*)malloc(sizeof(TOKEN));
	memset(token, 0, sizeof(TOKEN));
	token->type=UNTERMINAL;
	copy_yytext(token);
	yylval.token_value = token;
		
	return UNTERMINAL;
};
[A-Z_]+		{
	TOKEN* token = (TOKEN*)malloc(sizeof(TOKEN));
	memset(token, 0, sizeof(TOKEN));
	token->type=TERMINAL;
	copy_yytext(token);
	yylval.token_value = token;
	return TERMINAL;
};
":"		return COLON;
"|"		return OR;
.		{
	TOKEN* token = (TOKEN*)malloc(sizeof(TOKEN));
	memset(token, 0, sizeof(TOKEN));
	token->type=TERMINAL;
	copy_yytext(token);
	yylval.token_value = token;
	return TERMINAL;
};
%%
