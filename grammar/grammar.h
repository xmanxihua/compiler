#ifndef __GRAMMAR_H__
#define	__GRAMMAR_H__

#include <stdio.h>

typedef enum {
	UNTERMINAL,
	TERMINAL,
	COLON,
	OR,
	CR
} TOKEN;

typedef struct {  
	size_t len;
	u_char *data;
} str_t;

extern char* yytext;

#define ERROR_EXIT(msg) \
do { \
	peror(msg); \
	exit(1); \
}while (0);

#endif
