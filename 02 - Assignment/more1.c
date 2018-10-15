#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 512

void do_more(FILE *);

int main(int argc, char * argv[]) {
	if (argc == 1) {
		printf("Help Page...\n");
		exit(0);
	}
	int i = 0;
	while (++i < argc) {
		FILE * fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("Can't open file: %s\n", argv[i]);
			exit(-1);
		}
		do_more(fp);
		fclose(fp);
	}
	return 0;
}

void do_more(FILE * fp) {
	char buffer[LINE_LENGTH];
	while (fgets(buffer, LINE_LENGTH, fp)) {
		fputs(buffer, stdout);
	}
}
