#include <stdio.h>

#include "grammar.h"

int main() {
	TOKEN_MAP* map = initMap();
	printf ("isEmpty(map):   %s\n", isEmpty(map)?"true":"false");
	TOKEN token1;
	token1.value = "a";
	put(map, &token1);
	
	TOKEN token2;
	token2.value = "b";
	put(map, &token2);

	printf("map->size=%d, map->capacity=%d\n", map->size, map->capacity);
	
	TOKEN token3;
	token3.value = "c";
	put(map, &token3);
	printf("map->size=%d, map->capacity=%d\n", map->size, map->capacity);

	TOKEN token4;
	token4.value = "d";
	put(map, &token4);
	printf("map->size=%d, map->capacity=%d\n", map->size, map->capacity);

	TOKEN token5;
	token5.value = "e";
	put(map, &token5);
	printf("map->size=%d, map->capacity=%d\n", map->size, map->capacity);

	TOKEN token6;
	token6.value = "f";
	put(map, &token6);
	printf("map->size=%d, map->capacity=%d\n", map->size, map->capacity);

	TOKEN token7;
	token7.value = "g";
	put(map, &token7);
	printf("map->size=%d, map->capacity=%d\n", map->size, map->capacity);
		
	printf("get(map, \"a\")->value=%s\n", get(map, "a")->value);

	printf("remove d\n");	
	TOKEN* token = removeWithKey(map, "d");
	printf("remove d %s\n", token?"true":"false");
	printf("map->size==%d\n", map->size);
	printf("get(map, \"d\")->value=%s\n", get(map, "d")?get(map, "d")->value:"NULL" );
	
	printf("contains(\"c\"): %s\n", containsKey(map, "c")?"true": "false");
	return 0;
}
