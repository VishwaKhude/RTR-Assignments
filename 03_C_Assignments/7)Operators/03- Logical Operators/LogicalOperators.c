#include <stdio.h>

int main(void)
{
	//variable declarations
	int a;
	int b;
	int c;
	int result;

	//code
	printf("\n\n");
	printf("Enter First Integer :");
	scanf("%d", &a);

	printf("\n\n");
	printf("Enter Second Integer :");
	scanf("%d", &b);

	printf("\n\n");
	printf("Enter Third Integer :");
	scanf("%d", &c);

	printf("\n\n");
	printf("If Answer = 0, It Is 'False'\n");
	printf("If Answer = 1, It Is 'True' \n");

	result = (a <= b) && (b != c);
	printf("LOGICAL AND (&&) : Answer is TRUE (1) if and only if BOTH Conditions are True.The Answer is FALSE(0), if any One or Both conditions are False\n\n");
	printf("A = %d is Less than or Equal to B = %d AND B = %d is NOT Equal to C = %d		\t Answer = % d \n\n", a, b, b, c, result);

	result = (b >= a) || (a == c);
	printf("LOGICAL OR (||) : Answer is FALSE (0) if and Only if BOTH Conditions are False.The Answer is TRUE(1), if any One or Both Conditions are True \n\n");
	printf("Either B = %d is Greater Than or Equal To A = %d OR A = %d is Equal To C = %d		\t Answer = % d\n\n", b, a, a, c, result);

			result = !a;
	printf("A = %d and using Logical NOT (!) Operator on A Gives Result = %d\n\n", a, result);

	result = !b;
	printf("B = %d and using Logical NOT (!) Operator on B Gives Result = %d\n\n", b, result);

	result = !c;
	printf("C = %d and using Logical NOT (!) Operator on C Gives Result = %d\n\n", c, result);

	result = (!(a <= b) && !(b != c));
	printf("Using Logical NOT(!) on (a <= b And Also On (b != c) and then ANDing them afterwards givees result = %d\n", result);

	printf("\n\n");

	result = !(b >= a) || (a == c);
	printf("Using Logical NOT (!) on entire Logical expression (b >= a) || (a == c) gives result = %d\n", result);

	printf("\n\n");

	return(0);
}
