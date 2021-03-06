#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "hash_map.h"

int main() {
	HASH_MAP map;
	initMap(&map);
	int i = 0;
	char alpha[] = "abcdefghijklmnopq12345678_PEZDKLFR";
	srand(time(NULL));
	for (i=0; i<20;i++){
		int len = rand()%6+1;
		char* pKey = (char*)malloc(sizeof alpha);
		memset(pKey, 0, sizeof alpha);
		int j;
		for (j=0; j<len; j++){
			int r = rand()%strlen(alpha);
			pKey[j] = alpha[r];
		}
		printf ("put (%s, %s)\n", pKey, pKey);
		put(&map, pKey, pKey);
		//printf("map.capacity = %d, map.size=%d\n", map.capacity, map.size);
	}
	
	ITERATOR* it = createIterator(&map);
	while (it->has_next(it)){
		ENTRY* e = it->next(it);
		printf("(%s, %s)\n", e->key, (char*)e->value);
	}
	return 0;
}
