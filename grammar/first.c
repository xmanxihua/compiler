#include "grammar.h"
#include <stdlib.h>

int main (int argc, char* argv) {
	if (argc<2){
		fprintf(stderr, "Usage: params <file path>\n");
		exit(1);
	}
	
	FILE* file = fopen(argv[1], "r");
	if (!file){
		char str [1024] = {0};
		sscanf(argv[1], "Open file %s failed!\n", str);
		ERROR_EXIT("open file %s");
	}
}

