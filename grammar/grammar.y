%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#define YYDEBUG	1

extern char *yytext;

TOKEN* curr;

TOKEN_MAP map;
%}

%union {
	TOKEN* token_value;
	TOKEN_LIST* token_list_value;
}

%token COLON OR CR
%token <token_value> UNTERMINAL TERMINAL
%type  <token_value> product_left 
%type <token_list_value> product_right

%%
line_list
: line 
| line line_list
;

line
:product_left COLON product_right CR
{
	TOKEN* token = $1;
	if (containsKey(token->value)){
		TOKEN_LIST* list = token -> children;
		TOKEN_LIST* new_list = $3;
		for (;list && list->next;list=list->next);
		list-> next = new_list;
		new_list->pre = list
	}else
		put(&map, token);
	char product[8192]={0};
	char* p = (char*) product;
	int len = strlen(token->value);
	strncpy(p, token->value, len+1);
	p+=len;
	*(p++)=':';
	TOKEN_LIST* list;
	for (list = token->children;list;list = list->next){
		TOKEN* t;
		for (t=list->head; t; t = t->next){
			len = strlen(t->value);
			strncpy(p, t->value, len+1);
			p+=len;
		}
		if (list->next)
			*(p++) = '|';
	}
	printf("product = %s\n", product);
}
;
product_left
:UNTERMINAL
{
	TOKEN* token = $1;
	token -> children = (TOKEN_LIST*)malloc(sizeof (TOKEN_LIST));
	
	curr=token;
	$$ = token;
}
;
product_right
:TERMINAL
{
	TOKEN* token = $1;
	TOKEN_LIST* token_list = curr->children;
	
	token_list->head = token;
	token_list->tail = token;
	token_list->parent = curr;
	token->parent = token_list;

	$$ = token_list;
}
|UNTERMINAL
{
	TOKEN* token = $1;
	TOKEN_LIST* token_list = curr->children;
	
	token_list->head = token;
	token_list->tail = token;
	token_list->parent = curr;
	token->parent = token_list;
	
	$$ = token_list;
}
|product_right TERMINAL 
{
	TOKEN_LIST* list = $1;
	TOKEN* token = $2;
	if (list->tail!= list->head && list->tail){
		list->tail->next = token;
		token->pre = list->tail;
	} else if (list->tail){
		list-> head->next = token;
		token->pre = list->head;
	}
	list->tail = token;	
	list->parent = curr;
	token->parent = list;
	$$ = list;
}
|product_right UNTERMINAL 
{
	TOKEN_LIST* list = $1;
	TOKEN* token = $2;
	if (list->tail!= list->head && list->tail){
		list->tail->next = token;
		token->pre = list->tail;
	} else if (list->tail){
		list-> head->next = token;
		token->pre = list->head;
	}
	list->tail = token;	
	list->parent = curr;
	token->parent = list;
	$$ = list;
}
|product_right OR TERMINAL 
{
	TOKEN_LIST* list = $1;
	TOKEN_LIST* tail = list; 
	
	while (tail->next){
		tail=tail->next;
	}
	
	TOKEN_LIST* token_list = (TOKEN_LIST*)malloc(sizeof (TOKEN_LIST));
	tail->next = token_list;
	token_list->pre = tail;
	
	TOKEN* token = $3;
	token_list->head = token;
	token_list->tail = token;
	token_list->parent = curr;
	token->parent = token_list;
	$$ = token_list;
}
|product_right OR UNTERMINAL 
{
	TOKEN_LIST* list = $1;
	TOKEN_LIST* tail = list; 
	
	while (tail->next){
		tail=tail->next;
	}
	
	TOKEN_LIST* token_list = (TOKEN_LIST*)malloc(sizeof (TOKEN_LIST));
	tail->next = token_list;
	token_list->pre = tail;
	
	TOKEN* token = $3;
	token_list->head = token;
	token_list->tail = token;
	token_list->parent = curr;
	token->parent = token_list;
	$$ = token_list;
}
;

%%

void reduceLeftRecurse(){
}

int yyerror(char const* str){
	extern char* yytext;
	fprintf(stderr, "parse error!\n");
	return 0;
}

int main(void){
	initMap(&map);
	extern int yyparse(void);
	extern FILE* yyin;
	
	yyin = stdin;
	if (yyparse()){
		fprintf(stderr, "Error!\n");
		exit(1);
	}
	
	return 0;
}
