#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
       
extern int errno;

void do_ls(char * dir);
void long_ls(char * file_name);
char * get_user_name(int uid);
char * get_group_name(int gid);
char get_file_type(int mode);
char * get_file_permissions(int mode);
int get_blocks(char * file_name);
int get_total_blocks(char * dir);
void print_colorful_name(char * filename, int mode);

int main(int argc, char * argv[])
{
	if (argc == 1) {
		printf("total %d\n", get_total_blocks("."));
		do_ls(".");
	} else {
		int i = 0;
		while (++i < argc) {
			printf("total %d\n", get_total_blocks(argv[i]));
			printf("%s:\n", argv[i]); 
			do_ls(argv[i]);
			printf("\n");
		}
	}
	return 0;
}

void do_ls(char * dir) {
	DIR * dp = opendir(dir);
	chdir(dir);
	if (dp == NULL) {
		fprintf(stderr, "Can't open directory: %s, (%d).\n", dir, errno);
		exit(-1);
	}
	errno = 0;
	struct dirent * entry = NULL;
	while ((entry = readdir(dp)) != NULL) {
		if (entry == NULL && errno != 0) {
			fprintf(stderr, "Error while reading directory %s, (%d).\n", dir, errno);
			exit(errno);
		} else {
			if (entry->d_name[0] == '.') {
				continue;
			}
			long_ls(entry->d_name);
		}
	}
	closedir(dp);
}

void long_ls(char * file_name) {
	struct stat info;
	if (lstat(file_name, &info) == -1) {
		fprintf(stderr, "Error while lstat of %s, (%d).\n", file_name, errno);
		exit(-1);
	}
	printf("%s ", get_file_permissions(info.st_mode));
	printf("%3ld ", info.st_nlink);
	printf("%9s ", get_user_name(info.st_uid));
	printf("%9s ", get_group_name(info.st_gid));
	printf("%9ld ", info.st_size);
	char * m_time = ctime(&info.st_mtime);
	m_time[strlen(m_time) - 1] = '\0';
	printf("%s ", m_time);
	print_colorful_name(file_name, info.st_mode);
}

char * get_user_name(int uid) {
	errno = 0;
	struct passwd * pwd = getpwuid(uid);
	if (pwd == NULL) {
		if (errno == 0) {
			fprintf(stderr, "No record was found in /etc/passwd, (%d).\n", errno);
			exit(-1);
		} else {
			fprintf(stderr, "Error while getpwuid, (%d).\n", errno);
			exit(-1);
		}
	} else {
		return pwd->pw_name;
	}
}

char * get_group_name(int gid) {
	errno = 0;
	struct group * grp = getgrgid(gid);
	if (grp == NULL) {
		if (errno == 0) {
			fprintf(stderr, "No record was found in /etc/group, (%d).\n", errno);
			exit(-1);
		} else {
			fprintf(stderr, "Error while getgrgid, (%d)\n", errno);
			exit(-1);
		}
	} else {
		return grp->gr_name;
	}
}

char get_file_type(int mode) {
	if ((mode & 0170000) == 0010000) {
		return 'p';
	} else if ((mode & 0170000) == 0020000) {
		return 'c';
	} else if ((mode & 0170000) == 0040000) {
		return 'd';
	} else if ((mode & 0170000) == 0060000) {
		return 'b';
	} else if ((mode & 0170000) == 0100000) {
		return '-';
	} else if ((mode & 0170000) == 0120000) {
		return 'l';
	} else if ((mode & 0170000) == 0140000) {
		return 's';
	} else {
		return 'u';
	}
}

char * get_file_permissions(int mode) {
	char * str = (char *) malloc(10);
	strcpy(str, "----------");
	str[0] = get_file_type(mode);
	if ((mode & 0000400) == 0000400) {
		str[1] = 'r';
	}
	if ((mode & 0000200) == 0000200) {
		str[2] = 'w';
	}
	if ((mode & 0000100) == 0000100) {
		str[3] = 'x';
	}
	
	if ((mode & 0000040) == 0000040) {
		str[4] = 'r';
	}
	if ((mode & 0000020) == 0000020) {
		str[5] = 'w';
	}
	if ((mode & 0000010) == 0000010) {
		str[6] = 'x';
	}
	
	if ((mode & 0000004) == 0000004) {
		str[7] = 'r';
	}
	if ((mode & 0000002) == 0000002) {
		str[8] = 'w';
	}
	if ((mode & 0000001) == 0000001) {
		str[9] = 'x';
	}
	
	if ((mode & 0004000) == 0004000) {
		if (str[3] = 'r') {
			str[3] = 's';
		} else {
			str[3] = 'S';
		}
	}
	if ((mode & 0002000) == 0002000) {
		if (str[6] = 'r') {
			str[6] = 's';
		} else {
			str[6] = 'S';
		}
	}
	if ((mode & 0001000) == 0001000) {
		if (str[9] = 'r') {
			str[9] = 't';
		} else {
			str[9] = 'T';
		}
	}
	return str;
}

int get_blocks(char * file_name) {
	struct stat info;
	if (lstat(file_name, &info) == -1) {
		fprintf(stderr, "Error while lstat of %s, (%d).\n", file_name, errno);
		exit(-1);
	}
	return info.st_blocks;
}

int get_total_blocks(char * dir) {
	DIR * dp = opendir(dir);
	chdir(dir);
	if (dp == NULL) {
		fprintf(stderr, "Can't open directory: %s, (%d).\n", dir, errno);
		exit(-1);
	}
	errno = 0;
	int total_blocks = 0;
	struct dirent * entry = NULL;
	while ((entry = readdir(dp)) != NULL) {
		if (entry == NULL && errno != 0) {
			fprintf(stderr, "Error while reading directory %s, (%d).\n", dir, errno);
			exit(errno);
		} else {
			if (entry->d_name[0] == '.') {
				continue;
			}
			total_blocks += get_blocks(entry->d_name);
		}
	}
	closedir(dp);
	return total_blocks / 2;
}

void print_colorful_name(char * filename, int mode) {
	char file_type = get_file_type(mode);
	if (file_type == 'd') {
		printf("\033[1;34m%s\033[0m", filename);
	} else if (file_type == 'l') {
		printf("\033[1;36m%s\033[0m", filename);
	} else if (file_type == 'p') {
		printf("\033[40m\033[0;33m%s\033[0m", filename);
	} else if (file_type == 'b') {
		printf("\033[40m\033[1;33m%s\033[0m", filename);
	} else if (file_type == 'c') {
		printf("\033[40m\033[1;33m%s\033[0m", filename);
	}else if (file_type == 's') {
		printf("\033[40m\033[1;35m%s\033[0m", filename);
	} else {
		if ((strstr(filename, "tar")) != NULL) {
			printf("\033[1;31m%s\033[0m", filename);
		} else if ((mode & 0000100) == 0000100) {
			printf("\033[1;32m%s\033[0m", filename);
		} else {
			printf("%s", filename);
		}
	}
	printf("\n");
}
