#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LINE_LENGTH 512

static int page_length = 0;
static int total_lines = 0;
static int lines_read = 0;

void do_more(FILE *);
int get_input(FILE *);
void get_total_lines(FILE *);

int main(int argc, char * argv[]) {	
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	page_length = w.ws_row;
	if (argc == 1) {
		get_total_lines(stdin);
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
	get_total_lines(fp);
	int total_lines_read = 0;
	char buffer[LINE_LENGTH];
	FILE * fp_tty = fopen("/dev/tty", "r");
	while (fgets(buffer, LINE_LENGTH, fp)) {
		total_lines_read++;
		lines_read++;
		if (total_lines_read == page_length) {
			int input = get_input(fp_tty);
			if (input == 0) {
				printf("\033[1K \033[1G");
				exit(0);
			} else if (input == 1) {
				total_lines_read -= page_length;
				printf("\033[1K \033[1G");
			} else if (input == 2) {
				total_lines_read--;
				printf("\033[1A \033[2K \033[1G");
			} else {
				printf("\033[1K \033[1G");
				exit(-1);
			}
		}
		fputs(buffer, stdout);
	}
	int total_lines = 0;
	int lines_read = 0;
}

int get_input(FILE * fp_tty) {
	int percentage_read = (lines_read * 100) / total_lines;
	printf("\033[7m --more-- (%d%)\033[m", percentage_read);
	char input = '\0';
	system("stty -icanon");
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

void get_total_lines(FILE * fp) {
	char buffer[LINE_LENGTH];
	while (fgets(buffer, LINE_LENGTH, fp)) {
		total_lines++;
	}
	rewind(fp);	
}
