#include <stdio.h>
int main(void)
{
	//variable declarations
	int VMK_i, VMK_j;

	//code
	printf("\n Printing Digits From 1 to 10 and 10 to 100 : \n\n");
	
	for (VMK_i = 1, VMK_j = 10; VMK_i <= 10, VMK_j <= 100; VMK_i++, VMK_j = VMK_j + 10)
	{
		printf("\t %d \t %d\n", VMK_i, VMK_j);
	}

	printf("\n\n");

	return(0);
}
