#include <stdio.h>
int main(void)
{
	//variable declarations
	int i, j;

	//code
	printf("\n Printing Digits From 10 to 1 & 100 to 1 : \n");

	for (i = 10, j = 100; i >= 1, j >= 10; i--, j -= 10)
	{
		printf("\t %d \t %d\n", i, j);
	}

	return(0);
}
