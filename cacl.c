#include <stdio.h>
#include <stdlib.h>

#include "cacl.h"

static TOKEN curr_token = INIT_TOKEN;

extern char* yytext;

static double error() {
	printf("Error!\n");
	exit(1);
}

static void cacl_lex() {
	curr_token = yylex();
	switch (curr_token) {
	case ADD:
		printf("read +\n");
		break;
	case SUB:
		printf("read -\n");
		break;
	case MUL:
		printf("read *\n");
		break;
	case DIV:
		printf("read /\n");
		break;
	case DOUBLE_LITERAL:
		printf("read %s\n", yytext);
		break;
	case LEFT_BRACKETS:
		printf("read (\n");
		break;
	case RIGHT_BRACKETS:
		printf("read )\n");
		break;
	case CR:
		printf("read CR\n");
		break;
	}
}
static double parse_S();
static double parse_T(double r);
static double parse_P();
static double parse_D(double r);
static double parse_C(double r);
static double parse_A(double r);
static double parse_B(double r);

static double parse_S(){
	double result = 0;
	switch (curr_token) {
	case DOUBLE_LITERAL:
	case LEFT_BRACKETS:
		result = parse_T(0);
		return parse_C(result);
	case SUB:
		cacl_lex();
		result = -parse_T(0);
		return parse_C(result);
	default:
		error();
		break;
	}
	return 0;
}


static double parse_T(double r){
	double result = 0;
	switch (curr_token) {
	case DOUBLE_LITERAL:
	case LEFT_BRACKETS:
		result = parse_P();
		return parse_D(result);
	case SUB:
		cacl_lex();
		result = -parse_P();
		return parse_D(result);
	default:
		error();
		break;
	}
	return 0;
}

static double parse_P() {
	double temp = 0;
	switch (curr_token) {
	case DOUBLE_LITERAL:
		temp = yylval.double_value;
		cacl_lex();
		return temp;
	case LEFT_BRACKETS:
		cacl_lex();
		temp = parse_S();
		if (curr_token!=RIGHT_BRACKETS)
			error();
		cacl_lex();
		return temp;
	case SUB:
		cacl_lex();
		return -parse_P();
	default:
		error();
		break;
	}

	return 0;
}
static double parse_D(double r) {
	double result = 0;
	switch (curr_token) {
	case ADD:
	case SUB:
	case CR:
	case RIGHT_BRACKETS:
		return r;
	case MUL:
	case DIV:
		result = parse_B(r);
		return parse_D(result); 
	default:
		error();
		break;
	}
	return 0;
}


static double parse_C(double r) {
	double result = 0;
	switch (curr_token) {
	case ADD:
	case SUB:
		result =  parse_A(r);
		return parse_C(result);
	case CR:
	case RIGHT_BRACKETS:
		return r;
	default:
		error();
		break;
	}
	return 0;
}

static double parse_B(double r) {
	switch (curr_token) {
	case MUL:
		cacl_lex();
		return r * parse_P();
	case DIV:
		cacl_lex();
		return r / parse_P();
	default:
		error();
		break;
	}
	return 0;
}
static double parse_A(double r) {
	switch (curr_token) {
	case ADD:
		cacl_lex();
		return r + parse_T(r);
	case SUB:
		cacl_lex();
		return r - parse_T(r);
	default:
		error();
		break;
	}
	return 0;
}

static double yyparse() {
	extern FILE* yyin;
	yyin = stdin;
	cacl_lex();
	return parse_S();
}
int main() {
	printf("cacl result=%lf!\n", yyparse());
	return 0;
}
