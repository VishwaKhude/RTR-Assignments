#include <stdio.h>
int main(void)
{
	//variable declarations
	char option, ch = '\0';

	//code
	printf("\n\n");
	printf("Once The Infinte Loop Begins, Enter 'Q' or 'q' To Quit The Infinte For Loop : \n\n");

	do
	{
		do
		{
			printf("\n");
			printf("In Loop...\n");
			ch = getch(); //control flow waits for character input...
		} while (ch != 'Q' && ch != 'q');
		printf("\n\n");
		printf("EXITTING FROM THE USER CONTROLLED INFINTE LOOP...");

		printf("\n\n");
		printf("DO YOU WANT TO BEGIN THE USER - CONTROLLED INFINTE LOOP AGAIN?...(Y/y - Yes, Any Other Key - No) : ");
		option = getch();
	} while (option == 'Y' || option == 'y');

	return(0);
}
