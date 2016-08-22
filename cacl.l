%{
#include <stdio.h>

#ifdef LR1
#include "y.tab.h"
#else
#include "cacl.h"
#endif

int yywrap(void) {
	return 1;
}

%}

%%

"+"	return ADD;
"-"	return SUB;
"*"	return MUL;
"/"	return DIV;
"("	return LEFT_BRACKETS;
")"	return RIGHT_BRACKETS;
(0|([1-9][0-9]*))(\.[0-9]*)?	{
	double temp;
	sscanf(yytext, "%lf", &temp);
	yylval.double_value = temp;
	return DOUBLE_LITERAL;
}
"\n"	return CR;
[ \t]	;
. {
	fprintf (stderr, "lexical error near %s.\n", yytext);
	exit(1);
}

%%

