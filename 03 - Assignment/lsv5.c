#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int errno;
static char ** entries;
static int rows = 0;
static int cols = 0;

int get_total_entries(char * dir);
int get_length_of_max_filename(char * dir);
int cmpstringp(const void *p1, const void *p2);
void print_entries();
void do_ls(char * dir);

int main(int argc, char * argv[])
{
	if (argc == 1) {
		rows = get_total_entries(".");
		cols = get_length_of_max_filename(".");
		entries = (char **) malloc(sizeof(char *) * rows);
		for (int i = 0; i < rows; i++) {
			entries[i] = (char *) malloc(sizeof(char) * cols);
		}
		do_ls(".");
		qsort(entries, rows, sizeof(char *), cmpstringp);
		print_entries();
		for (int i = 0; i < rows; i++) {
			free(entries[i]);
		}
		free(entries);
	} else {
		int j = 0;
		while (++j < argc) {
			printf("%s:\n", argv[j]);
			rows = get_total_entries(argv[j]);
			cols = get_length_of_max_filename(argv[j]);
			entries = (char **) malloc(sizeof(char *) * rows);
			for (int i = 0; i < rows; i++) {
				entries[i] = (char *) malloc(sizeof(char) * cols);
			}
			do_ls(argv[j]);
			qsort(entries, rows, sizeof(char *), cmpstringp);
			print_entries();
			for (int i = 0; i < rows; i++) {
				free(entries[i]);
			}
			free(entries);
		}
	}
	return 0;
}

void do_ls(char * dir) {
	DIR * dp = opendir(dir);
	chdir(dir);
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
			memset(entries[i], '\0', cols);
			strcpy(entries[i], entry->d_name);
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

int cmpstringp(const void *p1, const void *p2) {
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}


void print_entries() {
	for (int i = 0; i < rows; i++) {
		printf("%s\n", entries[i]);
	}
}
