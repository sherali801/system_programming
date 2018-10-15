#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 512

void do_more(FILE *);

int main(int argc, char * argv[]) {
	if (argc == 1) {
		printf("Help Page...\n");
		exit(0);
	}
	FILE * fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Can't open file: %s\n", argv[1]);
		exit(-1);
	}
	do_more(fp);
	fclose(fp);
	return 0;
}

void do_more(FILE * fp) {
	char buffer[LINE_LENGTH];
	while (fgets(buffer, LINE_LENGTH, fp)) {
		fputs(buffer, stdout);
	}
}
