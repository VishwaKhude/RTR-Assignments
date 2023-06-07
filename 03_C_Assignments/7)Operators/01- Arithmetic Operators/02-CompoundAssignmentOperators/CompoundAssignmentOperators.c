#include <stdio.h>

int main(void)
{
	//variable declarations
	int a;
	int b;
	int x;

	//code 
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &a);

	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &b);

	printf("\n\n");

	//Since, In All The Following 5 cases, the operand on the left 'a' is 
	//getting repeated immediately on the right (e.g.: a = a + b or a = a - b),
	//We are using compound assignment operators +=, -=, *=, /= and %= 

	//Since, 'a' will be assigned the value of (a + b) at the expression (a += b), we must save the original 
	//value of 'a' to another variable (x)
	x = a;
	a += b; // a = a + b
	printf("Addition of A = %d and B = %d gives %d\n", x, b, a);

	//Value Of 'a' Alterd in the above expression is used here...
	//Since, 'a' will be assigned the value of (a - b) at the expression (a -= b)
	//We must save the original value of 'a' to another variable (x)
	x = a;
	a -= b; // a = a - b
	printf("Substraction of A = %d and B = %d gives %d \n", x, b, a);

	//Value Of 'a' Altered in the above expression is used here...
	//Since, 'a', will be assigned the value 0f (a * b) at the expression (a *= b)
	//We must save the original value of 'a' to another variable (x)
	x = a;
	a *= b; // a = a * b
	printf("Multiplication of A = %d and B = %d gives %d \n", x, b, a);

	//Value of 'a' Altered in the above expression is used here...
	//Since, 'a' will be assigned the value of (a / b) at the expression (a /= b)
	//We must save the original value of 'a' to another variable (x)
	x = a;
	a /= b; // a = a / b
	printf("Division of A = %d and B = %d gives Quotient %d\n", x, b, a);

	//Value of 'a' Altered in the above expresssion is used here...
	//Since, 'a' will be assigned the value of (a % b) at the expression (a %= b),
	//We must save the original value of 'a' to another variable (x)
	x = a;
	a %= b; // a = a % b

	printf("Division of a = %d and b = %d gives Remainder %d \n", x, b, a);

	printf("\n\n");

	return(0);
}
