#include <stdio.h>
int main(void)
{
	//variable declarations
	int i;

	//code
	printf("\n\n");

	printf("Printing Even Numbers From 0 To 100 : \n\n");

	for (i = 0; i <= 100; i++)
	{

	   //condition for a number to be even number => division of number by 2 leaves the number no remainder (remaindere = 0)
	   //if remainder is not 0, then the remainder is an odd number...

		if (i % 2 != 0) //every number is divided by 2 and checked whether is divisible by 2 until 100
		{
			continue; //the loop will be continued if the number is divisible by 2
	    }
		else
		{
			printf("\t%d\n", i);
		}
	}

	printf("\n\n");

	return(0);
}
