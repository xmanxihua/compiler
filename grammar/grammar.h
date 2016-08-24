#ifndef __GRAMMAR_H__
#define	__GRAMMAR_H__
struct TOKEN;
 
typedef struct TOKEN_LIST {
        struct TOKEN_LIST* next;
        struct TOKEN_LIST* pre;
        struct TOKEN* head;
        struct TOKEN* tail; 
        struct TOKEN* parent;
} TOKEN_LIST;

typedef struct TOKEN {
        struct TOKEN* next;
        struct TOKEN* pre;

        TOKEN_LIST* parent;
        TOKEN_LIST* children;
        char* value;
        int type;
}TOKEN;

#endif
