#include <stdio.h>

int main(void)
{
	//variable declarations
	int vk_a;
	int vk_b;
	int vk_result;
	
	//code
	printf("\n\n");
	printf("Enter One Integer : ");
	scanf("%d", &vk_a);

	printf("\n\n");
	printf("Enter Another Integer :");
	scanf("%d", &vk_b);

	printf("\n\n");
	printf("If Answer = 0, It Is 'FALSE' \n");
	printf("If Answer = 1, It Is 'TRUE' \n\n");

	vk_result = (vk_a < vk_b);
	printf("(a < b) A = %d Is Less Than B = %d                  \t Answer = %d\n",
		vk_a, vk_b, vk_result);

	vk_result = (vk_a > vk_b);		
	printf("(a > b) A = %d Is Greater Than B = %d				\t Answer = %d\n",
		vk_a, vk_b, vk_result);

	vk_result = (vk_a <= vk_b);
	printf("(a <= b) A = %d Is Less Than Or Equal To B = %d		\t Answer = %d\n",
		vk_a, vk_b, vk_result);

	vk_result = (vk_a >= vk_b);
	printf("(a >= b) A = %d Is Greater Than Or Equal To B = %d	\t Answer = %d\n",
		vk_a, vk_b, vk_result);

	vk_result = (vk_a == vk_b);
	printf("(a == b) A = %d Is Equal To B = %d					\t Answer = %d\n",
		vk_a, vk_b, vk_result);

	vk_result = (vk_a != vk_b);									
	printf("(a != b) A = %d Is NOT Equal To B = %d				 \t	Answer = %d\n",
		vk_a, vk_b, vk_result);

	return(0);
}
