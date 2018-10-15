#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 512
#define PAGE_LENGTH 20

void do_more(FILE *);
int get_input();

int main(int argc, char * argv[]) {
	if (argc == 1) {
		do_more(stdin);
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
	int total_lines = 0;
	char buffer[LINE_LENGTH];
	FILE * fp_tty = fopen("/dev/tty", "r");
	while (fgets(buffer, LINE_LENGTH, fp)) {
		total_lines++;
		if (total_lines == PAGE_LENGTH) {
			int input = get_input(fp_tty);
			printf("\033[1A \033[2K \033[1G");
			if (input == 0) {
				exit(0);
			} else if (input == 1) {
				total_lines -= PAGE_LENGTH;
			} else if (input == 2) {
				total_lines--;
			} else {
				exit(-1);
			}
		}
		fputs(buffer, stdout);
	}
}

int get_input(FILE * fp_tty) {
	printf("\033[7m --more-- (%)\033[m");
	char input = '\0';
	input = getc(fp_tty);
	if (input == 'q') {
		return 0;
	} else if (input == ' ') {
		return 1;
	} else if (input == '\n') {
		return 2;
	} else {
		return 3;
	}
}
