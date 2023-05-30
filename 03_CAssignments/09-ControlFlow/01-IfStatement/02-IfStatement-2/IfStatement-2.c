#include <stdio.h>
int main(void)
{
    //variable declaraions
	int age;

	//code
	printf("\n\n");
	printf("Enter Your Age : ");
	scanf("%d", &age);
	if (age >= 18)
	{
		printf("You Are Eligible For Voting");
	}

	return(0);
}
