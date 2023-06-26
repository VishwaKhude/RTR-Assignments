#include <stdio.h> //'stdio.h' contains declaration of 'printf()'

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char *argv[], char *envp[])
{
    //variable declarations
    int VMK_i;

    //code
    printf("\n\n");
    printf("Hello World !!!\n\n"); //Library Function

    printf("Number Of Command Line Arguments =%d\n\n", argc);

    printf("Command Line Arguments Passed To This Program Are : \n\n");
    for (VMK_i = 0; VMK_i < argc; VMK_i++)
    {
        printf("Command Line Argument Number = %d = %s\n", (VMK_i + 1), argv[VMK_i]);
    }
    printf("\n\n");

    printf("First 20 Environmental Variables Passed To This Program Are :\n\n");
    for (VMK_i = 0; VMK_i < 20; VMK_i++)
    {
        printf("Environmental Variable Number = %d = %s\n", (VMK_i + 1), envp[VMK_i]);
    } 
    printf("\n\n");
    return(0);
}
