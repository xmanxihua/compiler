#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

#define MAP_INIT_CAPACITY	2

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

HASH_MAP* createMap(){
        HASH_MAP* map = (HASH_MAP*)malloc(sizeof (HASH_MAP));
        memset(map, 0, sizeof(HASH_MAP));
        map->capacity = MAP_INIT_CAPACITY;
        map->table = (ENTRY**)malloc(map->capacity * sizeof (ENTRY*));
        memset(map->table, 0, map->capacity * sizeof (ENTRY*));
        return map;
}

void initMap(HASH_MAP* map){
	if (!map)
		return;
        memset(map, 0, sizeof(HASH_MAP));
        map->capacity = MAP_INIT_CAPACITY;
        map->table = (ENTRY**)malloc(map->capacity * sizeof (ENTRY*));
        memset(map->table, 0, map->capacity * sizeof (ENTRY*));
}

static void resize(HASH_MAP* map, size_t capacity){
        ENTRY** oldTable = map->table;
        size_t oldCapacity = map->capacity;

        map->table = (ENTRY**)malloc(sizeof (ENTRY*) * capacity);
        memset(map->table, 0, capacity * sizeof (ENTRY*));
        map->capacity = capacity;

        //rehash
        ENTRY* entry;
        int i;
        for (i=0, entry=oldTable[i]; i<oldCapacity; entry=oldTable[++i]){
                for (;entry;entry=entry->next){
                        unsigned int index = indexFor(entry->hash, capacity);
                        ENTRY* e = map->table[index];
                        entry->next = e;
                        map->table[index] = entry;
                }
        }
        free(oldTable);
}


static void addTokenEntry(HASH_MAP* map, char* key, void* value, unsigned int hash){
        if (map->capacity<=map->size){
                resize(map, map->capacity*2);
        }
       
	ENTRY* entry = (ENTRY*)malloc(sizeof (ENTRY));

        entry->key = key;
        entry->value = value;
        entry->hash = hash;

        unsigned int index = indexFor(hash, map->capacity);
        ENTRY* e = map->table[index];
        entry->next = e;
        map->table[index] = entry;

        ++map->size;
}

void* put(HASH_MAP* map, char* key, void* value) {
        if (!map || !key || !value)
                return NULL;
        unsigned int hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        ENTRY* entry;
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

void* get(HASH_MAP* map, char* key) {
        if (!map || !map->size || !key)
                return NULL;
        unsigned hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        ENTRY* entry;
        for (entry = map->table[index];entry; entry=entry->next){
                if (entry->hash == hash && (entry->key == key || !strncmp(entry->key, key, 8192)))
                        return entry->value;
        }
        return NULL;
}

int isEmpty(HASH_MAP* map) {
	return map ? !map->size>0 : 1;
}

int containsKey(HASH_MAP* map, char* key){
	return get(map, key)!=NULL;
}

void* removeWithKey(HASH_MAP* map, char* key) {
	if (!map || !map->size || !key)
		return NULL;
	unsigned int hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        ENTRY* entry;
	ENTRY* pre;
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
