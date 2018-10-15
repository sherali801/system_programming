
/*
**	This program is a template for SP lab 3 task 2 you are 
**	required to complete the main function.
*/


#include <stdio.h>
#include<stdlib.h> 


/*
**	the function should read a file and matrices and calculates the 
**	matrix multiplication and print the resultant matrix on screen.
*/
int main(int argc, char ** argv)
{

	if( argc != 2 )
	{
		printf("must enter the file name\n usage\t./a.out filename\n");
		return 1;
	}
	FILE *myFile;
	myFile=fopen(argv[1],"r");		/*	opening file	*/

	int f_row, f_col;
	int s_row, s_col;
	int  i, j, k, sum = 0;

	if(myFile==NULL)
	{
		printf("Error Reading File \n ");
	}

	/*	reading the rows and columns of first matrix
*/
	fscanf(myFile,"%d",&f_row);
	fscanf(myFile,"%d",&f_col);
	int ** A;

	/*	Allocate the space for Matrix A
*/	A=(int **)malloc(sizeof(int*)*f_row);
	for(i=0;i<f_row;i++){
		A[i] = (int *) malloc(sizeof(int)* f_col);
	}

	/* 	Read a matrix entries from file
*/	for(i=0;i<f_row;i++){
		for(j=0;j<f_col;j++){
			fscanf(myFile, "%d", &A[i][j]);
		}
	}

	/*	reading the rows and columns of first matrix
*/	printf("First Matrix:\n");
	for(i=0;i<f_row;i++){
                for(j=0;j<f_col;j++){
                       printf("%d ", A[i][j]);
                }
		printf("\n");
        }

	fscanf(myFile,"%d",&s_row);
	fscanf(myFile,"%d",&s_col);
	int ** B;

	/*	Allocate the space for Matrix B
*/	B=(int **)malloc(sizeof(int*)*s_row);
        for(i=0;i<s_row;i++){
		B[i] = (int *) malloc(sizeof(int) * s_col);
        }


	/* 	Read a matrix entries from file
*/	for(i=0;i<s_row;i++){
                for(j=0;j<s_col;j++){
                        fscanf(myFile, "%d", &B[i][j]);
                }
        }

	printf("Second Matrix:\n");
        for(i=0;i<s_row;i++){
                for(j=0;j<s_col;j++){
			printf("%d ", B[i][j]);
                }
                printf("\n");
        }



	

	int ** C;
	
	/*	Allocate the space for Matrix C resultant matrix it size must be rows of first and 
*/	C=(int **)malloc(sizeof(int*)*f_row);
        for(i=0;i<f_row;i++){
               C[i] = (int *) malloc(sizeof(int) * s_col);
        }


	/*	Matrix multiplication
*/
	for (i = 0; i < f_row; i++) 
	{
		for (j = 0; j < s_col; j++) 
		{
			for (k = 0; k < s_row; k++) 
			{
				sum = sum + A[i][k] * B[k][j];
			}

			C[i][j] = sum;

			sum = 0;
		}
	}

/*      Print the resultant matrix
*/
	

	printf("Product of entered matrices:-\n");
	 for (i = 0; i < f_row; i++)
        {
                for (j = 0; j < s_col; j++)
                {
			printf("%d ", C[i][j]);
                }
		printf("\n");
        }


	return 0;
}
