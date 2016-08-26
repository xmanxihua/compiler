#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "linked_hash_map.h"

int main() {
	LINKED_HASH_MAP map;
	initLinkedMap(&map);
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
		if (i==10){
			printf ("put (%s, %s)\n", "BBB", "BBB");
			putLinkedMap(&map, "BBB", "BBB");
		}
		printf ("put (%s, %s)\n", pKey, pKey);
		putLinkedMap(&map, pKey, pKey);
		//printf("map.capacity = %d, map.size=%d\n", map.capacity, map.size);
	}
	putLinkedMap(&map, "AAAA", "AAAA");
	linkedMapRemoveWithKey(&map, "BBB");
	
	LINKED_ITERATOR* it = createLinkedIterator(&map);
	while (it->has_next(it)){
		LINKED_ENTRY* e = it->next(it);
		printf("(%s, %s)\n", e->key, (char*)e->value);
		
	}
	
	return 0;
}
