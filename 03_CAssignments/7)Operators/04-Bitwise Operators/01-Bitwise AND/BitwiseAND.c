#include <stdio.h>

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
    printf("Enter An Integer = ");
    scanf("%u", &VMK_b);

    printf("\n\n\n\n");
    VMK_result = VMK_a & VMK_b;
    printf("Bitwise AND-ing Of \nA = %d (Decimal) and B = %d (Decimal) gives result %d (Decimal) \n\n", VMK_a, VMK_b, VMK_result);

    PrintBinaryFormOfNumber(VMK_a);
    PrintBinaryFormOfNumber(VMK_b);
    PrintBinaryFormOfNumber(VMK_result);

    return(0);
}


// **** BEGINNERS TO C PROGRMMING LANGUAGE : PLEASE IGNORE THE CODE OF THE
//   FOLLOWING FUNCTION SNIPPET 'PrintBinaryFormOfNumber()'  ****
// **** YOU HAVE COME BACK TO THIS CODE AND WILL UNDERSTAND IT MUCH BETTER AFTER
//   YOU HAVE COVERED : ARRAYS, LOOPS AND FUNCTIONS ****
// **** THE ONLY OBJECTIVE OF WRITING THIS FUNCTION WAS TO OBTAIN THE BINARY
//   REPRESENTATION OF DECIMAL INTEGERS SO THAT BIT-WISE AND-ing, OR-ing, COMPLEMENT
//   AND BIT-SHIFTING COULD BE UNDERSTOOD WITH GREAT EASE ****

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
    // variable declarations
    unsigned int quotient, remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    //code
    for (i = 0; i < 8; i++)

      binary_array[i] = 0;

printf("The Binary Form Of The Decimal Integer %d Is \t=\t", decimal_number);
num = decimal_number;
i = 7;
while (num != 0)
{
    quotient = num / 2;
    remainder = num % 2;
    binary_array[i] = remainder;
    num = quotient;
    i--;
}

for (i = 0; i < 8; i++)
    printf("%u", binary_array[i]);

printf("\n\n");    
}
