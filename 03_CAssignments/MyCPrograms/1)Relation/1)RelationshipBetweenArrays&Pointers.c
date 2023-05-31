//Arrays and pointers relationship
#include <stdio.h>
main(void)
{
	int arr[3] = { 10, 15, 20 };
	printf("First element of array is at %p \n", arr);
	printf("Second element of array is at %p \n", arr + 1);
	printf("Third element of array is at %p \n", arr + 2);

	return(0);
}
