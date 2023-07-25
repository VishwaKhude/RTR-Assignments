#include <stdio.h>
#include <conio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declarations
    int iArray[NUM_ROWS][NUM_COLUMNS];
    int i, j;

    //code
    // *** EVERY ROW OF A 2D ARRAY IS AN INTEGER ARRAY ITSELF COMPRISONG OF 'NUM_COLUMNS' INTEGER ELEMENTS ***
    // *** THERE ARE 5 ROWS AND 3 COLUMNS IN A 2D INTEGER ARRAY, EACH OF THE 5 ROWS IS A 1D ARRAY OF 3 INTEGERS.
    // *** HENCE, EACH OF THESE 5 ROWS THEMSELVES BEING ARRAYS, WILL BE THE BASE ADDRESSES OF THEIR RESPECTIVE ROWS ***

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
            *(iArray[i] + j) = (i + 1) * (j + 1); // 'iArray[i]' Can Be Treated As 1D Array Using Pointers...

    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*iArray[%d] + %d) = %d \t \t At Address (iArray[i] + j) : %p\n", i, j, *(iArray[i] + j), (iArray[i] + j));
        }
        printf("\n\n");
    }

    return(0);
}
