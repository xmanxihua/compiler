%{
#include <stdio.h>
#ifdef LR1
#include "y.tab.h"
#else
#include "grammar.h"
#endif
int yywrap(void){
	return 1;
}
%}

%%
[\n]		return CR;
[ \t]		;
[a-z_]+[0-9]*	return UNTERMINAL;
[A-Z_]+		return TERMINAL;
":"		return COLON;
"|"		return OR;
"ε"		return EMPTY_RESULT;
.		return TERMINAL;
%%
