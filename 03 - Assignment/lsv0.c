#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
       
extern int errno;

void do_ls(char * dir);

int main(int argc, char * argv[])
{
	if (argc != 2) {
		printf("Usage: ./a.out (directory name).\n");
		return -1;
	}
	printf("Directory listing of %s:\n", argv[1]);
	do_ls(argv[1]);
	return 0;
}

void do_ls(char * dir) {
	DIR * dp = opendir(dir);
	if (dp == NULL) {
		fprintf(stderr, "Can't open directory: %s\n", dir);
		exit(-1);
	}
	errno = 0;
	struct dirent * entry = NULL;
	while ((entry = readdir(dp)) != NULL) {
		if (entry == NULL && errno != 0) {
			fprintf(stderr, "Error while reading directory %s\n", dir);
			exit(errno);
		} else {
			printf("%s\n", entry->d_name);
		}
	}
	closedir(dp);
}
