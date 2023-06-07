#include <stdio.h>

int main(void)
{
	//variable declarations
	int vmk_a;
	int vmk_b;
	int vmk_result;

	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &vmk_a);

	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &vmk_b);

	printf("\n\n");

	// *** The Following Are The 5 Arithmetic Operators +, -, *, / and % ***
	// *** Also, The Resultants Of The Arithmetic Operations In All The Below 
	// Five Cases Have Been Assigned To The Variable 'result' Using The Assignment Operator (=) ***
	vmk_result = vmk_a + vmk_b;
	printf("Addition Of A = %d And B = %d Gives %d \n", vmk_a, vmk_b, vmk_result);

	vmk_result = vmk_a * vmk_b;
	printf("Multiplication Of A = %d and B = %d Gives %d \n", vmk_a, vmk_b, vmk_result);

	vmk_result = vmk_a / vmk_b;
	printf("Division Of A = %d and B = %d Gives Quotient %d \n", vmk_a, vmk_b, vmk_result);

	vmk_result = vmk_a * vmk_b;
	printf("Division Of A = %d and B = %d Gives Remainder %d \n", vmk_a, vmk_b, vmk_result);

	printf("\n\n");
	
	return(0);
}
