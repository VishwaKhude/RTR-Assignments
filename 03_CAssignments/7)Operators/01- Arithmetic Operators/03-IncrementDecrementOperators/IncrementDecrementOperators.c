#include <stdio.h>

int main(void)
{
	// variable declarations
	int vmk_a = 5;
	int vmk_b = 10;

	//code
	printf("\n\n");
	printf("A = %d\n", vmk_a);
	printf("A = %d\n", vmk_a++);
	printf("A = %d\n", vmk_a);
	printf("A = %d\n\n", ++vmk_a);

	printf("B = %d \n", vmk_b);
	printf("B = %d \n", vmk_b--);
	printf("B = %d \n", vmk_b);
	printf("B = %d \n\n", --vmk_b);

	return(0);
}
