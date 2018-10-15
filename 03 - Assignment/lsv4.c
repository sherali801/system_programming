#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>
#include <strings.h>

extern int errno;
static int max_entry_length = 0;
static int cols = 0;

int get_total_entries(char * dir);
int get_length_of_max_filename(char * dir);
void do_ls(char * dir);

int main(int argc, char * argv[])
{
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	int win_cols = ws.ws_col;
	int total_entries = 0;
	if (argc == 1) {
		total_entries = get_total_entries(".");
		max_entry_length = get_length_of_max_filename(".");
		cols = (int) floor((double) win_cols / (double) max_entry_length);
		do_ls(".");
		printf("\n");
	} else {
		int i = 0;
		while (++i < argc) {
			total_entries = get_total_entries(argv[i]);
			max_entry_length = get_length_of_max_filename(argv[i]);
			cols = (int) floor((double) win_cols / (double) max_entry_length);
			do_ls(argv[i]);
			printf("\n");
		}
	}
	return 0;
}

void do_ls(char * dir) {
	DIR * dp = opendir(dir);
	if (dp == NULL) {
		fprintf(stderr, "Can't open directory: %s\n", dir);
		exit(-1);
	}
	int i = 0;
	errno = 0;
	struct dirent * entry = NULL;
	while ((entry = readdir(dp)) != NULL) {
		if (entry == NULL && errno != 0) {
			fprintf(stderr, "Error while reading directory %s\n", dir);
			exit(errno);
		} else {
			if (entry->d_name[0] == '.') {
				continue;
			}
			if (i >= cols) {
				printf("\n");
				i = 0;
			}
			printf("%-*s ", max_entry_length, entry->d_name);
			i++;
		}
	}
	closedir(dp);
}

int get_total_entries(char * dir) {
	DIR * dp = opendir(dir);
	if (dp == NULL) {
		fprintf(stderr, "Can't open directory: %s\n", dir);
		exit(-1);
	}
	int total_entries = 0;
	errno = 0;
	struct dirent * entry = NULL;
	while ((entry = readdir(dp)) != NULL) {
		if (entry == NULL && errno != 0) {
			fprintf(stderr, "Error while reading directory %s\n", dir);
			exit(errno);
		} else {
			if (entry->d_name[0] == '.') {
				continue;
			}
			total_entries++;
		}
	}
	closedir(dp);
	return total_entries;
}

int get_length_of_max_filename(char * dir) {
	DIR * dp = opendir(dir);
	if (dp == NULL) {
		fprintf(stderr, "Can't open directory: %s\n", dir);
		exit(-1);
	}
	int max_length = 0;
	errno = 0;
	struct dirent * entry = NULL;
	while ((entry = readdir(dp)) != NULL) {
		if (entry == NULL && errno != 0) {
			fprintf(stderr, "Error while reading directory %s\n", dir);
			exit(errno);
		} else {
			if (entry->d_name[0] == '.') {
				continue;
			}
			int current_length = strlen(entry->d_name);
			if (current_length > max_length) {
				max_length = current_length;
			}
		}
	}
	closedir(dp);
	return max_length;
}
