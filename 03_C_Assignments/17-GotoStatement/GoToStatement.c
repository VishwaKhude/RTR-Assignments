#include <stdio.h>
#include <string.h> //for toupper()

int main(void)
{
    //variable declaration
    char ch, ch_i;
    unsigned int ascii_ch = 0;

    //code
    printf("\n\n");
    printf("Enter the first character of first name :");
    ch = getch();

    for ( ch_i = 0; ch_i <= 'Z'; ch_i++)
    {
        if(ch == ch_i)
        {
            ascii_ch = (unsigned int)ch;
            goto result_output;
        }
    }
    
    printf("\n\n");
    printf("Goto statement not executed, so printing \"Hello, world !!!\".\n");

    result_output:
        printf("\n\n");

        if (ascii_ch == 0)
        {
            printf("You must have a strange name ! could not find the charater '%c' in the entire English alphabet!\n", ch);
        }
        else 
        {
            printf("Character '%c' found. it has ascii value %u.\n", ch, ascii_ch);
        }
        printf("\n\n");
        return(0);
}
