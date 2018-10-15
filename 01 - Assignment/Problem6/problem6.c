/*	
**	This program is a template for SP lab 3 task 6 you are  
**	required to implement its one function.
**	the program takes string through command line arguments 
**	and convert it into float and display it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
**	the function takes input string from stdin and
**	convert it into float and return it
*/

float string_to_float(char *);
int altoint(char ch);

int main(int argc,char *argv[])
{

	if(argc !=2)
	{
		printf("Invalid number of arguments. Please enter String to convert\n");
		exit(1);
	}


	float ans=string_to_float(argv[1]);		/*	calling finction
*/
	printf("converted number is : %f\n",ans);		/*	displaying converted number
*/
	return 0;
}


float string_to_float(char * str)
{

	float result= 0.0f;

	/*	
	**	Add your code here to convert string into float	
	*/

	int i=0;
	int decimal = 0; //to make sure that decimal exists only once in the given string
	
	//Check if string is valid
	while(str[i] != '\0')
	{
		if(str[i]=='0' || str[i]=='1' || str[i]=='2' || str[i]=='3' || str[i]=='4' || str[i]=='5' || str[i]=='6' || str[i]=='7' || str[i]=='8' || str[i]=='9')
		{
			i++;
		}	
		
		else if(str[i]=='.' && decimal==0)
		{
			i++;
			decimal++;
		}
		
		else 
		{
			printf("Invalid floating point number\n");
			exit(1);
		}
	}

	//Convert string to floating point number
	int num;
	int pos=1;
	int flag=0;
	i=0;

	while(str[i] != '.')
        {
                num = altoint(str[i]);
                result = result*10;
                result = result +(float) num;
                i++;
        }
                
	if(str[i]=='.')
        {
                i++;
                flag=1;
        }
	
	float fnum;
	int j, floatnum;
	while(str[i] != '\0')
	{
			
			floatnum=1;		
			num = altoint(str[i]);
//			printf("num%d\n", num);
			for(j=0; j<pos; j++)
			{
				floatnum = floatnum*10;
			}
			pos++;
			fnum=(float)num/(float)floatnum;
//			 printf("fnum%f\n", fnum);
//			 printf("result%f\n", result);

			result = result+fnum;
//			 printf("res%f\n", result);
			i++;		
	}
	
	return result;
}

int altoint(char ch)
{
	if(ch=='0')
		return 0;
	else if(ch=='1')
		return 1;
	else if(ch=='2')
                return 2;
	else if(ch=='3')
                return 3;
        else if(ch=='4')
                return 4; 
        else if(ch=='5')
                return 5;
        else if(ch=='6')
                return 6;
        else if(ch=='7')
                return 7; 
        else if(ch=='8')
                return 8;
        else if(ch=='9')
                return 9;
}

