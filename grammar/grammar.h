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

typedef struct TOKEN_ENTRY {
	char* key;
	TOKEN* token;
	struct TOKEN_ENTRY* next;
	unsigned int hash;
}TOKEN_ENTRY;

typedef struct TOKEN_MAP {
	unsigned int size;
	unsigned int capacity;
	TOKEN_ENTRY** container;
}TOKEN_MAP;

TOKEN_MAP* createMap();

void initMap(TOKEN_MAP* map);

TOKEN* put(TOKEN_MAP* map, TOKEN* token);

TOKEN* get(TOKEN_MAP* map, char* key);

int isEmpty(TOKEN_MAP* map);

int containsKey(TOKEN_MAP* map, char* key);

TOKEN* removeWithKey(TOKEN_MAP* map, char* key);
#endif
