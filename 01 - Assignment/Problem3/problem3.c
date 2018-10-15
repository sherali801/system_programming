/*
**	This program is a template for SP lab 3 task 4 you are  
**	required to complete the logic for tokenizing the string.
*/

#include <stdio.h>
#include <string.h>


int main()
{
        /*
        **      char array to keep input string size is 100 the string contain 
        **      7 word each of size not more than 13 so 7*14=98
        */
        char line[100];
        printf("Enter the line to tokenize\n");
        fgets(line, 100, stdin);


        char arr[8][14];

        /*Add your code here */
	/*int i = 0, pos = 0;
	while (line[pos] != '\0' && i < 8) {
		int k = pos, j;
		for (j = 0; line[k] != '\0' && j < 13 && line[k] != ' '; j++) {
			arr[i][j] = line[k];
			k++;
		}
		arr[i][j] = '\0';
		i++;
		pos = k + 1;
	}*/

        char* ptr;
        int i=0;
        ptr = strtok(line, " ");
        while(ptr !=NULL){

                strcpy(arr[i++],ptr);
                ptr = strtok(NULL, " ");
        }

        for(i=0;i<8;i++){
                printf("%s\n",arr[i]);
        }


        return 0;
}
