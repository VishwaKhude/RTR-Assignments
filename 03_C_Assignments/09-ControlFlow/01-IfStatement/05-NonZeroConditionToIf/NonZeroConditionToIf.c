#include <stdio.h>
int main(void)
{
	//variable declarations
	int a;

	//code
	printf("\n\n");

	a = 5;
	if (a) //Non-zero Positive Value
	{
		printf("if-block 1 : 'A' Exists And Has Positive Value = %d !!!\n\n", a);
	}

	a = -5;
	if (a) //Non-zero Negative Value
	{
		printf("if-block 2 : 'A' Exists And Has An Negative Value = %d !!!\n\n", a);
	}

	a = 0;
	if (a) //zero
	{
		printf("if-block 3 : 'A' Exists And Has An Zero Value = %d !!!\n\n", a);
	}

	printf("All Three if-statements Are Done !!! \n\n");
	return(0);
}
