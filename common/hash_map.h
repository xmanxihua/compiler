#ifndef __HASH_MAP_H__
#define	__HASH_MAP_H__
typedef struct ENTRY {
        char* key;
        void* value;
        struct ENTRY* next;
        unsigned int hash;
}ENTRY;

typedef struct HASH_MAP {
        unsigned int size;
        unsigned int capacity;
        ENTRY** table;
}HASH_MAP;

typedef struct ITERATOR {
	HASH_MAP* map;
	int (*has_next)(struct ITERATOR*);
	ENTRY* (*next)(struct ITERATOR*);
	ENTRY* nextEntry;
}ITERATOR;

HASH_MAP* createMap();

void initMap(HASH_MAP* map);

void* put(HASH_MAP* map, char* key, void* value);

void* get(HASH_MAP* map, char* key);

int isEmpty(HASH_MAP* map);

int containsKey(HASH_MAP* map, char* key);

void* removeWithKey(HASH_MAP* map, char* key);

ITERATOR* createIterator(HASH_MAP* map);

#endif
