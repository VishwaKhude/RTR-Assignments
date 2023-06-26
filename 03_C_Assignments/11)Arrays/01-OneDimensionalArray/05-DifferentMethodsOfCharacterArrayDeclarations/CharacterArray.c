#include <stdio.h>

int main(void)
{
    //varaible declarations
    char chArray_01[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0' }; // Must give \0 Explicitly for Proper Initialization
    char chArray_02[9] = {'W', 'E', 'L', 'C', 'O', 'M', 'E', 'S', '\0' }; //Must give \0 explicitly for Proper Initialization
    char chArray_03[] = { 'Y', 'O', 'U', '\0' }; //Must Give \0 Explicitly For Proper Initializaton
    char chArray_04[] = "To"; // \0 is assumed, size is given as 3, although string has only 2 characters
    char chArray_05[] = "REAL TIME RENDERING BATCH OF 2023-24"; //\0 is assumed, size is given as 40, although string has 39 characters
    
    char chArray_WithoutNullTerminator[] = { 'H', 'e', 'l', 'l', 'o' };

    //code
    printf("\n\n");

    printf("Size Of chArray_01 : %zu\n\n", sizeof(chArray_01)); // use '%zu' instead
    printf("Size Of chArray_02 : %zu\n\n", sizeof(chArray_02)); // of using '%lu'
    printf("Size Of chArray_03 : %zu\n\n", sizeof(chArray_03)); // to avoid
    printf("Size Of chArray_04 : %zu\n\n", sizeof(chArray_04)); // compilation
    printf("Size Of chArray_05 : %zu\n\n", sizeof(chArray_05)); // error

    printf("\n\n");

    printf("The Strings Are : \n\n");
    printf("chArray01 : %s\n\n", chArray_01);
    printf("chArray02 : %s\n\n", chArray_02);
    printf("chArray03 : %s\n\n", chArray_03);
    printf("chArray04 : %s\n\n", chArray_04);
    printf("chArray05 : %s\n\n", chArray_05);

    printf("\n\n");
    printf("Size Of chArray_WithoutNullTerminator : %zu\n\n", sizeof (chArray_WithoutNullTerminator)); //use '%zu' instead of '%lu' to avoid compilation error
    printf("chArray_WithoutNullTerminator : %s\n\n", chArray_WithoutNullTerminator); //Wiil display garbage value at the end of the string due to absence of \0

    return(0);
}
