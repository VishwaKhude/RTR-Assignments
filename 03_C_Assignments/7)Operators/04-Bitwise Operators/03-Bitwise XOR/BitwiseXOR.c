#include<stdio.h>

int main(void)
{
	//function prototypes
	void PrintBinaryFormOfNumber(unsigned int);

	//variable declarations
	unsigned int VMK_a;
	unsigned int VMK_b;
	unsigned int VMK_result;

	//code 
	printf("\n\n");
	printf("Enter An Integer = ");
	scanf("%u", &VMK_a);

	printf("\n\n");
	printf("Enter Another Integer = ");
	scanf("%u", &VMK_b);

	printf("\n\n\n\n");
	VMK_result = VMK_a ^ VMK_b;
	printf("Bitwise XOR-ing Of \nA = %d (Decimal) and B = %d (Decimal) gives result %d (Decimal) gives result %d (Decimal) \n\n", VMK_a, VMK_b, VMK_result);

	PrintBinaryFormOfNumber(VMK_a);
	PrintBinaryFormOfNumber(VMK_b);
	PrintBinaryFormOfNumber(VMK_result);

	return(0);
}
	/* **** Beginners To C Programming Language : Please Ignore THe Code Of The
	      Following Function Snippet 'PrintBinaryFormOfAnothernumber()' ****
	   **** You May Come Back To This Code And Will Understand It Much Better After
	      You Have Covered : Arrays, Loops And Functions ****  
       **** The Only Objective Of Writing This Function Was To Obtain The Binary
	      Representation Of Decimal Integers So That Bit-Wise AND-ing, OR-ing, Complement
		  And Bit-Shifting Could Be Understood With Great Ease **** */

	void PrintBinaryFormOfNumber(unsigned int decimal_number)
	{
		// variable declarations
		unsigned int VMK_quotient, VMK_remainder;
		unsigned int VMK_num;
		unsigned int VMK_binary_array[8];
		int VMK_i;

		//code
		for (VMK_i = 0; VMK_i < 8; VMK_i++)

			VMK_binary_array[VMK_i] = 0;

		printf("The Binary Form Of THe Decimal Integer %d Is \t=\t", decimal_number);
		VMK_num = decimal_number;
		VMK_i = 7;
		while (VMK_num != 0)
		{
			VMK_quotient = VMK_num / 2;
			VMK_remainder = VMK_num % 2;
			VMK_binary_array[VMK_i] = VMK_remainder;
			VMK_num = VMK_quotient;
			VMK_i--;
		}

		for (VMK_i = 0; VMK_i < 8; VMK_i++)
			printf("%u", VMK_binary_array[VMK_i]);

		printf("\n\n");
	}
  