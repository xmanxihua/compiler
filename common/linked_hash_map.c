#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_hash_map.h"

#define MAP_INIT_CAPACITY	2

static LINKED_ENTRY* HEADER;

static unsigned int elf_hash(char* key){
        unsigned int hash = 0;
        unsigned int x = 0;
        while (*key){
                hash = (hash<<4)+(*key++);
                if ((x = hash & 0XF0000000L)){
                        hash = (x>>24);
                        hash &= x;
                }
        }
        return (hash & 0x7FFFFFFF);
}

static unsigned int indexFor(unsigned int hashCode, size_t length){
        return hashCode & (length -1);
}

LINKED_HASH_MAP* createMap(){
        LINKED_HASH_MAP* map = (LINKED_HASH_MAP*)malloc(sizeof (LINKED_HASH_MAP));
        memset(map, 0, sizeof(LINKED_HASH_MAP));
        map->capacity = MAP_INIT_CAPACITY;
        map->table = (LINKED_ENTRY**)malloc(map->capacity * sizeof (LINKED_ENTRY*));
        memset(map->table, 0, map->capacity * sizeof (LINKED_ENTRY*));
	HEADER = (LINKED_ENTRY*) malloc(sizeof (LINKED_ENTRY));
	memset(HEADER, 0, sizeof(LINKED_ENTRY));
	HEADER->before = HEADER->after = HEADER;
	HEADER->hash = -1;
	map->header = HEADER;
        return map;
}

void initMap(LINKED_HASH_MAP* map){
	if (!map)
		return;
        memset(map, 0, sizeof(LINKED_HASH_MAP));
        map->capacity = MAP_INIT_CAPACITY;
        map->table = (LINKED_ENTRY**)malloc(map->capacity * sizeof (LINKED_ENTRY*));
        memset(map->table, 0, map->capacity * sizeof (LINKED_ENTRY*));
}

static void resize(LINKED_HASH_MAP* map, size_t capacity){
        LINKED_ENTRY** oldTable = map->table;
        size_t oldCapacity = map->capacity;

        map->table = (LINKED_ENTRY**)malloc(sizeof (LINKED_ENTRY*) * capacity);
        memset(map->table, 0, capacity * sizeof (LINKED_ENTRY*));
        map->capacity = capacity;

        //rehash
        LINKED_ENTRY* entry;
        int i;
        for (i=0, entry=oldTable[i]; i<oldCapacity; entry=oldTable[++i]){
                for (;entry;entry=entry->next){
                        unsigned int index = indexFor(entry->hash, capacity);
                        LINKED_ENTRY* e = map->table[index];
                        entry->next = e;
                        map->table[index] = entry;
                }
        }
        free(oldTable);
}


static void addTokenEntry(LINKED_HASH_MAP* map, char* key, void* value, unsigned int hash){
        if (map->capacity<=map->size){
                resize(map, map->capacity*2);
        }
       
	LINKED_ENTRY* entry = (LINKED_ENTRY*)malloc(sizeof (LINKED_ENTRY));

        entry->key = key;
        entry->value = value;
        entry->hash = hash;

        unsigned int index = indexFor(hash, map->capacity);
        LINKED_ENTRY* e = map->table[index];
        entry->next = e;
        map->table[index] = entry;

        ++map->size;

	//add entry to  double linked list
	LINKED_ENTRY* h = map->header;
	h->before = 
}

void* put(LINKED_HASH_MAP* map, char* key, void* value) {
        if (!map || !key || !value)
                return NULL;
        unsigned int hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        LINKED_ENTRY* entry;
        for (entry=map->table[index];entry; entry = entry->next){
                if (entry->hash == hash && (entry->key==key || !strncmp(entry->key, key, 8192))){
                        void* old=entry->value;
                        entry->value = value;
                        return old;
                }
        }
	
        addTokenEntry(map, key, value, hash);
        return NULL;
}

void* get(LINKED_HASH_MAP* map, char* key) {
        if (!map || !map->size || !key)
                return NULL;
        unsigned hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        LINKED_ENTRY* entry;
        for (entry = map->table[index];entry; entry=entry->next){
                if (entry->hash == hash && (entry->key == key || !strncmp(entry->key, key, 8192)))
                        return entry->value;
        }
        return NULL;
}

int isEmpty(LINKED_HASH_MAP* map) {
	return map ? !map->size>0 : 1;
}

int containsKey(LINKED_HASH_MAP* map, char* key){
	return get(map, key)!=NULL;
}

void* removeWithKey(LINKED_HASH_MAP* map, char* key) {
	if (!map || !map->size || !key)
		return NULL;
	unsigned int hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        LINKED_ENTRY* entry;
	LINKED_ENTRY* pre;
        for (entry=map->table[index], pre=entry;entry; pre = entry, entry = entry->next){
                if (entry->hash == hash && (entry->key==key || !strncmp(entry->key, key, 8192))){
			if (pre!=entry)
				pre->next = entry->next;
			else
				map->table[index] = entry->next;
			--map->size;
                        void* old=entry->value;
			free(entry);
                        return old;
                }
        }
	return NULL;
}

static LINKED_ENTRY* nextEntry(LINKED_ITERATOR* itr){
	LINKED_ENTRY* e = itr->nextEntry;
	if (e && !(itr->nextEntry = e->next)){
		unsigned int index = indexFor(e->hash, itr->map->capacity);
		if (index < itr->map->capacity - 1)
			for (itr->nextEntry=itr->map->table[++index];index<itr->map->capacity && !itr->nextEntry;itr->nextEntry=itr->map->table[++index]);
	}
	return e;
}

static int hasNext(LINKED_ITERATOR* itr) {
	return itr->nextEntry!=NULL;
}

LINKED_ITERATOR* createLinkedIterator(LINKED_HASH_MAP* map) {
	if (!map)
		return NULL;

	LINKED_ITERATOR* itr = (LINKED_ITERATOR*)malloc(sizeof(LINKED_ITERATOR));
	itr->map = map;
	itr->next=nextEntry;
	itr->has_next=hasNext;

	int i;
	for (i=0, itr->nextEntry=map->table[i]; i<map->capacity && !itr->nextEntry; itr->nextEntry=map->table[++i]);
	return itr;
}
