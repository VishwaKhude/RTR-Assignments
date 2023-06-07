#include <stdio.h>
int main(void)
{
	//variable declarations
	int i_num, num, i;

	//code
	printf("\n\n");

	printf("Enter An Integer Value From Which Iteration Must Begin : ");
	scanf("%d", &i_num);

	printf("How Many Digits Do You Want To Print From %d Onwards ? : ", i_num);
	scanf("%d", &num);

	printf("Printing Digits %d to %d : \n\n", i_num, (i_num + num));

	i = i_num;    //'i' integer is used to store and display the addition of i_num and num. If not done so and if assigned i_num only, the addition will turn into an infinte loop 
	while (i <= i_num + num)
	{
		printf("\t %d\n", i);
		i++;
	}

	printf("\n");

	return(0);
}
