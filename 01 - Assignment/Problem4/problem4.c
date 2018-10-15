/*	
**	This program is a template for SP lab 3 task 4 you are  
**	required to implement toHexadecimal functions.
**	the program uses bitwise operators to compute bit set
**	and number system conversion
*/
#include <stdio.h>

/*
**	function takes one integer variable and convert it into hexadecimal
**	and print the converted number on stdout. 
*/
void toHexadecimal(unsigned int);

int main()
{
	unsigned int num;
	printf("Enter the number to convert : ");/*taking input to convert*/
	scanf("%d", &num);
	//Call function toHexadecimal from here
	toHexadecimal(num);

	return 0;
}

void toHexadecimal(unsigned int num)
{
	unsigned int mask = 15;
	char result[10] = { 0 };
	static char HEX[] = "0123456789ABCDEF";
	int i = 9;
	while (num > 0) {
		result[i] = HEX[(num & 0xF)];
		num >>= 4;
		i--;
	}
	printf("Hexadecimal: ");
	for (i = 0; i < 10; i++) {
		printf("%c", result[i]);
	}
	printf("\n");
}
