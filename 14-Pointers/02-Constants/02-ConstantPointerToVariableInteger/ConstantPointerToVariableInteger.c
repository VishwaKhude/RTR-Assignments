#include <stdio.h>

int main(void)
{
    // variable declarations
    int num = 5;
    int* const ptr = &num; // Read this line from right to left => "ptr is a constant (const) pointer (*) to integer (int)."

    //code
    printf("\n\n");
    printf("Current Value Of 'num' = %d\n", num);
    printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

    // The following line does NOT give error ... as we are modifying the value of the variable indivisually
    num++;
    printf("\n\n");
    printf("After num++, value of 'num' = %d\n", num);

    // The following line gives error and is hence commented out.
    // We cannot alter the 'ptr' value as 'ptr' is "a constant pointer to integer."
    // With respect to the pointer, the value it points to is not constant but the pointer itself is conastant.
    // Uncomment it and see the error.

    // ptr++;

    // The following line does NOT give error
    // We do not get error because we are changing the value at a constant pointer (address).
    // The pointer is constant. The value to which the pointer is NOT constant.
   // (*ptr)++;

    printf("\n\n");
    printf("After (*ptr)++, value of 'ptr' = %p\n", ptr);
    printf("Value at this 'ptr' = %d\n", *ptr);
    printf("\n");
    return(0);
}

// CONCLUSION :
// As "ptr" is a "constant pointer to a variable integer" - we can change the value stored at address "ptr" but we cannot change the 'ptr' (Address) itself.
// We can change the value of the variable indivisually - whose address is contained in "ptr".
// We can also change the "value at address of ptr" - we can change the value of "num" with respect to "ptr" => (*ptr)++ is allowed
// We cannot change the value OF 'ptr' => THat is we cannot store a new address inside 'ptr' => So, ptr++ is NOT allowed 

