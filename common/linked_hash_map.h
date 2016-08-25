#ifndef __LINKED_HASH_MAP_H__
#define __LINKED_HASH_MAP_H__

typedef struct LINKED_ENTRY {
        char* key;
        void* value;
        struct LINKED_ENTRY* next;
        unsigned int hash;
	struct LINKED_ENTRY* before, *after;
}LINKED_ENTRY;

typedef struct LINKED_HASH_MAP {
        unsigned int size;
        unsigned int capacity;
        LINKED_ENTRY** table;
	LINKED_ENTRY* header;
}LINKED_HASH_MAP;

typedef struct LINKED_ITERATOR {
        LINKED_HASH_MAP* map;
        int (*has_next)(struct LINKED_ITERATOR*);
        ENTRY* (*next)(struct LINKED_ITERATOR*);
        ENTRY* nextEntry;
}LINKED_ITERATOR;

LINKED_HASH_MAP* createMap();

void initMap(LINKED_HASH_MAP* map);

void* put(LINKED_HASH_MAP* map, char* key, void* value);

void* get(LINKED_HASH_MAP* map, char* key);

int isEmpty(LINKED_HASH_MAP* map);

int containsKey(LINKED_HASH_MAP* map, char* key);

void* removeWithKey(LINKED_HASH_MAP* map, char* key);

LINKED_ITERATOR* createLinkedIterator(LINKED_HASH_MAP* map);

#endif
