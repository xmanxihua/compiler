#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"

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

TOKEN_MAP* initMap(){
        TOKEN_MAP* map = (TOKEN_MAP*)malloc(sizeof (TOKEN_MAP));
        memset(map, 0, sizeof(TOKEN_MAP));
        map->capacity = MAP_INIT_CAPACITY;
        map->container = (TOKEN_ENTRY**)malloc(map->capacity * sizeof (TOKEN_ENTRY*));
        memset(map->container, 0, map->capacity * sizeof (TOKEN_ENTRY*));
        return map;
}

static void resize(TOKEN_MAP* map, size_t capacity){
        TOKEN_ENTRY** oldContainer = map->container;
        size_t oldCapacity = map->capacity;

        map->container = (TOKEN_ENTRY**)malloc(sizeof (TOKEN_ENTRY*) * capacity);
        memset(map->container, 0, capacity * sizeof (TOKEN_ENTRY*));
        map->capacity = capacity;

        //rehash
        TOKEN_ENTRY* entry;
        int i;
        for (i=0, entry=oldContainer[i]; i<oldCapacity; entry=oldContainer[++i]){
                for (;entry;entry=entry->next){
                        unsigned int index = indexFor(entry->hash, capacity);
                        TOKEN_ENTRY* e = map->container[index];
                        entry->next = e;
                        map->container[index] = entry;
                }
        }
        free(oldContainer);
}


static void addTokenEntry(TOKEN_MAP* map, TOKEN* token, unsigned int hash){
        if (map->capacity<=map->size){
                resize(map, map->capacity*2);
        }
       
	TOKEN_ENTRY* entry = (TOKEN_ENTRY*)malloc(sizeof (TOKEN_ENTRY));

        entry->key = token->value;
        entry->token = token;
        entry->hash = hash;

        unsigned int index = indexFor(hash, map->capacity);
        TOKEN_ENTRY* e = map->container[index];
        entry->next = e;
        map->container[index] = entry;

        ++map->size;
}

TOKEN* put(TOKEN_MAP* map, TOKEN* token) {
        if (!map || !token || !token->value)
                return NULL;
        unsigned int hash = elf_hash(token->value);
        unsigned int index = indexFor(hash, map->capacity);
        TOKEN_ENTRY* entry;
        for (entry=map->container[index];entry; entry = entry->next){
                if (entry->hash == hash && (entry->key==token->value || !strncmp(entry->key, token->value, 8192))){
                        TOKEN* old=entry->token;
                        entry->token = token;
                        return old;
                }
        }
        addTokenEntry(map, token, hash);
        return NULL;
}

TOKEN* get(TOKEN_MAP* map, char* key) {
        if (!map || !map->size || !key)
                return NULL;
        unsigned hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        TOKEN_ENTRY* entry;
        for (entry = map->container[index];entry; entry=entry->next){
                if (entry->hash == hash && (entry->key == key || !strncmp(entry->key, key, 8192)))
                        return entry->token;
        }
        return NULL;
}

int isEmpty(TOKEN_MAP* map) {
	return map ? !map->size>0 : 1;
}

int containsKey(TOKEN_MAP* map, char* key){
	return get(map, key)!=NULL;
}

TOKEN* removeWithKey(TOKEN_MAP* map, char* key) {
	if (!map || !map->size || !key)
		return NULL;
	unsigned int hash = elf_hash(key);
        unsigned int index = indexFor(hash, map->capacity);
        TOKEN_ENTRY* entry;
	TOKEN_ENTRY* pre;
        for (entry=map->container[index], pre=entry;entry; pre = entry, entry = entry->next){
                if (entry->hash == hash && (entry->key==key || !strncmp(entry->key, key, 8192))){
			if (pre!=entry)
				pre->next = entry->next;
			else
				map->container[index] = entry->next;
			--map->size;
                        TOKEN* old=entry->token;
			free(entry);
                        return old;
                }
        }
	return NULL;
}
