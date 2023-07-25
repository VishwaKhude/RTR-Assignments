#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    void MyStrcat(char *, char *);
    int MyStrlen(char *);

    //variable declarations
    char *chArray_One = NULL, *chArray_Two = NULL; // A Character Is A String

    //code

    // *** STRING INPUT ***
    printf("\n\n");
    chArray_One = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_One == NULL)
    {
        printf("MEMORY ALLOCATION TO FIRST STRING FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }

    printf("Enter First String :\n\n");
    gets_s(chArray_One, MAX_STRING_LENGTH);

    printf("\n\n");
    chArray_Two = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_Two == NULL)
    {
        printf("MEMORY ALLOCATION TO SECOND STRING FAILED !!! EXITTING NOW !!!...\n\n");
        exit(0);
    }

    printf("Enter Second String : \n\n");
    gets_s(chArray_Two, MAX_STRING_LENGTH);

    // *** STRING CONCAT ***
    printf("\n\n");
    printf("***** BEFORE CONCATENATION *****");
    printf("\n\n");
    printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
    printf("%s\n", chArray_One);

    printf("\n\n");
    printf("The Original Second String Entered By You (i,e : 'chArray_Two[]') Is : \n\n");
    printf("%s\n", chArray_Two);

    MyStrcat(chArray_One, chArray_Two);

    printf("\n\n");
    printf("***** AFTER CONCATENATION *****");
    printf("\n\n");
    printf("chArray_One[]' Is : \n\n");
    printf("%s\n", chArray_One);

    printf("\n\n");
    printf("chArray_Two[]' Is : \n\n");
    printf("%s\n", chArray_Two);

    if (chArray_Two)
    {
        free(chArray_Two);
        chArray_Two = NULL;
        printf("\n\n");
        printf("MEMORY ALLOCATED TO SECOND STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (chArray_One)
    {
        free(chArray_One);
        chArray_One = NULL;
        printf("\n\n");
        printf("MEMORY ALLOCATED TO FIRST STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return(0);
}

void MyStrcat(char *str_destination, char *str_source)
{
    //function prototype
    int MyStrlen(char *);

    //variable declarations
    int iStringLength_Source = 0, iStringLength_Destination = 0;
    int i, j;

    //code
    iStringLength_Source = MyStrlen(str_source);
    iStringLength_Destination = MyStrlen(str_destination);

    //ARRAY INDICES BEGIN FROM 0, HENCE, LAST VALID INDEX OOF ARRAY WILL ALWAYS BE (LENGTH - 1)
    // SO, CONCATENATION MUST BEGIN FROM INDEX NUMBER EQUAL TO LENGTH OF THE ARRAY 'str_destination'
    // WE NEED TO PUT THE CHARACTER WHICH IS AT FIRST INDEX OF 'str_source' TO THE (LAST INDEX + 1) OF 'str_destination'
    for (i = iStringLength_Destination, j = 0; j < iStringLength_Source; i++, j++)
    {
        *(str_destination + i) = *(str_source + j);
    } 

    *(str_destination + i) = '\0';
}

int MyStrlen(char *str)
{
    // variable declarations
    int j;
    int string_length = 0;

    //code
    // *** DETERMINING EXACT LENGTH OF THE STRING , BY DETECTING THE FIRST OCCURENCE OF NULL-TERIMINATING CHARACTER ( \0 ) ***
    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if (str[j] == '\0')
            break;
        else    
            string_length++;
    }
    return(string_length);
}
