/*
**	This program is a template for SP lab 3 task 5 you are 
**	required to implement its one function.
*/


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define GREP 0
#define REPLACE 1

/*	
**	This function take file pointer as paramter to read content from and 
**	char pointer as an second argument which points to string to search in file
*/
void mygrep(FILE*, char*);

/*	
**	This function take file pointer as paramter to read content from and 
**	char pointer as an second argument which points to string to search in file
** 	and char pointer as an third argument to replace the string finded with it.
*/
void myreplace(FILE *fp,char *find, char * replace);


int  main(int argc,char *argv[])
{


	/*creating variables*/
	int behaviour;
	FILE *fp;
	char *filename=argv[1];
	char *find=argv[2];
	char * replace;

	/*check if mygrep is called or myreplace*/
	if(strcmp(argv[0], "./mygrep") == 0)
	{
		if(argc != 3)
		{
			printf("usage\t./mygrep filename <string-to-search>\n");
			exit(1);
		}
		behaviour = GREP;
	}
	else if(strcmp(argv[0], "./myreplace") == 0)
	{
		if(argc != 4)
		{
			printf("usage\t./myreplace filename  <string-to-search> <string-to-replace>\n");
			exit(1);
		}
		behaviour = REPLACE;
		replace = argv[3];
	}
	else
	{
		behaviour = -1;
	}

	/*opening file*/
	fp=fopen(filename,"rt");

	if(behaviour == GREP)
	{
		mygrep(fp,find);/*caling function*/
	}
	else if ( behaviour == REPLACE )
	{
		myreplace(fp,find,replace);/*calling myreplace*/
	}
	
	fclose(fp);/*closing file*/
	return 0;
}


void mygrep(FILE *fp,char *find)
{
	char c1[500];
	/*Add code to get strings from file*/
	while (fgets(c1, 500, fp))
	{
		/*Add your code here to search a string find on string c1 readed from file*/
		if (strstr(c1, find) != NULL) {
			printf("%s\n", c1);
		}
	}
}

void myreplace(FILE *fp,char *find, char * replace)
{
	char c1[500];
	char c2[500];
	int i = 0, j = 0, k = 0;
	bool found = false, firstTime = false;
	while (fgets(c1, 500, fp)) {
		i = 0;
		k = 0;
		found = false;
		while (c1[i] != '\0') {
			if (!firstTime && c1[i] == find[0]) {
				j = 0;
				found = true;
				while (find[j] != '\0') {
					if (c1[i + j] != find[j]) {
						found = false;
						break;
					}
					j++;
				}
			}
			if (found) {
				firstTime = true;
				found = false;
				i = i + j;
				j = 0;
				while (replace[j] != '\0') {
					c2[k] = replace[j];
					k++;
					j++;
				}
			}
			if (!found) {
				c2[k] = c1[i];
				k++;
				i++;
			}
		}
		c2[k] = '\0';
		printf("%s\n", c2);
	}
}
