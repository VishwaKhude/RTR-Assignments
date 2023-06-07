#include <stdio.h>
int main(void)
{
	//variable declarations
	char option, ch = '\0';

	//code
	printf("\n\n Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The 'Infinte Loop' : \n\n");
	printf("Enter 'Y' or 'y' To Initialize The User Controlled Loop : \n\n");
	option = getch();
	if (option == 'Y' || option == 'y')
	{
		for (;;) //Infinite Loop
		{
			printf("In The Infinte Loop...\n");
			ch = getch();
			if (ch == 'Q' || ch == 'q')
				break; //User Controlled Exitting From The 'Infinite Loop'
		}
	}

	printf("\n\n");
	printf("EXITTING FROM THE USER CONTROLLED INFINTE LOOP...\n");

	return(0);
}
