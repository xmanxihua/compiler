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

unsigned int elf_hash(char* key){
	unsigned int hash = 0;
	unsigned int x = 0;
	while (*key){
		hash = (hash<<4)+(*key++);
		if ((x = hash & 0XF0000000L)!=0){
			hash = (x>>24);
			hash &= x;
		}
	}
	return (hash & 0x7FFFFFFF);
}
unsigned int indexFor(unsigned int hashCode, size_t length){
	return hashCode & length -1;
}

typedef struct TOKEN_ENTRY {
	char* key;
	TOKEN* token;
	struct TOKEN_ENTRY* next;
	unsigned int hash;
}TOKEN_ENTRY;

typedef struct TOKEN_MAP {
	size_t size;
	size_t capacity;
	TOKEN_ENTRY** container;
}

TOKEN_MAP* initMap(){
	TOKEN_MAP* map = (TOKEN_MAP*)malloc(sizeof (TOKEN_MAP));
	memset (map, 0, sizeof(TOKEN_MAP));
	map->capacity = 16;
	map->container = (TOKEN_ENTRY**)malloc(map->capacity * sizeof (TOKEN_ENTRY*));
	memset(map->container, 0, map->capacity * sizeof (TOKEN_ENTRY*));
	return map;
}

void resize(TOKEN_MAP* map, size_t capacity){
	TOKEN_ENTRY *oldContainer = map->container;
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


void addTokenEntry(TOKEN_MAP* map, TOKEN* token, unsigned int hash){
	if (map>capacity<=map->size){
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
	if (!map || !key)
		return NULL;
	unsigned hash = elf_hash(key);
	unsigned int index = indexFor(hash, map->capacity)
	TOKEN_ENTRY* entry;
	for (entry = map->container[index];entry; entry=entry->next){
		if (entry->hash == hash && (entry->key == key || !strncmp(entry->key, key, 8192)))
			return entry->token;
	}
	return NULL;
}

#endif
