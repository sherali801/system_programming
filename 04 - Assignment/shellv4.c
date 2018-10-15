#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LENGTH 512
#define MAX_ARGUMENTS 10
#define ARGUMENT_LENGTH 30

int execute(char * argument_list[]);
char ** tokenize(char * cmdline);
char * read_cmd(FILE * fp);
int is_internal_cmd(char * cmd);
void execute_internal_cmd(char * argument_list[]);
void change_directory(char * directory_name);
void help(char * cmd);
void input_redirection(char * argument_list[]);
void output_redirection(char * argument_list[]);
char ** split_by_delimeter(char * cmdline, char delimeter);
int count_pipes(char * cmdline);
int execute_pipes(char ** cmd_list, int count_of_pipes);

int main()
{
	char * cmdline = NULL;
	char ** argument_list = NULL;
	char ** cmd_list = NULL;
	int status = 0;
	while ((cmdline = read_cmd(stdin)) != NULL) {
		cmd_list = split_by_delimeter(cmdline, ';');
		int cmd_number = 0;
		while (cmd_list[cmd_number] != NULL) {
			int count_of_pipes = count_pipes(cmd_list[cmd_number]);
			if (count_of_pipes > 0) {
				execute_pipes(split_by_delimeter(cmd_list[cmd_number], '|'), count_of_pipes);
			} else {
				if ((argument_list = tokenize(cmd_list[cmd_number])) != NULL) {
					if (is_internal_cmd(argument_list[0]) == 1) {
						execute_internal_cmd(argument_list);
					} else {
						execute(argument_list);
					}
					for (int i = 0; i < MAX_ARGUMENTS + 1; i++) {
						free(argument_list[i]);
					}
					free(argument_list);
				}
			}
			cmd_number++;
		}
		for (int i = 0; i < MAX_ARGUMENTS + 1; i++) {
			free(cmd_list[i]);
		}
		free(cmd_list);
		free(cmdline);
	}
	printf("\n");
	return 0;
}

int execute(char * argument_list[]) {
	int status = 0;
	int cpid = fork();
	int i = 0;
	switch (cpid) {
		case -1:
			fprintf(stderr, "Error: Fork failed.\n");
			exit(-1);
		case 0:
			input_redirection(argument_list);
			output_redirection(argument_list);
			execvp(argument_list[0], argument_list);
			fprintf(stderr, "Error: Command not found...\n");
			exit(-1);
		default:
			waitpid(cpid, &status, 0);
			printf("Child exited with status: %d\n", status >> 8);
			return 0;
	}
}

int execute_pipes(char ** cmd_list, int count_of_pipes) {
	char ** argument_list = NULL;
	if (count_of_pipes == 1) {
		int fd[2];
		pipe(fd);
		int i;
		for (i = 0; i < 2; i ++) {
			if (fork() == 0) {
				break;
			}
		}
		if (i == 0) {
			dup2(fd[1], 1);
			close(fd[0]);
			argument_list = tokenize(cmd_list[0]);
			execvp(argument_list[0], argument_list);
		}
		if (i == 1) {
			dup2(fd[0], 0);
			close(fd[1]);
			argument_list = tokenize(cmd_list[1]);
			execvp(argument_list[0], argument_list);
		}
		close(fd[0]);
		close(fd[1]);
		for(int i = 0; i < 2; i++) {
			wait(NULL);
		}
	} else if (count_of_pipes == 2) {
		int fd1[2], fd2[2];
		pipe(fd1);
		pipe(fd2);
		int i;
		for (i = 0; i < 3; i ++) {
			if (fork() == 0) {
				break;
			}
		}
		if (i == 0) {
			dup2(fd1[1], 1);
			close(fd1[0]);
			close(fd2[0]);
			close(fd2[1]);
			argument_list = tokenize(cmd_list[0]);
			execvp(argument_list[0], argument_list);
		}
		if (i == 1) {
			dup2(fd1[0], 0);
			dup2(fd2[1], 1);
			close(fd1[1]);
			close(fd2[0]);
			argument_list = tokenize(cmd_list[1]);
			execvp(argument_list[0], argument_list);
		}
		if (i == 2) {
			dup2(fd2[0], 0);
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[1]);
			argument_list = tokenize(cmd_list[2]);
			execvp(argument_list[0], argument_list);
		}
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		for(i = 0; i < 3; i++) {
			wait(NULL);
		}
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

char ** split_by_delimeter(char * cmdline, char delimeter) {
	char ** cmd_list = (char **) malloc(sizeof(char *) * (MAX_ARGUMENTS + 1));
	for (int i = 0; i < MAX_ARGUMENTS + 1; i++) {
		cmd_list[i] = (char *) malloc(sizeof(char) * ARGUMENT_LENGTH);
		bzero(cmd_list[i], ARGUMENT_LENGTH);
	}
	if (cmdline[0] == '\0') {
		return NULL;
	}
	int cmd_number = 0;
	char * current_position = cmdline;
	char * start = NULL;
	int length = 0;
	while (*current_position != '\0') {
		start = current_position;
		length = 0;
		while (*current_position != '\0') {
			if (*current_position == delimeter) {
				break;
			}
			length++;
			current_position++;
		}
		strncpy(cmd_list[cmd_number], start, length);
		cmd_list[cmd_number][length + 1] = '\0';
		cmd_number++;
		current_position++;
	}
	cmd_list[cmd_number] = NULL;
	return cmd_list;
}

int is_internal_cmd(char * cmd) {
	if ((strcmp(cmd, "cd") == 0) || (strcmp(cmd, "exit") == 0) || (strcmp(cmd, "help") == 0)) {
		return 1;
	}
	return 0;
}

void execute_internal_cmd(char * argument_list[]) {
	if (strcmp(argument_list[0], "cd") == 0) {
		change_directory(argument_list[1]);
	} else if (strcmp(argument_list[0], "exit") == 0) {
		exit(0);
	} else if (strcmp(argument_list[0], "help") == 0) {
		help(argument_list[1]);
	}
}

void change_directory(char * directory_name) {
	if (directory_name != NULL) {
		if (chdir(directory_name) == -1) {
			fprintf(stderr, "Error: Can't change directory to %s.\n", directory_name);
		}
	}
}

void help(char * cmd) {
	if (cmd == NULL) {
		printf("cd <directory>: change directory\nexit: exit the shell\nhelp <command>: help about a command\n");
	} else {
		if (strcmp(cmd, "cd") == 0) {
			printf("cd <directory>: change directory\n");
		} else if (strcmp(cmd, "exit") == 0) {
			printf("exit: exit the shell\n");
		} else if (strcmp(cmd, "help") == 0) {
			printf("help <command>: help about a command\n");
		} else {
			printf("Invalid command\n");
		}
	}	
}

void input_redirection(char * argument_list[]) {
	int i = 0;
	while (argument_list[i] != NULL) {
		if (strchr(argument_list[i], '<')) {
			int fd0 = open(argument_list[i + 1], O_RDONLY);
			dup2(fd0, 0);
			int j = i;
			do {
				argument_list[j] = argument_list[j + 1];
				j++;
			} while (argument_list[j] != NULL);
			break;
		}
		i++;
	}
}

void output_redirection(char * argument_list[]) {
	int i = 0;
	while (argument_list[i] != NULL) {
		if (strchr(argument_list[i], '>')) {
			int fd1 = open(argument_list[i + 1], O_CREAT | O_WRONLY, 0666);
			dup2(fd1, 1);
			int j = i;
			do {
				argument_list[j] = argument_list[j + 1];
				j++;
			} while (argument_list[j] != NULL);
			break;
		}
		i++;
	}
}

int count_pipes(char * cmdline) {
	int count_of_pipes = 0;
	char * position = cmdline;
	while (*position != '\0') {
		if (*position == '|') {
			count_of_pipes++;
		}
		position++;
	}
	return count_of_pipes;
}
