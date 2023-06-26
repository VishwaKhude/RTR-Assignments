#include <stdio.h>
int main(void)
{
	//variable declarations
	int i, j, c;

	//code
	printf("\n\n");

	//Each row is made of 8 blank spaces & 8 block made of star pattern which make a chess board
	//Lets assume the blank blocks are black and the star sequence of 8 stars are white blocks same as in a checker board(chess board
	i = 0;
	while (i < 64)
	{
		j = 0;
		while (j < 64)
		{

			c = ((i & 0x8) == 0) ^ ((j & 0x8) == 0); //eight rows

			if (c == 0)
				printf("  "); //4 blocks  made of 8 blank spaces

			if (c == 1)
				printf("* "); //4 blocks made of 8 stars

			j++;

		}
		printf("\n\n");
		i++;
	}
	return(0);
}
