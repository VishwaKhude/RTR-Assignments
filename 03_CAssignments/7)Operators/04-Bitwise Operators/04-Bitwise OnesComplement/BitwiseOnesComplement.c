#include <stdio.h>
int main(void)
{
	//function prototypes
	void PrintBinaryFormOfNumber(unsigned int);

	//varaiable declarations
	unsigned int VMK_a;
	unsigned int VMK_result;

	//code
	printf("\n\n");
	printf("Enter An Integer = ");
	scanf("%u", &VMK_a);

	printf("\n\n\n\n");
	VMK_result = ~VMK_a;
	printf("Bitwise	COMPLEMENTING Of \nA = %d (Decimal) gives result %d (Decimal) \n\n", VMK_a, VMK_result);
	PrintBinaryFormOfNumber(VMK_a);
	PrintBinaryFormOfNumber(VMK_result);

	return(0);
}

/* ****BEGINNERS TO C LANGUAGE : PLEASE IGNORE THE CODE OF THE
      FOLLOWING FUNCTION SNIPPET 'PrintBinaryFormOfNumber()' ****
   **** YOU MAY COME BACK TO THIS CODE AND WILL UNDERSTAND IT MUCH BETTER AFTER
      YOU HAVE COVERED : ARRAYS, LOOPS AND FUNCTIONS ****
   **** THE ONLY OBJECTIVE OF WRITING THIS FUNCTION WAS TO OBTAIN THE BINARY 
      REPRESENTATION OF DECIMAL INTEGERS SO THAT BIT-WISE AND-ing, OR-ing, COMPLEMENT
	  AND BIT-SHIFTING COULD BE UNDERSTOOD WITH GREAT EASE **** */

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
	//variable declarations
	unsigned int quotient, remainder;
	unsigned int num;
	unsigned int binary_array[8];
	int VMK_i;

	//code
	for (VMK_i = 0; VMK_i < 8; VMK_i++)
		binary_array[VMK_i] = 0;

	printf("The Binary Form Of The Decimal Integer %d Is \t=\t", decimal_number);
	num = decimal_number;
	VMK_i = 7;
	while (num != 0)
	{
		quotient = num / 2;
		remainder = num % 2;
		binary_array[VMK_i] = remainder;  
		num = quotient;
		VMK_i--;
	}

	for (VMK_i = 0; VMK_i < 8; VMK_i++)
		printf("%u", binary_array[VMK_i]);

	printf("\n\n");
 }
	  