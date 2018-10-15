#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <strings.h>
#include <string.h>

#define MAX_LENGTH 512
#define MAX_ARGUMENTS 10
#define ARGUMENT_LENGTH 30

int execute(char * argument_list[]);
char ** tokenize(char * cmdline);
char * read_cmd(FILE *);

int main()
{
	char * cmdline = NULL;
	char ** argument_list = NULL;
	while ((cmdline = read_cmd(stdin)) != NULL) {
		if ((argument_list = tokenize(cmdline)) != NULL) {
			execute(argument_list);
			for (int i = 0; i < MAX_ARGUMENTS + 1; i++) {
				free(argument_list[i]);
			}
			free(argument_list);
			free(cmdline);
		}
	}
	printf("\n");
	return 0;
}

int execute(char * argument_list[]) {
	int status = 0;
	int cpid = fork();
	switch (cpid) {
		case -1:
			fprintf(stderr, "Error: Fork failed.\n");
			exit(-1);
		case 0:
			execvp(argument_list[0], argument_list);
			fprintf(stderr, "Error: Command not found...\n");
			exit(-1);
		default:
			waitpid(cpid, &status, 0);
			printf("Child exited with status: %d\n", status >> 8);
			return 0;
	}
}

char ** tokenize(char * cmdline) {
	char ** argument_list = (char **) malloc(sizeof(char *) * (MAX_ARGUMENTS + 1));
	for (int i = 0; i < MAX_ARGUMENTS + 1; i++) {
		argument_list[i] = (char *) malloc(sizeof(char) * ARGUMENT_LENGTH);
		bzero(argument_list[i], ARGUMENT_LENGTH);
	}
	if (cmdline[0] == '\0') {
		return NULL;
	}
	int argument_number = 0;
	char * current_position = cmdline;
	char * start = NULL;
	int length = 0;
	while (*current_position != '\0') {
		while (*current_position == ' ' || *current_position == '\t') {
			current_position++;
		}
		start = current_position;
		length = 1;
		while (*++current_position != '\0' && !(*current_position == ' ' || *current_position == '\t')) {
			length++;
		}
		strncpy(argument_list[argument_number], start, length);
		argument_list[argument_number][length] = '\0';
		argument_number++;
	}
	argument_list[argument_number] = NULL;
	return argument_list;
}

char * read_cmd(FILE * fp) {
	printf("\033[1;34m%s\033[0m$ ", getcwd(NULL, MAX_LENGTH));
	int c = 0;
	int position = 0;
	char * cmdline = (char *) malloc(sizeof(char) * MAX_LENGTH);
	while ((c = getc(fp)) != EOF) {
		if (c == '\n') {
			break;
		}
		cmdline[position++] = c;
	}
	if (c == EOF && position == 0) {
		return NULL;
	}
	cmdline[position] = '\0';
	return cmdline;
}
