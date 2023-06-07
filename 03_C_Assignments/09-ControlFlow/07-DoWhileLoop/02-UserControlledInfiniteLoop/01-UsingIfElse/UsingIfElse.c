#include <stdio.h>
int main(void)
{
	//variable declarations
	char option, ch = '\0';

	//code
	printf("\n\n");
	printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The Infinte For Loop : \n\n");
	printf("Enter 'Y' or 'y' To Initiate User Controlled Infinte Loop : ");
	printf("\n\n");
	option = getch();
	if (option == 'Y' || option == 'y')
	{
		do
		{
			printf("In Loop...\n");
			ch = getch(); //control flow waits for character input...
			if (ch == 'Q' || ch == 'q')
				break; //User Controlled Exitting From The Infinte Loop
		} while (1); //Infinite Loop

		printf("\n\n");
		printf("EXITTING FROM THE USER CONTROLLED LOOP...");
		printf("\n\n");
	}

	else
		printf("Wrong Key Pressed! You Must Press 'Y' or 'y' To Initiate The User Controlled Infinte Loop...Please Try Again!\n\n");

	return(0);
}
