%{
#include <stdio.h>
#include <stdlib.h>

#define YYDEBUG	1

extern char *yytext;

#define MAX_YYTEXT_LENGTH	8192

TOKEN* curr;
%}

%union {
	TOKEN*  token_value;
	TOKEN* token_list_value;
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
	
}
;
product_left
:UNTERMINAL
{
	TOKEN* token = $1;
	token -> children = (TOKEN_LIST*)malloc(sizeof TOKEN_LIST);
	$$ = token;
	curr=token;
}
;
product_right
:TERMINAL
{
	TOKEN* token = $1;
	TOKEN_LIST* token_list = curr->children;
	
	token_list->head = token;
	token_list->tail = token;

	$$ = token_list;
}
|UNTERMINAL
{
	TOKEN* token = $1;
	TOKEN_LIST* token_list = curr->children;
	token_list->head = token;
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
	$$ = list;
}
|product_right OR TERMINAL 
{
	TOKEN_LIST* list = $1;
	TOKEN_LIST* tail = list; 
	
	while (tail->next){
		tail=tail->next;
	}
	
	TOKEN_LIST* token_list = (TOKEN_LIST*)malloc(sizeof TOKEN_LIST);
	tail->next = token_list;
	token_list->pre = tail;
	
	TOKEN* token = $3;
	token_list->head = token;
	token_list->tail = token;
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
	
	TOKEN_LIST* token_list = (TOKEN_LIST*)malloc(sizeof TOKEN_LIST);
	tail->next = token_list;
	token_list->pre = tail;
	
	TOKEN* token = $3;
	token_list->head = token;
	token_list->tail = token;
	token->parent = token_list;
	$$ = token_list;
}
;

%%
typedef struct TOKEN TOKEN;

typedef struct TOKEN_LIST {
	struct TOKEN_LIST* next;
	struct TOKEN_LIST* pre;
	TOKEN*	head;
	TOKEN* tail;
	TOKEN* parent;
} TOKEN_LIST;

typedef struct TOKEN {
	TOKEN* next;
	TOKEN* pre;
	
	TOKEN_LIST* parent;
	TOKEN_LIST* children;
	TOKEN_LIST* children_tail;
	char* value;
	int type;
} TOKEN;

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
