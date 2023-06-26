#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declarations

    // A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] IS A char ARRAY AND HENCE, char[] IS A 'STRING' ***
    // *** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! ***
    // *** HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING ***
    // *** HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OF STRINGS ***

    // Here, the string array can allow a maximum number of 5 strings (5 rows) and each of these 5 strings can have nly upto 10 characters maximum (10 columns)
    char strArray[5][10]; //5 ROWS (0, 1, 2, 3, 4) -> 5 STRINGS (EACH STRING CAN HAVE A MAXIMUM OF 10 CHARACTERS)
    int char_size;
    int strArray_size;
    int strArray_num_elements, strArray_num_rows, strArray_num_columns;
    int i;

    //code
    printf("\n\n");

    char_size = sizeof(char);

    strArray_size = sizeof(strArray);
    printf("Size Of Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_size);

    strArray_num_rows = strArray_size / sizeof(strArray[0]);
    printf("Number Of Rows (Strings) In Two Dimensional ( 2D ) Character Array (StringArray) Is = %d\n\n", strArray_num_rows);

    strArray_num_columns = sizeof(strArray[0]) / char_size;
    printf("Number Of Columns In Two Dimensional ( 2D ) Character Array (String) Is = %d\n\n", strArray_num_columns);

    strArray_num_elements = strArray_num_rows * strArray_num_columns;
    printf("Maximum Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_num_elements);

    // *** PIECE-MEAL ASSIGNMENT ***
    // *** ROW 1 / STRING 1 ***
    strArray[0][0] = 'M';
    strArray[0][1] = 'y';
    strArray[0][2] = '\0'; //NULL-TERMINATING CHARACTER 

    // ****** ROW 2  / STRING 2 ******
    strArray[1][0] = 'N';
    strArray[1][1] = 'a';
    strArray[1][2] = 'm';
    strArray[1][3] = 'e';
    strArray[1][4] = '\0'; //NULL-TERMINATING CHARACTER 

    // ****** ROW 3 / STRING 3 ******
    strArray[2][0] = 'I';
    strArray[2][1] = 's';
    strArray[2][2] = '\0'; // NULL-TERMINATING CHARACTER

    // ****** ROW 4 / STRING 4 ******
    strArray[4][0] = 'V';
    strArray[4][1] = 'i';
    strArray[4][2] = 's';
    strArray[4][3] = 'h';
    strArray[4][4] = 'w';
    strArray[4][5] = 'a';
    strArray[4][6] = '\0'; //NULL TERMINATING CHARACTER

    // ****** ROW 5/ STRING 5 ******
    strArray[5][0] = 'K';
    strArray[5][1] = 'h';
    strArray[5][2] = 'u';
    strArray[5][3] = 'd';
    strArray[5][4] = 'e';
    strArray[5][5] = '\0'; //NULL-TERMINATING CHARACTER

    printf("\n\n");
    printf("The Strings In the 2d Chracter Array Are : \n\n");

    for (i = 0; i < strArray_num_rows; i++)
        printf("%s", strArray[i]);

    printf("\n\n");

    return(0);
}

int MyStrlen(char str[])
{
    //variable declarations
    int j;
    int string_length = 0;

    //code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER ( \0 ) ***
    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if (str[j] == '\0')
            break;
        else 
            string_length++;
    }

    return(string_length);
}
