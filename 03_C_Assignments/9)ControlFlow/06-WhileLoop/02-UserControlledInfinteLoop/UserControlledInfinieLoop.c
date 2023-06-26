#include <stdio.h>
int main(void)
{
	//variable declarations
	char option, ch = '\0'; // '\0' - same as constant '0' means take null value if this operator is assigned

	//code
	printf("\n\n");
	printf("Once The Infinite Loop Begins, Enter 'T' or 'T' To Terminate The Infinite For Loop : \n\n");
	printf("Enter 'Y' or 'y' To Initiate The User Controlled Infinte Loop : ");
	printf("\n\n");
	option = getch();
	if (option == 'Y' || option == 'y')
	{
		while (1) //Infinite Loop
		{
			printf("In Loop...\n");
			ch = getch();
			if (ch == 'T' || ch == 't')
				break; //User Controlled Exitting From Infinte Loop
		}

		printf("\n\n");
		printf("YOU HAVE PRESSED 'T', TERMINATING THE USER CONTROLLED INFINTE LOOP...");
		printf("\n\n");
	}

	else
		printf("You Must Press 'Y' or 'y' To Initiate The User Controlled Infinte Loop...Please Try Again!\n\n");

	return(0);
}
