/*
**	This program is a template for SP lab 3 task 1 you are  required to
**	complete the logic for tokenizing the string and taking input.
**	the program should convert string into tokens without strtok
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define MAX_LEN 100
#define MAX_TOKENS 7  
#define TOKEN_LEN 14

/*
**	receives a line from user character by character using getchar()
**	it returns pointer to string
*/

/*	
**	this function gets character by character input, creates and returns a string 	
*/
char* read_line();

/*
**	this function takes string and convert the string into tokens of words 
**	without using strtok
*/
char** tokenize(char*);



int main()
{
	char *line;
	line = read_line();	/*Calling function to read line*/	
	printf("Entered input: %s\n",line);
	char** words = tokenize(line);	/*calling function to tokenize string*/

	/*display words each on a new line*/
	for(int i=0; words[i] != '\0'; i++)
		printf("%d. %s\n",i+1 , words[i]);

	/*Add our cde here to free 2D memory for words*/
	for (int i = 0; i < MAX_TOKENS + 1; i++) {
		free(words[i]);
	}
	free(words);
	/*Add our cde here to free 1D memory of input string*/
	free(line);
	return 0;
}

char* read_line()
{
	/*
	 * initializing variables
	 * */
	char ch;
	int pos = 0; 
	char* line = (char*) malloc(sizeof(char)*MAX_LEN);

	/*
	 * Add your code here for taking input with getchar( ) function
	 * */	
	printf("Enter a sentence: ");
	while ((ch = getchar()) != '\n') {
		*(line + pos) = ch;
		pos++;
	}
	*(line + pos) = '\0';
	return line;
}

char** tokenize(char* line)
{
	/*allocate memory*/
	char** words = (char**)malloc(sizeof(char*)* (MAX_TOKENS+1));		
	for(int j=0; j < MAX_TOKENS+1; j++)
	{
		words[j] = (char*)malloc(sizeof(char)* TOKEN_LEN);
		bzero(words[j],TOKEN_LEN);
	}
	int pos=0;int i=0;
	/*Add youir code here to tokenize the string coming as a parameter line*/
	while (line[pos] != '\0' && i < MAX_TOKENS + 1) {
		int k = pos;
		for (int j = 0; line[k] != '\0' && j < TOKEN_LEN - 1 && line[k] != ' '; j++) {
			words[i][j] = line[k];
			k++;
		}
		i++;
		pos = k + 1;
	}
	return words;
}
