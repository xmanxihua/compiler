#ifndef _CACL_H_
#define _CACL_H_

typedef enum {
	INIT_TOKEN = -1,
	ADD,
	SUB,
	MUL,
	DIV,
	DOUBLE_LITERAL,
	LEFT_BRACKETS,
	RIGHT_BRACKETS,
	NEGATIVE,
	CR,
} TOKEN;

typedef union {
	double	double_value;
} value;

value yylval;

#endif /*_CACL_H_*/
